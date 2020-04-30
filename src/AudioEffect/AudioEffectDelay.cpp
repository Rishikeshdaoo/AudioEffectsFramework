//
// Created by Rishikesh Daoo on 2/15/20.
//  Copyright © 2020 Rishikesh Daoo. All rights reserved.
//

#include "AudioEffectDelay.h"

#include "RingBuffer.h"
#include "Lfo.h"
#include <math.h>


CAudioEffectDelay::CAudioEffectDelay()
{
    m_eEffectType = Effect_t::kDelay;
    m_fDryWetMix = 0.f;
    m_fFeedback = 0.f;
    m_fDelayInSamples = 0.f;
    m_iNumChannels = 0;
    m_fSampleRateInHz = 0;
    m_bIsInitialized = false;
    m_ppCRingBuffer = 0;
    m_fMaxDelayInSamples = 0.5;
    m_fTremoloAmount = 0.5;
    m_fModWidthInSamples = 0.f;
    
};

CAudioEffectDelay::CAudioEffectDelay(float fSampleRateInHz, int iNumChannels, DelayType_t subType, float fMaxDelayInSec, EffectParam_t params[] , float values[], int iNumParams)
{
    m_eEffectType = Effect_t::kDelay;
    m_fDryWetMix = 0.f;
    m_fFeedback = 0.f;
    m_fDelayInSamples = 0.f;
    m_iNumChannels = 0;
    m_fSampleRateInHz = 0;
    m_bIsInitialized = false;
    m_ppCRingBuffer = 0;
    m_fMaxDelayInSamples = 0.5;
    m_fTremoloAmount = 0.5;
    m_fModWidthInSamples = 0.f;
    init(fSampleRateInHz, iNumChannels, subType, fMaxDelayInSec, params, values, iNumParams);
};


CAudioEffectDelay::~CAudioEffectDelay()
{
    
};

Error_t CAudioEffectDelay::init(float fSampleRateInHz, int iNumChannels, DelayType_t subType, float fMaxDelayInSec, EffectParam_t params[], float values[], int iNumParams)
{
    bool bInvalidParam = false;
    
    m_fSampleRateInHz = fSampleRateInHz;
    m_iNumChannels = iNumChannels;
    m_eDelayType = subType;

    if(iNumChannels < 1)
        return kChannelError;

    m_fMaxDelayInSamples = fMaxDelayInSec * m_fSampleRateInHz;
    
    m_pCLfo             = new CLfo(m_fSampleRateInHz);
    
    m_ppCRingBuffer = new CRingBuffer<float>*[m_iNumChannels];
    
    for (int c= 0; c < m_iNumChannels; c++)
    {
        m_ppCRingBuffer[c]= new CRingBuffer<float>(CUtil::float2int<int>(m_fMaxDelayInSamples+1));
    }

    if(params == NULL || values == NULL) {
        iNumParams = 5;
        params = (EffectParam_t*) new int(iNumParams);
        values = new float[iNumParams];

        if(subType == kBasicDelay) {
            params[0] = CAudioEffect::kParamDelayInSecs;
            values[0] = 0.4f;
            params[1] = CAudioEffect::kParamModRateInHz;
            values[1] = 1.5f;
            params[2] = CAudioEffect::kParamModWidthInSecs;
            values[2] = 0.01f;
            params[3] = CAudioEffect::kParamDryWetMix;
            values[3] = 0.5f;
            params[4] = CAudioEffect::kParamFeedback;
            values[4] = 0.f;
        }
        else if(subType == kChorus){
            params[0] = CAudioEffect::kParamDelayInSecs;
            values[0] = 0.04f;
            params[1] = CAudioEffect::kParamModRateInHz;
            values[1] = 0.5f;
            params[2] = CAudioEffect::kParamModWidthInSecs;
            values[2] = 0.002f;
            params[3] = CAudioEffect::kParamDryWetMix;
            values[3] = 0.7f;
            params[4] = CAudioEffect::kParamFeedback;
            values[4] = 0.5f;
        }
        else if(subType == kFlanger){
            params[0] = CAudioEffect::kParamDelayInSecs;
            values[0] = 0.002f;
            params[1] = CAudioEffect::kParamModRateInHz;
            values[1] = 0.5f;
            params[2] = CAudioEffect::kParamModWidthInSecs;
            values[2] = 0.002f;
            params[3] = CAudioEffect::kParamDryWetMix;
            values[3] = 0.7f;
            params[4] = CAudioEffect::kParamFeedback;
            values[4] = 0.5f;
        }
        else if(subType == kTremolo){
            params[0] = CAudioEffect::kParamDelayInSecs;
            values[0] = 0.005f;
            params[1] = CAudioEffect::kParamModRateInHz;
            values[1] = 10.f;
            params[2] = CAudioEffect::kParamModWidthInSecs;
            values[2] = 0.001f;
            params[3] = CAudioEffect::kParamTremoloAmount;
            values[3] = 0.5f;
            params[4] = CAudioEffect::kParamFeedback;
            values[4] = 0.f;
        }
        else if(subType == kVibrato){
            params[0] = CAudioEffect::kParamDelayInSecs;
            values[0] = 0.005f;
            params[1] = CAudioEffect::kParamModRateInHz;
            values[1] = 5.f;
            params[2] = CAudioEffect::kParamModWidthInSecs;
            values[2] = 0.001f;
            params[3] = CAudioEffect::kParamDryWetMix;
            values[3] = 0.5f;
            params[4] = CAudioEffect::kParamFeedback;
            values[4] = 0.f;
        }
    }

    for (int i = 0; i < iNumParams; i++)
    {
        switch (params[i]) {
            case kParamDryWetMix:
                m_fDryWetMix = values[i];
                break;
            case kParamDelayInSecs:
                m_fDelayInSamples = values[i] * m_fSampleRateInHz;
                
                if(m_fDelayInSamples > m_fMaxDelayInSamples)
                    return kFunctionInvalidArgsError;
                for (int c = 0; c < m_iNumChannels; c++)
                    m_ppCRingBuffer[c]->setReadIdx(-(1.f*m_fDelayInSamples+1));
                break;
            case kParamFeedback :
                m_fFeedback = values[i];
                break;
            case kParamModRateInHz :
                m_pCLfo->setParam(CLfo::kLfoParamFrequency, values[i]);
                break;
            case kParamModWidthInSecs :
                if(m_eDelayType == kFlanger && values[i] * m_fSampleRateInHz > m_fDelayInSamples)
                    return kFunctionInvalidArgsError;
                m_pCLfo->setParam(CLfo::kLfoParamAmplitude, values[i] * m_fSampleRateInHz);
                m_fModWidthInSamples = values[i] * m_fSampleRateInHz;
                break;
            case kParamTremoloAmount :
                m_fTremoloAmount = values[i];
                break;
                
            default:
                bInvalidParam = true;
                break;
        }
    }
    
    m_bIsInitialized = true;
    
    if (bInvalidParam)
        return kFunctionInvalidArgsError;
    else
        return kNoError;
};

