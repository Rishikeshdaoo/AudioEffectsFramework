//
//  AudioEffectGain.cpp
//  AudioEffectsFramework
//
//  Created by Jason Smith on 2/05/20.
//  Copyright © 2020 Jason Smith. All rights reserved.
//

#include "AudioEffectGain.h"

CAudioEffectGain::CAudioEffectGain()
{
    m_eEffectType = kGain;
    m_fGain = 0.f;
    m_iNumChannels = 0;
    m_fSampleRateInHz = 0;
    m_bIsInitialized = false;
};

CAudioEffectGain::CAudioEffectGain(float fSampleRateInHz, int iNumChannels, EffectParam_t params[], float values[], int iNumParams)
{
    m_eEffectType = kGain;
    init(fSampleRateInHz, iNumChannels, params, values, iNumParams);
};

CAudioEffectGain::~CAudioEffectGain()
{
    this->reset();
};

Error_t CAudioEffectGain::init(float fSampleRateInHz, int iNumChannels, EffectParam_t params[], float values[], int iNumParams)
{
    
    m_fSampleRateInHz = fSampleRateInHz;
    m_iNumChannels = iNumChannels;
    
    assert(iNumChannels > 0);

    if(params == NULL || values == NULL) {
        iNumParams = 1;
        params = (EffectParam_t*) new int(iNumParams);
        values = new float[iNumParams];
        
        params[0] = CAudioEffect::kParamGain;
        values[0] = 1.f;
        }
    
    for (int i = 0; i < iNumParams; i++)
    {
        switch (params[i]) {
            case kParamGain:
                if(values[i]<0 || values[i]>1)
                    return kFunctionInvalidArgsError;
                m_fGain = values[i];
                break;
            default:
                return kFunctionInvalidArgsError;
        }
    }
    
    m_bIsInitialized = true;
    
    return kNoError;
};

Error_t CAudioEffectGain::reset()
{
    m_iNumChannels = 0;
    m_fSampleRateInHz = 0;
    m_bIsInitialized = false;

    return kNoError;
};

Error_t CAudioEffectGain::setParam(EffectParam_t eParam, float fValue)
{
    if (!m_bIsInitialized)
    return kNotInitializedError;
    
    switch (eParam) {
        case kParamGain:
            if(fValue<0 || fValue>1)
                return kFunctionInvalidArgsError;
            m_fGain = fValue;
            break;
        default:
            return kFunctionInvalidArgsError;
            break;
    }
    return kNoError;
};

float CAudioEffectGain::getParam(EffectParam_t eParam)
{
    if (!m_bIsInitialized)
    return kNotInitializedError;
    
    switch (eParam) {
        case kParamGain:
            return m_fGain;
            break;
            
        default:
            return 0.f;
            break;
    }
};

Error_t CAudioEffectGain::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
{
    for (int channel = 0; channel < m_iNumChannels; channel++)
    {
        for (int frame = 0; frame < iNumberOfFrames; frame++)
        {
            ppfOutputBuffer[channel][frame] = m_fGain * ppfInputBuffer[channel][frame];
        }
    }
    return kNoError;
};
