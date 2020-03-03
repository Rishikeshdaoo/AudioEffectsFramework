//
// Created by Rishikesh Daoo on 2/24/20.
// Copyright © 2020 Rishikesh Daoo. All rights reserved.
//


#include <AudioEffectCompressor.h>

CAudioEffectCompressor::CAudioEffectCompressor():   m_fTav(0.01),
                                                    m_fAt(0.03),
                                                    m_fRt(0.003)
{
    m_eEffectType = kCompressor;
    m_iNumChannels = 0;
    m_fSampleRateInHz = 0;
    m_bIsInitialized = false;

    m_fCompressorThreshold = 0.f;
    m_fCompressorSlope = 0.f;
    m_fExpanderThreshold = 0.f;
    m_fExpanderSlope = 0.f;
}

CAudioEffectCompressor::CAudioEffectCompressor(float fSampleRateInHz, int iNumChannels, int iMaxDelayInSec, EffectParam_t params[] = NULL, float values[] = NULL, int iNumParams = 0):
        m_fTav(0.01),
        m_fAt(0.03),
        m_fRt(0.003)
{
    m_eEffectType = kBiquad;
    init(fSampleRateInHz, iNumChannels, iMaxDelayInSec, params, values, iNumParams);
}

Error_t CAudioEffectCompressor::init(float fSampleRateInHz, int iNumChannels, int iMaxDelayInSec, EffectParam_t params[] = NULL, float values[] = NULL, int iNumParams = 0)
{

    m_fSampleRateInHz = fSampleRateInHz;
    m_iNumChannels = iNumChannels;
    m_bIsInitialized = true;

    assert(iMaxDelayInSec > 0);
    assert(iNumChannels > 0);

    m_iMaxDelay = static_cast<int>(iMaxDelayInSec * m_fSampleRateInHz);

    for (int i = 0; i < iNumParams; i++)
    {
        switch (params[i]) {
            case kParamCompressorThreshold:
                m_fCompressorThreshold = values[i];
                break;
            case kParamCompressorSlope:
                m_fCompressorSlope = values[i];
                break;
            case kParamExpanderThreshold:
                m_fExpanderThreshold = values[i];
                break;
            case kParamExpanderSlope:
                m_fExpanderSlope = values[i];
                break;
            default:
                break;
        }
    }

    m_ppfDelayBuffer = new CRingBuffer<float>*[m_iNumChannels];
    for (int c = 0; c < m_iNumChannels; c++)
        m_ppfDelayBuffer[c]  = new CRingBuffer<float>(m_iMaxDelay);

    return kNoError;
}

Error_t CAudioEffectCompressor::reset(){

    return kNoError;
}

Error_t CAudioEffectCompressor::setParam(EffectParam_t eParam, float fValue)
{
    if (!m_bIsInitialized)
        return kNotInitializedError;

    switch (eParam) {
        case kParamCompressorThreshold:
            m_fCompressorThreshold = fValue;
            break;
        case kParamCompressorSlope:
            m_fCompressorSlope = fValue;
            break;
        case kParamExpanderThreshold:
            m_fExpanderThreshold = fValue;
            break;
        case kParamExpanderSlope:
            m_fExpanderSlope = fValue;
            break;
        default:
            return kFunctionInvalidArgsError;
    }

    return kNoError;
}

float CAudioEffectCompressor::getParam(EffectParam_t eParam)
{
    if (!m_bIsInitialized)
        return kNotInitializedError;

    switch (eParam) {
        case kParamCompressorThreshold:
            return m_fCompressorThreshold;
            break;
        case kParamCompressorSlope:
            return m_fCompressorSlope;
            break;
        case kParamExpanderThreshold:
            return m_fExpanderThreshold;
            break;
        case kParamExpanderSlope:
            return m_fExpanderSlope;
            break;
        default:
            return 0.f;
            break;
    }
}

Error_t CAudioEffectCompressor::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames){

    float f_inputSample = 0.f;
    float f_rms = 0.f;
    float f_X = 0.f;
    float f_G = 0.f;
    float f_g = 1.f;
    float f_f = 0.f;
    float f_coeff = 0.f;

    for(int c = 0; c < m_iNumChannels; c++)
    {
        for (int i = 0; i < iNumberOfFrames; i++)
        {
            f_inputSample = ppfInputBuffer[c][i];
            f_rms = (1 - m_fTav) * f_rms + m_fTav * (pow(f_inputSample, 2));
            f_X = 10 * log10(f_rms);
            f_G = fmin(m_fCompressorSlope * (m_fCompressorThreshold - f_X), m_fExpanderSlope * (m_fExpanderThreshold - f_X));

            f_f = pow(10, (f_G/20));

            if(f_f < f_G)
                f_coeff = m_fAt;
            else
                f_coeff = m_fRt;

            f_g = (1 - f_coeff) * f_g + f_coeff * f_f;

            ppfOutputBuffer[c][i] = m_ppfDelayBuffer[c]->getPostInc() * f_g;

            m_ppfDelayBuffer[c]->putPostInc(ppfInputBuffer[c][i]);
        }
    }
    return kNoError;
}

