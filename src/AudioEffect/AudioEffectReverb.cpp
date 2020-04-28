//
// Created by Jason Smith on 3/19/20.
//  Copyright © 2020 Jason Smith. All rights reserved.
//

#include "AudioEffectReverb.h"

#include "Vector.h"
#include "Lfo.h"

CAudioEffectReverb::CAudioEffectReverb()
{
    m_eEffectType = Effect_t::kReverb;
    m_iNumChannels = 0;
    m_fSampleRateInHz = 0;
    m_bIsInitialized = false;
    m_iNumFilters = 0;
    m_afFilterDelaysInSec = 0;
    m_ppCAudioEffectBiquad = 0;
};

CAudioEffectReverb::CAudioEffectReverb(float fSampleRateInHz, int iNumChannels, float fMaxDelayInSec, EffectParam_t params[] = NULL, float values[] = NULL, int iNumParams = 0)
{
    m_eEffectType = Effect_t::kReverb;
    m_iNumChannels = 0;
    m_fSampleRateInHz = 0;
    m_bIsInitialized = false;
    m_iNumFilters = 0;
    m_afFilterDelaysInSec = 0;
    m_ppCAudioEffectBiquad = 0;
    
    init(fSampleRateInHz, iNumChannels, fMaxDelayInSec, params, values, iNumParams);
};


CAudioEffectReverb::~CAudioEffectReverb()
{

};

Error_t CAudioEffectReverb::init(float fSampleRateInHz, int iNumChannels, float fMaxDelayInSec, EffectParam_t params[], float values[], int iNumParams)
{
    m_fSampleRateInHz = fSampleRateInHz;
    m_iNumChannels = iNumChannels;

    for (int i = 0; i < iNumParams; i++)
    {
        switch (params[i]) {
            case kParamNumFilters:
                m_iNumFilters = int(values[i]);
                break;
            case kParamFilterGains:
                m_fFilterGain = values[i];
                break;
            default:
                return kNoError;
        }
    }
    
    m_afFilterDelaysInSec = new float[m_iNumFilters];
    for (int f = 0; f < m_iNumFilters; f++)
        m_afFilterDelaysInSec[f] = 0.f;
    
    m_ppCAudioEffectBiquad = new CAudioEffectBiquad*[m_iNumFilters];
    
    for (int i = 0; i < m_iNumFilters; i++)
    {
        CAudioEffect::EffectParam_t param[2];
        float value[2];

        param[0] = CAudioEffect::kParamGain;
        value[0] = m_fFilterGain;
        param[1] = CAudioEffect::kParamDelayInSecs;
        value[1] = m_afFilterDelaysInSec[i];
        
        m_ppCAudioEffectBiquad[i] = new CAudioEffectBiquad(m_fSampleRateInHz, m_iNumChannels, CAudioEffectBiquad::kAllpass, param, value, 2);

    }

    m_bIsInitialized = true;

    return kNoError;
};

Error_t CAudioEffectReverb::reset()
{
    if (!m_bIsInitialized)
        return kNotInitializedError;
    
    for (int i = 0; i < m_iNumFilters; i++)
    {
        m_ppCAudioEffectBiquad[i]->reset();
    }
    delete [] m_ppCAudioEffectBiquad;
    
    m_bIsInitialized = false;
    
    return kNoError;
};

Error_t CAudioEffectReverb::setParam(EffectParam_t eParam, float fValue)
{
    if (!m_bIsInitialized)
        return kNotInitializedError;

    switch (eParam) {
        case CAudioEffect::kParamFilterGains:
            m_fFilterGain = fValue;
            for (int i = 0; i < m_iNumFilters; i++)
                m_ppCAudioEffectBiquad[i]->setParam(kParamGain, m_fFilterGain);
            break;
        case CAudioEffect::kParamNumFilters:
            m_iNumFilters = int(fValue);
            for (int i = 0; i < m_iNumFilters; i++)
            {
                CAudioEffect::EffectParam_t param[1];
                float value[1];

                param[0] = CAudioEffect::kParamGain;
                value[0] = m_fFilterGain;
                
                m_ppCAudioEffectBiquad[i]->init(m_fSampleRateInHz, m_iNumChannels, CAudioEffectBiquad::kAllpass, param, value, 3, m_afFilterDelaysInSec[i]);
            }
            break;
        default:
            return kFunctionInvalidArgsError;
    }

    return kNoError;
};

float CAudioEffectReverb::getParam(EffectParam_t eParam)
{
    if (!m_bIsInitialized)
        return kNotInitializedError;

    switch (eParam) {
        case kParamNumFilters:
            return m_iNumFilters;
        case kParamFilterGains:
            return m_fFilterGain;
        default:
            return 0.f;
            break;
    }
};

Error_t CAudioEffectReverb::setFilterDelays(float fValues[], int iNumFilters)
{
    if (iNumFilters != m_iNumFilters)
        return kFunctionInvalidArgsError;
    
    for (int i = 0; i < iNumFilters; i++) {
        m_afFilterDelaysInSec[i] = fValues[i];
        m_ppCAudioEffectBiquad[i]->setParam(kParamDelayInSecs, m_afFilterDelaysInSec[i]);
    }
    
    return kNoError;
};


Error_t CAudioEffectReverb::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
{
    float ** ppfTempBuffer = new float*[m_iNumChannels];
    
    for(int c = 0; c < m_iNumChannels; c++)
    {
        ppfTempBuffer[c] = new float[iNumberOfFrames];
        for (int i = 0; i < iNumberOfFrames; i++)
        {
            ppfTempBuffer[c][i] = ppfInputBuffer[c][i];
            ppfOutputBuffer[c][i] = ppfInputBuffer[c][i];
        }
    }
    
    Error_t err = kNoError;
    
    for (int f = 0; f < m_iNumFilters; f++)
    {
        err = m_ppCAudioEffectBiquad[f]->process(ppfTempBuffer, ppfTempBuffer, iNumberOfFrames);
        
        for(int c = 0; c < m_iNumChannels; c++)
        {
            for (int i = 0; i < iNumberOfFrames; i++)
            {
                ppfOutputBuffer[c][i] = ppfOutputBuffer[c][i] + ppfTempBuffer[c][i];
            }
        }
    }
    
    for(int c = 0; c < m_iNumChannels; c++)
    {
        delete [] ppfTempBuffer[c];
    }
    delete [] ppfTempBuffer;
    
    return err;
};

