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
    m_eEffectType = kCompressor;
    m_iNumChannels = 0;
    m_fSampleRateInHz = 0;
    m_bIsInitialized = false;

    m_fThreshold = 0.f;
    m_fSlope = 0.f;

}

CAudioEffectCompressorExpander::CAudioEffectCompressorExpander(Effect_t effectType, float fSampleRateInHz, int iNumChannels, int iMaxDelayInSec, EffectParam_t params[] = NULL, float values[] = NULL, int iNumParams = 0):
        m_fAveragingTime(0.01),
        m_fAttackTime(0.03),
        m_fReleaseTime(0.003)
{
    init(effectType, fSampleRateInHz, iNumChannels, params, values, iNumParams);
}

CAudioEffectCompressorExpander::~CAudioEffectCompressorExpander(){

}

Error_t CAudioEffectCompressorExpander::init(Effect_t effectType, float fSampleRateInHz, int iNumChannels, EffectParam_t params[] = NULL, float values[] = NULL, int iNumParams = 0)
{
    m_eEffectType = effectType;
    m_fSampleRateInHz = fSampleRateInHz;
    m_iNumChannels = iNumChannels;
    m_bIsInitialized = true;

    assert(iNumChannels > 0);

//    int iBufferSize = 1024;
    int iBufferSize = 150;


    for (int i = 0; i < iNumParams; i++)
    {
        switch (params[i]) {
            case kParamThreshold:
                m_fThreshold = values[i];
                break;
            case kParamSlope:
                m_fSlope = values[i];
                break;
            default:
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
        m_ppfDelayBuffer[c]  = new CRingBuffer<float>(iBufferSize);
        for (int i = 0; i < iBufferSize; i++) {
            m_ppfDelayBuffer[c]->putPostInc(0.F);
        }
    }

    return kNoError;
}

Error_t CAudioEffectCompressorExpander::reset(){

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

            if(m_eEffectType == kCompressor) {
                if(f_logRmsSignal > m_fThreshold)
                    f_logGain = m_fSlope * (m_fThreshold - f_logRmsSignal);
                else
                    f_logGain = 0.f;
            }
            else if(m_eEffectType == kExpander){
                if(f_logRmsSignal < m_fThreshold)
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
