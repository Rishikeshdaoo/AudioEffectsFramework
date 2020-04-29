//
// Created by Rishikesh Daoo on 2/24/20.
// Copyright © 2020 Rishikesh Daoo. All rights reserved.
//


#include <AudioEffectCompressor.h>
#include <iostream>

CAudioEffectCompressorExpander::CAudioEffectCompressorExpander():   m_fAveragingTime(0.01),
                                                                    m_fAttackTime(0.03),
                                                                    m_fReleaseTime(0.003)
{
    m_eEffectType = kCompressorExpander;
    m_eCompressorType = kNone;
    m_iNumChannels = 0;
    m_fSampleRateInHz = 0;

    m_fThreshold = 0.f;
    m_fSlope = 0.f;
    m_iLookaheadBufferSize = 0;

    m_bIsInitialized = false;
}

CAudioEffectCompressorExpander::CAudioEffectCompressorExpander(Effect_t effectType, float fSampleRateInHz, int iNumChannels, EffectSubtype_t subType, int iLookaheadBufferSize, EffectParam_t params[], float values[], int iNumParams):
        m_fAveragingTime(0.01),
        m_fAttackTime(0.03),
        m_fReleaseTime(0.003)
{
    init(effectType, fSampleRateInHz, iNumChannels, subType, iLookaheadBufferSize, params, values, iNumParams);
}

CAudioEffectCompressorExpander::~CAudioEffectCompressorExpander(){
    this->reset();
}

Error_t CAudioEffectCompressorExpander::init(Effect_t effectType, float fSampleRateInHz, int iNumChannels, EffectSubtype_t subType, int iLookaheadBufferSize, EffectParam_t params[], float values[], int iNumParams)
{
    bool bInvalidParam = false;
    
    m_eEffectType = effectType;
    m_eCompressorType = subType;
    m_fSampleRateInHz = fSampleRateInHz;
    m_iNumChannels = iNumChannels;
    m_iLookaheadBufferSize = iLookaheadBufferSize;
    m_bIsInitialized = true;

    if(iNumChannels < 1)
        return kChannelError;

    if(params == NULL || values == NULL) {
        iNumParams = 2;
        params = (EffectParam_t*) new int(iNumParams);
        values = new float[iNumParams];

        if(subType == kCompressor) {
            params[0] = CAudioEffect::kParamThreshold;
            values[0] = -50.f;
            params[1] = CAudioEffect::kParamSlope;
            values[1] = 0.5f;
        }
        else if(subType == kExpander){
            params[0] = CAudioEffect::kParamThreshold;
            values[0] = -20.f;
            params[1] = CAudioEffect::kParamSlope;
            values[1] = -2.f;
        }
    }

    for (int i = 0; i < iNumParams; i++) {
        switch (params[i]) {
            case kParamThreshold:
                m_fThreshold = values[i];
                break;
            case kParamSlope:
                // Compressor: 0 < CS < 1
                // Expander: ES < 0
                if (m_eCompressorType == kCompressor) {
                    if (values[i] >= 0 && values[i] <= 1)
                        m_fSlope = values[i];
                    else
                        return kFunctionInvalidArgsError;
                } else if (m_eCompressorType == kExpander) {
                    if (values[i] <= 0)
                        m_fSlope = values[i];
                    else
                        return kFunctionInvalidArgsError;
                }
                break;
            default:
                bInvalidParam = true;
                break;
        }
    }

    m_pfRmsSignal = new float[m_iNumChannels];
    m_pf_gain = new float[m_iNumChannels];
    m_ppfDelayBuffer = new CRingBuffer<float>*[m_iNumChannels];
    for (int c = 0; c < m_iNumChannels; c++)
    {
        m_pfRmsSignal[c] = 0;
        m_pf_gain[c] = 1;
        m_ppfDelayBuffer[c]  = new CRingBuffer<float>(m_iLookaheadBufferSize);
        for (int i = 0; i < m_iLookaheadBufferSize; i++) {
            m_ppfDelayBuffer[c]->putPostInc(0.F);
        }
    }

    if (bInvalidParam)
        return kFunctionInvalidArgsError;
    else
        return kNoError;
}

