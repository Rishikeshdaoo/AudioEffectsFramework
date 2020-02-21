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

CAudioEffectGain::CAudioEffectGain(EffectParam_t params[], float values[], int iNumParams, float fSampleRateInHz, int iNumChannels)
{
    m_eEffectType = kGain;
    init(params, values, iNumParams, fSampleRateInHz, iNumChannels);
}

CAudioEffectGain::~CAudioEffectGain()
{

};

Error_t CAudioEffectGain::init(EffectParam_t params[], float values[], int iNumParams, float fSampleRateInHz, int iNumChannels)
{
    
    m_fSampleRateInHz = fSampleRateInHz;
    m_iNumChannels = iNumChannels;
    
    for (int i = 0; i < iNumParams; i++)
    {
        switch (params[i]) {
            case kParamGain:
                m_fGain = values[i];
                break;
            default:
                break;
        }
    }
    
    m_bIsInitialized = true;
    
    return kNoError;
};

Error_t CAudioEffectGain::reset()
{
    return kNoError;
};

Error_t CAudioEffectGain::setParam(EffectParam_t eParam, float fValue)
{
    if (!m_bIsInitialized)
    return kNotInitializedError;
    
    switch (eParam) {
        case kParamGain:
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
