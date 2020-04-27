// stereo to stereo version pan
#include "AudioEffectPan.h"
#include <math.h>

CAudioEffectPan::CAudioEffectPan()
{
    m_eEffectType = kPan;
    m_fPan = 0.f;
    m_iNumChannels = 0;
    m_fSampleRateInHz = 0;
    m_bIsInitialized = false;
};

CAudioEffectPan::CAudioEffectPan(float fSampleRateInHz, int iNumChannels, EffectParam_t params[], float values[], int iNumParams)
{
    m_eEffectType = kPan;
    init(fSampleRateInHz, iNumChannels, params, values, iNumParams);
}

CAudioEffectPan::~CAudioEffectPan()
{
    this->reset();
};

Error_t CAudioEffectPan::init(float fSampleRateInHz, int iNumChannels, EffectParam_t params[], float values[], int iNumParams)
{
    
    m_fSampleRateInHz = fSampleRateInHz;
    m_iNumChannels = iNumChannels;
    
    assert(iNumChannels > 0);

    if(params == NULL || values == NULL) {
        iNumParams = 1;
        params = (EffectParam_t*) new int(iNumParams);
        values = new float[iNumParams];
        
        params[0] = CAudioEffect::kParamPan;
        values[0] = 0.f;
        }
    
    for (int i = 0; i < iNumParams; i++)
    {
        switch (params[i]) {
            case kParamPan:
                if(values[i]<-1 || values[i]>1)
                    return kFunctionInvalidArgsError;
                m_fPan = values[i];
                break;
            default:
                return kFunctionInvalidArgsError;
        }
    }
    
    m_bIsInitialized = true;
    
    return kNoError;
};

Error_t CAudioEffectPan::reset()
{
    m_iNumChannels = 0;
    m_fSampleRateInHz = 0;
    m_bIsInitialized = false;

    return kNoError;
};

Error_t CAudioEffectPan::setParam(EffectParam_t eParam, float fValue)
{
    if (!m_bIsInitialized)
    return kNotInitializedError;
    
    switch (eParam) {
        case kParamPan:
            if(fValue<-1 || fValue>1)
                return kFunctionInvalidArgsError;
            m_fPan = fValue;
            break;
        default:
            return kFunctionInvalidArgsError;
            break;
    }
    return kNoError;
};

float CAudioEffectPan::getParam(EffectParam_t eParam)
{
    if (!m_bIsInitialized)
    return kNotInitializedError;
    
    switch (eParam) {
        case kParamPan:
            return m_fPan;
            break;
            
        default:
            return 0.f;
            break;
    }
};

Error_t CAudioEffectPan::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
{
    if (m_iNumChannels > 2 || m_iNumChannels < 1)
    {
        return kFunctionInvalidArgsError;
    }
    
    // input: mono -> output: stereo
    // constant power pan rule: http://www.rs-met.com/documents/tutorials/PanRules.pdf
    else if (m_iNumChannels == 1)
    {
        float gain_left = cos(M_PI*(m_fPan+1)/4);
        float gain_right = sin(M_PI*(m_fPan+1)/4);
        for (int frame = 0; frame < iNumberOfFrames; frame++)
        {
            ppfOutputBuffer[0][frame] = gain_left * ppfInputBuffer[0][frame];
            ppfOutputBuffer[1][frame] = gain_right * ppfInputBuffer[0][frame];
        }
    }
    
    // input: stereo -> output: stereo
    else if (m_iNumChannels == 2)
    {
        for (int frame = 0; frame < iNumberOfFrames; frame++)
        {
            if (m_fPan >= 0)
            {
                ppfOutputBuffer[0][frame] = (1 - m_fPan) * ppfInputBuffer[0][frame];
                ppfOutputBuffer[1][frame] = ppfInputBuffer[1][frame];
            }
            else if (m_fPan < 0)
            {
                ppfOutputBuffer[0][frame] = ppfInputBuffer[0][frame];
                ppfOutputBuffer[1][frame] = (1 + m_fPan) * ppfInputBuffer[1][frame];
            }
        }
    }
    
    return kNoError;
};
