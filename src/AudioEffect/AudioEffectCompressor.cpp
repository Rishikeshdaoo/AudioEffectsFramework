//
// Created by Rishikesh Daoo on 2/24/20.
// Copyright © 2020 Rishikesh Daoo. All rights reserved.
//


#include <AudioEffectCompressor.h>

CAudioEffectCompressor::CAudioEffectCompressor():   m_fTav(0.01),
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

CAudioEffectCompressor::CAudioEffectCompressor(float fSampleRateInHz, int iNumChannels, EffectParam_t params[] = NULL, float values[] = NULL, int iNumParams = 0):
        m_fTav(0.01),
        m_fAttackTime(0.03),
        m_fReleaseTime(0.003)
{
    m_eEffectType = kCompressor;
    init(fSampleRateInHz, iNumChannels, params, values, iNumParams);
}

CAudioEffectCompressor::~CAudioEffectCompressor()
{
};

Error_t CAudioEffectCompressor::init(float fSampleRateInHz, int iNumChannels, EffectParam_t params[] = NULL, float values[] = NULL, int iNumParams = 0)
{

    m_fSampleRateInHz = fSampleRateInHz;
    m_iNumChannels = iNumChannels;
    m_bIsInitialized = true;

    assert(iNumChannels > 0);

    int iBufferSize = 1024;

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

    m_ppfDelayBuffer = new CRingBuffer<float>*[m_iNumChannels];
    for (int c = 0; c < m_iNumChannels; c++)
        m_ppfDelayBuffer[c]  = new CRingBuffer<float>(iBufferSize);

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

float CAudioEffectCompressor::getParam(EffectParam_t eParam)
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
            m_ppfDelayBuffer[c]->putPostInc(ppfInputBuffer[c][i]);
            f_inputSample = ppfInputBuffer[c][i];
            f_rms = (1 - m_fTav) * f_rms + m_fTav * (pow(f_inputSample, 2));
            f_X = 10 * log10(f_rms);
            f_G = fmin(0, m_fSlope * (m_fThreshold - f_X));

            f_f = pow(10, (f_G/20));

            if(f_f < f_G)
                f_coeff = m_fAttackTime;
            else
                f_coeff = m_fReleaseTime;

            f_g = (1 - f_coeff) * f_g + f_coeff * f_f;

            ppfOutputBuffer[c][i] = m_ppfDelayBuffer[c]->getPostInc() * f_g;
        }
    }
    return kNoError;
}