Error_t CAudioEffectDelay::reset()
{
    
    if (!m_bIsInitialized)
        return kNotInitializedError;
    
    for (int c= 0; c < m_iNumChannels; c++)
        delete m_ppCRingBuffer[c];
    delete [] m_ppCRingBuffer;
    m_ppCRingBuffer = 0;
    delete m_pCLfo;
    m_pCLfo = 0;
    
    m_iNumChannels = 0;
    m_bIsInitialized = false;
    
    return kNoError;
};

Error_t CAudioEffectDelay::setParam(EffectParam_t eParam, float fValue)
{
    if (!m_bIsInitialized)
        return kNotInitializedError;
    
    switch (eParam) {
        case CAudioEffect::kParamDryWetMix :
            m_fDryWetMix = fValue;
            break;
        case kParamDelayInSecs:
            m_fDelayInSamples = fValue * m_fSampleRateInHz;
            
            if(m_fDelayInSamples > m_fMaxDelayInSamples)
                return kFunctionInvalidArgsError;
            for (int c = 0; c < m_iNumChannels; c++)
                m_ppCRingBuffer[c]->setReadIdx(-(1.f*m_fDelayInSamples+1));
            break;
        case kParamFeedback :
            m_fFeedback = fValue;
            break;
        case kParamModRateInHz:
            m_pCLfo->setParam(CLfo::kLfoParamFrequency, fValue);
            break;
        case kParamModWidthInSecs:
            if(kParamModWidthInSecs > m_fDelayInSamples)
                return kFunctionInvalidArgsError;
            m_pCLfo->setParam(CLfo::kLfoParamAmplitude, fValue * m_fSampleRateInHz);
            break;
        case kParamTremoloAmount :
            m_fTremoloAmount = fValue;
            break;
            
            
            
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
        case CAudioEffect::kParamDryWetMix:
            return m_fDryWetMix;
            break;
        case kParamDelayInSecs:
            return m_fDelayInSamples / m_fSampleRateInHz;
            break;
        case kParamFeedback:
            return m_fFeedback;
            break;
        case kParamModRateInHz:
            return m_pCLfo->getParam(CLfo::kLfoParamFrequency);
            break;
        case kParamModWidthInSecs:
            return m_pCLfo->getParam(CLfo::kLfoParamAmplitude) / m_fSampleRateInHz;
            break;
        case kParamTremoloAmount :
            return m_fTremoloAmount;
            break;
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
    
    return kNoError;
};

CAudioEffectDelay::DelayType_t CAudioEffectDelay::getDelayType()
{
    return m_eDelayType;
};

float CAudioEffectDelay::getTailLength()
{
    return m_fDelayInSamples + m_fModWidthInSamples;
}

Error_t CAudioEffectDelay::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
{
    for(int i = 0; i < iNumberOfFrames; i++)
    {
        float fOffset = m_pCLfo->getNext();
        
        for (int c = 0; c < m_iNumChannels; c++)
        {
            ppfOutputBuffer[c][i] = (1 - m_fDryWetMix) * (((m_eDelayType >> 4) & 1) * ppfInputBuffer[c][i] +
                                    (((m_eDelayType >> 3) & 1) * (1 + m_fTremoloAmount * fOffset/m_fModWidthInSamples)) * ppfInputBuffer[c][i]) +
                                    m_fDryWetMix * m_ppCRingBuffer[c]->get((((m_eDelayType >> 2) & 1) * m_fDelayInSamples) +
                                    (((m_eDelayType >> 1) & 1) * fOffset) -
                                    (((m_eDelayType >> 0) & 1) * abs(fOffset)));
            
            m_ppCRingBuffer[c]->getPostInc();
            
            m_ppCRingBuffer[c]->putPostInc(ppfInputBuffer[c][i] + (m_fFeedback * ppfOutputBuffer[c][i]));
        }
    }
    
    
    return kNoError;
};
