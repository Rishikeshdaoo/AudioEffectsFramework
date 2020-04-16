//
// Created by Jason Smith on 2/20/20.
//  Copyright © 2020 Jason Smith. All rights reserved.
//

#include "AudioEffectBiquad.h"
#include <math.h>

CAudioEffectBiquad::CAudioEffectBiquad()
{
    m_eEffectType = kBiquad;
    m_fGain = 0.f;
    m_iNumChannels = 0;
    m_fSampleRateInHz = 0;
    m_bIsInitialized = false;
    
    m_eFilterType = kLowpass;
    
    m_fCenterFrequencyInHz = 0.f;
    m_fQ = 0.f;
};

CAudioEffectBiquad::CAudioEffectBiquad(float fSampleRateInHz, int iNumChannels, EffectParam_t params[] = NULL, float values[] = NULL, int iNumParams = 0)
{
    m_eEffectType = kBiquad;
    m_fGain = 0.f;
    m_iNumChannels = 0;
    m_fSampleRateInHz = 0;
    m_bIsInitialized = false;
    
    m_eFilterType = kLowpass;
    init(fSampleRateInHz, iNumChannels, params, values, iNumParams);
}


CAudioEffectBiquad::~CAudioEffectBiquad()
{
    this->reset();
};

Error_t CAudioEffectBiquad::init(float fSampleRateInHz, int iNumChannels, EffectParam_t params[] = NULL, float values[] = NULL, int iNumParams = 0)
{

    m_fSampleRateInHz = fSampleRateInHz;
    m_iNumChannels = iNumChannels;
    m_bIsInitialized = true;
    
    for (int i = 0; i < iNumParams; i++)
    {
        switch (params[i]) {
            case kParamGain:
                m_fGain = values[i];
                break;
            case kParamCenterFrequency:
                m_fCenterFrequencyInHz = values[i];
                break;
            case kParamQ:
                m_fQ = values[i];
                break;
            default:
                break;
        }
    }

    m_fxn1 = new float[iNumChannels];
    m_fxn2 = new float[iNumChannels];
    m_fyn1 = new float[iNumChannels];
    m_fyn2 = new float[iNumChannels];
    
    for (int i = 0; i < iNumChannels; i++)
    {
        m_fxn1[i] = 0.f;
        m_fxn2[i] = 0.f;
        m_fyn1[i] = 0.f;
        m_fyn2[i] = 0.f;
    }

    return kNoError;
};

Error_t CAudioEffectBiquad::reset()
{
    
    delete m_fxn1;
    delete m_fxn2;
    delete m_fyn1;
    delete m_fyn2;

    m_iNumChannels = 0;
    m_fSampleRateInHz = 0;
    m_bIsInitialized = false;
    
    return kNoError;
};

Error_t CAudioEffectBiquad::setParam(EffectParam_t eParam, float fValue)
{
    if (!m_bIsInitialized)
        return kNotInitializedError;

    switch (eParam) {
        case kParamGain:
            m_fGain = fValue;
            break;
        case kParamCenterFrequency:
            m_fCenterFrequencyInHz = fValue;
            break;
        case kParamQ:
            m_fQ = fValue;
            break;
        default:
            return kFunctionInvalidArgsError;
            break;
    }

    return setFilterConstants();
};

float CAudioEffectBiquad::getParam(EffectParam_t eParam)
{
    if (!m_bIsInitialized)
        return kNotInitializedError;

    switch (eParam) {
        case kParamGain:
            return m_fGain;
            break;
        case kParamCenterFrequency:
            return m_fCenterFrequencyInHz;
            break;
        case kParamQ:
            return m_fQ;
            break;
        default:
            return 0.f;
            break;
    }
};

Error_t CAudioEffectBiquad::setFilterType(FilterType_t eValue)
{
    if (!m_bIsInitialized)
        return kNotInitializedError;

    m_eFilterType = eValue;
    
    return setFilterConstants();
};

CAudioEffectBiquad::FilterType_t CAudioEffectBiquad::getFilterType()
{
    return m_eFilterType;
};

