//
// Created by Rishikesh Daoo on 2/15/20.
//

#include <EffectDef.h>
#include "AudioEffectDelay.h"

CAudioEffectDelay::CAudioEffectDelay()
{
    m_eEffectType = kEffectGain;
    m_fGain = 0.f;
    m_iNumChannels = 0;
    m_fSampleRateInHz = 0;
    m_bIsInitialized = false;
};

CAudioEffectDelay::CAudioEffectDelay(EffectParam_t params[], float values[], int iNumParams, float fSampleRateInHz, int iNumChannels)
{
    m_eEffectType = kEffectDelay;
    init(params, values, iNumParams, fSampleRateInHz, iNumChannels);
}

CAudioEffectDelay::~CAudioEffectDelay()
{

};

Error_t CAudioEffectDelay::init(EffectParam_t params[], float values[], int iNumParams, float fSampleRateInHz, int iNumChannels)
{

    m_fSampleRateInHz = fSampleRateInHz;
    m_iNumChannels = iNumChannels;

    for (int i = 0; i < iNumParams; i++)
    {
        switch (params[i]) {
            case kParamGain:
                m_fGain = values[i];
                break;
            case kParamDelay:
                m_fDelayInSec = values[i];
            default:
                break;
        }
    }

    m_bIsInitialized = true;

    return kNoError;
};

Error_t CAudioEffectDelay::reset()
{
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
        case kParamDelay:
            m_fDelayInSec = fValue;
        default:
            return kFunctionInvalidArgsError;
            break;
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
        case kParamDelay:
            return m_fDelayInSec;
        default:
            return 0.f;
            break;
    }
};

Error_t CAudioEffectDelay::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
{
    for (int channel = 0; channel < m_iNumChannels; channel++)
    {
        for (int frame = 0; frame < iNumberOfFrames; frame++)
        {
            ppfOutputBuffer[channel][frame] = ppfInputBuffer[channel][frame];
        }
    }
    return kNoError;
};
