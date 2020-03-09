//
//  AudioEffectDistortion.h
//  AudioEffectsFramework
//
//  Created by Snehesh Nag on 2/21/20.
//  Copyright © 2020 Snehesh Nag. All rights reserved.
//

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

CAudioEffectDistortion::CAudioEffectDistortion(float fSampleRateInHz, int iNumChannels, EffectParam_t params[] = NULL, float values[] = NULL, int iNumParams = 0)
{
    m_eEffectType = kDistortion;
    init(fSampleRateInHz, iNumChannels, params, values, iNumParams);
};

CAudioEffectDistortion::~CAudioEffectDistortion()
{
    
};

Error_t CAudioEffectDistortion::init(float fSampleRateInHz, int iNumChannels, EffectParam_t params[] = NULL, float values[] = NULL, int iNumParams = 0)
{
    
    m_fSampleRateInHz = fSampleRateInHz;
    m_iNumChannels = iNumChannels;
    
    for (int i = 0; i < iNumParams; i++)
    {
        switch (params[i]) {
            case kParamGain:
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
        case kParamDistortion:
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
        case kParamDistortion:
            return m_fDistortion;
            break;
            
        default:
            return 0.f;
            break;
    }
};

Error_t CAudioEffectDistortion::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
{
    auto k = 2 * m_fDistortion /(1 - m_fDistortion);
    for (int channel = 0; channel < m_iNumChannels; channel++)
    {
        for (int frame = 0; frame < iNumberOfFrames; frame++)
        {
            ppfOutputBuffer[channel][frame] = (1 + k) * ppfInputBuffer[channel][frame] / (1 + k * abs(ppfInputBuffer[channel][frame]));
        }
    }
    return kNoError;
};

