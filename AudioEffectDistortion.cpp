//
//  AudioEffectDistortion.cpp
//  AudioEffect
//
//  Created by Snehesh Nag on 2/24/20.
//

#include <stdio.h>
#include "AudioEffectDistortion.h"
#include <cmath>

CAudioEffectDistortion::CAudioEffectDistortion()
{
    m_eEffectType = kDistortion;
    m_fDistortion = 0.f;
    m_iNumChannels = 0;
    m_fSampleRateInHz = 0;
    m_bIsInitialized = false;
};

CAudioEffectDistortion::CAudioEffectDistortion(EffectParam_t params[], float values[], int iNumParams, float fSampleRateInHz, int iNumChannels)
{
    m_eEffectType = kDistortion;
    init(params, values, iNumParams, fSampleRateInHz, iNumChannels);
}

CAudioEffectDistortion::~CAudioEffectDistortion()
{
    
};

Error_t CAudioEffectDistortion::init(EffectParam_t params[], float values[], int iNumParams, float fSampleRateInHz, int iNumChannels)
{
    
    m_fSampleRateInHz = fSampleRateInHz;
    m_iNumChannels = iNumChannels;
    
    for (int i = 0; i < iNumParams; i++)
    {
        switch (params[i]) {
            case kParamDistortion:
                m_fDistortion = values[i];
                break;
            default:
                break;
        }
    }
    
    m_bIsInitialized = true;
    
    return kNoError;
};

Error_t CAudioEffectDistortion::reset()
{
    return kNoError;
};

Error_t CAudioEffectDistortion::setParam(EffectParam_t eParam, float fValue)
{
    if (!m_bIsInitialized)
        return kNotInitializedError;
    
    switch (eParam) {
        case kParamGain:
            m_fDistortion = fValue;
            break;
        default:
            return kFunctionInvalidArgsError;
            break;
    }
    return kNoError;
};

float CAudioEffectDistortion::getParam(EffectParam_t eParam)
{
    if (!m_bIsInitialized)
        return kNotInitializedError;
    
    switch (eParam) {
        case kParamGain:
            return m_fDistortion;
            break;
            
        default:
            return 0.f;
            break;
    }
};

Error_t CAudioEffectDistortion::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
{
    auto k = 2 * m_fDistortion/(1 - m_fDistortion);
    for (int channel = 0; channel < m_iNumChannels; channel++)
    {
        for (int frame = 0; frame < iNumberOfFrames; frame++)
        {
            ppfOutputBuffer[channel][frame] = (1 + m_fDistortion) * ppfInputBuffer[channel][frame] / (1 + k * abs(ppfInputBuffer[channel][frame]));
        }
    }
    return kNoError;
};
