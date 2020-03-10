//
// Created by Rishikesh Daoo on 2/15/20.
//  Copyright © 2020 Rishikesh Daoo. All rights reserved.
//

#include "AudioEffectDelay.h"

#include "RingBuffer.h"
#include "Lfo.h"

CAudioEffectDelay::CAudioEffectDelay()
{
    m_eEffectType = Effect_t::kDelay;
    m_fGain = 0.f;
    m_fDelay = 0.f;
    m_iNumChannels = 0;
    m_fSampleRateInHz = 0;
    m_bIsInitialized = false;
    m_ppCRingBuffer = 0;
    m_fMaxDelay = 0;
};

CAudioEffectDelay::CAudioEffectDelay(float fSampleRateInHz, int iNumChannels, int iMaxDelayInSec, EffectParam_t params[] = NULL, float values[] = NULL, int iNumParams = 0)
{
    m_eEffectType = Effect_t::kDelay;
    init(fSampleRateInHz, iNumChannels, iMaxDelayInSec, params, values, iNumParams);

    m_ppCRingBuffer = new CRingBuffer<float>*[m_iNumChannels];
    for (int c = 0; c < m_iNumChannels; c++)
        m_ppCRingBuffer[c]  = new CRingBuffer<float>(iMaxDelayInSec * m_fSampleRateInHz);
};


CAudioEffectDelay::~CAudioEffectDelay()
{

};

Error_t CAudioEffectDelay::init(float fSampleRateInHz, int iNumChannels, int iMaxDelayInSec, EffectParam_t params[], float values[], int iNumParams)
{
    m_fSampleRateInHz = fSampleRateInHz;
    m_iNumChannels = iNumChannels;

    m_fMaxDelay = iMaxDelayInSec * m_fSampleRateInHz;
    
    m_pCLfo             = new CLfo(m_fSampleRateInHz);

    m_ppCRingBuffer = new CRingBuffer<float>*[m_iNumChannels];
    for (int c = 0; c < m_iNumChannels; c++)
        m_ppCRingBuffer[c]  = new CRingBuffer<float>(iMaxDelayInSec * m_fSampleRateInHz);

    for (int i = 0; i < iNumParams; i++)
    {
        switch (params[i]) {
            case kParamGain:
                m_fGain = values[i];
                break;
            case kParamDelayInSecs:
                m_fDelay = values[i] * m_fSampleRateInHz;

                if(m_fDelay > m_fMaxDelay)
                    return kFunctionInvalidArgsError;

                for (int c = 0; c < m_iNumChannels; c++) {
                    for (int i = 0; i < m_fDelay; i++) {
                        m_ppCRingBuffer[c]->putPostInc(0.F);
                    }
                }

                break;
            default:
                return kNoError;
        }
    }

    m_bIsInitialized = true;

    return kNoError;
};

Error_t CAudioEffectDelay::reset()
{
    delete m_pCLfo;
    m_pCLfo             = 0;
    
    return kNoError;
};

Error_t CAudioEffectDelay::setParam(EffectParam_t eParam, float fValue)
{
    if (!m_bIsInitialized)
        return kNotInitializedError;

    switch (eParam) {
        case kParamGain:
            m_fGain = fValue;
            break;
        case kParamDelayInSecs:
            m_fDelay = fValue * m_fSampleRateInHz;

            if(m_fDelay > m_fMaxDelay)
                return kFunctionInvalidArgsError;

            for (int c = 0; c < m_iNumChannels; c++) {
                for (int i = 0; i < m_fDelay; i++) {
                    m_ppCRingBuffer[c]->putPostInc(0.F);
                }
            }
        
        case kParamModFreqInHz:
            return m_pCLfo->setParam(CLfo::kLfoParamFrequency, fValue);
        case kParamModWidthInS:
            return m_pCLfo->setParam(CLfo::kLfoParamAmplitude, fValue * m_fSampleRateInHz);

    
        default:
            return kFunctionInvalidArgsError;
    }

    return kNoError;
};

float CAudioEffectDelay::getParam(EffectParam_t eParam)
{
    if (!m_bIsInitialized)
        return kNotInitializedError;

    switch (eParam) {
        case kParamGain:
            return m_fGain;
            break;
        case kParamDelayInSecs:
            return m_fDelay;
        case kParamModFreqInHz:
            return m_pCLfo->getParam(CLfo::kLfoParamFrequency);
        case kParamModWidthInS:
            return m_pCLfo->getParam(CLfo::kLfoParamAmplitude) / m_fSampleRateInHz;
        default:
            return 0.f;
            break;
    }
    
    
};

Error_t CAudioEffectDelay::setDelayType(DelayType_t eValue)
{
    if (!m_bIsInitialized)
        return kNotInitializedError;
    
    m_eDelayType = eValue;
    switch (m_eDelayType) {
        case kDelay:
            m_fModGain = 0;
            break;
        case kFlanger:
            m_fModGain = 1;
            
        default:
            break;
    }

    
    return kNoError;
};

CAudioEffectDelay::DelayType_t CAudioEffectDelay::getDelayType()
{
    return m_eDelayType;
};


Error_t CAudioEffectDelay::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
{
    for(int c = 0; c < m_iNumChannels; c++)
    {
        for (int i = 0; i < iNumberOfFrames; i++)
        {
            ppfOutputBuffer[c][i] = ppfInputBuffer[c][i] + m_fGain * m_ppCRingBuffer[c]->getPostInc();
            m_ppCRingBuffer[c]->putPostInc(ppfOutputBuffer[c][i]);
        }
    }
    return kNoError;
};