Error_t CAudioEffectBiquad::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
{
    for(int c = 0; c < m_iNumChannels; c++)
    {
        for (int i = 0; i < iNumberOfFrames; i++)
        {
            ppfOutputBuffer[c][i] = m_fb0/m_fa0*ppfInputBuffer[c][i] + m_fb1/m_fa0*m_fxn1[c] + m_fb2/m_fa0*m_fxn2[c] - m_fa1/m_fa0*m_fyn1[c] - m_fa2/m_fa0*m_fyn2[c];
            
            m_fxn2[c] = m_fxn1[c];
            m_fxn1[c] = ppfInputBuffer[c][i];
            m_fyn2[c] = m_fyn1[c];
            m_fyn1[c] = ppfOutputBuffer[c][i];
        }
    }
    return kNoError;
};

Error_t CAudioEffectBiquad::setFilterConstants()
{
    float A = pow(10.f,m_fGain/40.f);
    float w0 = 2 * M_PI * (m_fCenterFrequencyInHz/m_fSampleRateInHz);
    float alpha = sin(w0)/(2*m_fQ);
    
    switch (m_eFilterType) {
        case kLowpass:
            m_fb0 =  (1 - cos(w0))/2;
            m_fb1 =   1 - cos(w0);
            m_fb2 =  (1 - cos(w0))/2;
            m_fa0 =   1 + alpha;
            m_fa1 =  -2*cos(w0);
            m_fa2 =   1 - alpha;
            break;
        case kHighpass:
            m_fb0 =  (1 + cos(w0))/2;
            m_fb1 = -(1 + cos(w0));
            m_fb2 =  (1 + cos(w0))/2;
            m_fa0 =   1 + alpha;
            m_fa1 =  -2*cos(w0);
            m_fa2 =   1 - alpha;
            break;
        case kBandpass:
            m_fb0 = alpha;
            m_fb1 = 0;
            m_fb2 = -1 * alpha;
            m_fa0 = 1 + alpha;
            m_fa1 = -2 * cos(w0);
            m_fa2 = 1 - alpha;
            break;
        case kAllpass:
            m_fb0 = 1 - alpha;
            m_fb1 = -2 * cos(w0);
            m_fb2 = 1 + alpha;
            m_fa0 = 1 + alpha;
            m_fa1 = -2 * cos(w0);
            m_fa2 = 1 - alpha;
            break;
        case kPeak:
            m_fb0 = 1 + alpha * A;
            m_fb1 = -2 * cos(w0);
            m_fb2 = 1 - alpha * A;
            m_fa0 = 1 + alpha / A;
            m_fa1 = -2 * cos(w0);
            m_fa2 = 1 - alpha / A;
            break;
        case kNotch:
            m_fb0 =   1;
            m_fb1 =  -2*cos(w0);
            m_fb2 =   1;
            m_fa0 =   1 + alpha;
            m_fa1 =  -2*cos(w0);
            m_fa2 =   1 - alpha;
            break;
        case kLowshelf:
            m_fb0 =    A*( (A+1) - (A-1)*cos(w0) + 2*sqrt(A)*alpha );
            m_fb1 =  2*A*( (A-1) - (A+1)*cos(w0)                   );
            m_fb2 =    A*( (A+1) - (A-1)*cos(w0) - 2*sqrt(A)*alpha );
            m_fa0 =        (A+1) + (A-1)*cos(w0) + 2*sqrt(A)*alpha;
            m_fa1 =   -2*( (A-1) + (A+1)*cos(w0)                   );
            m_fa2 =        (A+1) + (A-1)*cos(w0) - 2*sqrt(A)*alpha;
            break;
        case kHighshelf:
            m_fb0 =    A*( (A+1) + (A-1)*cos(w0) + 2*sqrt(A)*alpha );
            m_fb1 = -2*A*( (A-1) + (A+1)*cos(w0)                   );
            m_fb2 =    A*( (A+1) + (A-1)*cos(w0) - 2*sqrt(A)*alpha );
            m_fa0 =        (A+1) - (A-1)*cos(w0) + 2*sqrt(A)*alpha;
            m_fa1 =    2*( (A-1) - (A+1)*cos(w0)                   );
            m_fa2 =        (A+1) - (A-1)*cos(w0) - 2*sqrt(A)*alpha;
            break;
        default:
            break;
    }
    
    return kNoError;
};