Error_t CAudioEffectCompressorExpander::reset(){

    if (!m_bIsInitialized)
        return kNotInitializedError;
    
    for (int c= 0; c < m_iNumChannels; c++)
        delete m_ppfDelayBuffer[c];
    delete [] m_ppfDelayBuffer;
    m_ppfDelayBuffer = 0;

    m_iNumChannels = 0;
    m_fSampleRateInHz = 0;
    m_bIsInitialized = false;

    return kNoError;
}

Error_t CAudioEffectCompressorExpander::setParam(EffectParam_t eParam, float fValue)
{
    if (!m_bIsInitialized)
        return kNotInitializedError;

    switch (eParam) {
        case kParamThreshold:
            m_fThreshold = fValue;
            break;
        case kParamSlope:
             if(m_eCompressorType==kCompressor){
                 if(fValue<0 || fValue>1)
                     return kFunctionInvalidArgsError;
             }
             else if (m_eCompressorType==kExpander){
                 if(fValue>0)
                     return kFunctionInvalidArgsError;
             }
            m_fSlope = fValue;
            break;
        default:
            return kFunctionInvalidArgsError;
    }

    return kNoError;
}

float CAudioEffectCompressorExpander::getParam(EffectParam_t eParam)
{
    if (!m_bIsInitialized)
        return kNotInitializedError;

    switch (eParam) {
        case kParamThreshold:
            return m_fThreshold;
            break;
        case kParamSlope:
            return m_fSlope;
            break;
        default:
            return 0.f;
            break;
    }
}

Error_t CAudioEffectCompressorExpander::setEffectSubtype(EffectSubtype_t eValue) {

    if (!m_bIsInitialized)
        return kNotInitializedError;

    m_eCompressorType = eValue;

    return kNoError;
}

CAudioEffectCompressorExpander::EffectSubtype_t CAudioEffectCompressorExpander::getEffectSubtype() {

    return m_eCompressorType;
}

float CAudioEffectCompressorExpander::getTailLength() {
    return float(m_iLookaheadBufferSize);
}

Error_t CAudioEffectCompressorExpander::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames){

    float f_inputSample = 0.f;
    float f_logRmsSignal = 0.f;
    float f_logGain = 0.f;
    float f_linGain = 0.f;
    float f_coeff = 0.f;

    for (int c = 0; c < m_iNumChannels; c++) {
        for (int i = 0; i < iNumberOfFrames; i++) {
            f_inputSample = ppfInputBuffer[c][i];
            m_pfRmsSignal[c] = (1 - m_fAveragingTime) * m_pfRmsSignal[c] + m_fAveragingTime * (pow(f_inputSample, 2));
            f_logRmsSignal = 10 * log10(m_pfRmsSignal[c]);

            if(m_eCompressorType == kCompressor) {
                if(f_logRmsSignal > m_fThreshold)
                    f_logGain = m_fSlope * (m_fThreshold - f_logRmsSignal);
                else
                    f_logGain = 0.f;
            }
            else if(m_eCompressorType == kExpander){
                if(f_logRmsSignal < m_fThreshold && f_logRmsSignal != -(1.0/0.0))
                    f_logGain = m_fSlope * (m_fThreshold - f_logRmsSignal);
                else
                    f_logGain = 0.f;
            }

            f_linGain = pow(10, (f_logGain / 20.f));

            if (f_linGain < m_pf_gain[c])
                f_coeff = m_fAttackTime;
            else
                f_coeff = m_fReleaseTime;

            m_pf_gain[c] = (1 - f_coeff) * m_pf_gain[c] + f_coeff * f_linGain;
            ppfOutputBuffer[c][i] = m_ppfDelayBuffer[c]->getPostInc() * m_pf_gain[c];
            m_ppfDelayBuffer[c]->putPostInc(ppfInputBuffer[c][i]);
            }
        }

    return kNoError;
}
