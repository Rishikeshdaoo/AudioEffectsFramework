//
// Created by Rishikesh Daoo on 2/13/20.
//


#include "AEGain.h"


CAEGain::CAEGain():
    m_bIsInitialized(false),
    m_pCAEGain(0),
    m_fSampleRate(0),
    m_iNumOfChannels(0),
    m_gain(0)
{
this->reset ();
}

CAEGain::~CAEGain() {

    this->reset();
}

Error_t CAEGain::create(CAEGain*& pCAEGain) {

    pCAEGain = new CAEGain();

    if(!pCAEGain)
        return kUnknownError;

    return kNoError;
}

Error_t CAEGain::destroy(CAEGain *&pCAEGain) {

    if (!pCAEGain)
        return kUnknownError;

    pCAEGain->reset();

    delete pCAEGain;
    pCAEGain = 0;

    return kNoError;
}

Error_t CAEGain::init(float gain, float fSampleRateInHz, int iNumOfChannels) {

    reset();

    if(gain < 0.f || gain > 1.f || fSampleRateInHz <= 0 || iNumOfChannels < 0)
        return kFunctionInvalidArgsError;

    m_gain = gain;
    m_iNumOfChannels = iNumOfChannels;
    m_fSampleRate       = fSampleRateInHz;
    m_bIsInitialized    = true;

    return kNoError;
}

Error_t CAEGain::reset() {

    delete  m_pCAEGain;
    m_pCAEGain = 0;

    m_fSampleRate = 0;
    m_gain = -1;

    m_bIsInitialized = false;

    return kNoError;
}

Error_t CAEGain::setParam(ParamType_t eParam, float paramValue) {

    if (!m_bIsInitialized)
        return kNotInitializedError;

    if(eParam == kGain)
        m_gain = paramValue;

    return kNoError;
}

float CAEGain::getParam(ParamType_t eParam) const {

    if (!m_bIsInitialized)
        return kNotInitializedError;

    if(eParam != kGain)
        return kFunctionInvalidArgsError;

    return m_gain;
}


Error_t CAEGain::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames) {

    for(int c = 0; c < m_iNumOfChannels; c++)
    {
        for (int i = 0; i < iNumberOfFrames; i++)
        {
            ppfOutputBuffer[c][i] = ppfInputBuffer[c][i] * m_gain;
        }
    }
    return kNoError;
}