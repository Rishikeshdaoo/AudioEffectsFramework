#ifndef AudioEffectPan_h
#define AudioEffectPan_h

#include "AudioEffect.h"

#include <stdio.h>
#include <iostream>

class CAudioEffectPan: public CAudioEffect
{
public:
    CAudioEffectPan();
    CAudioEffectPan(EffectParam_t params[], float values[], int iNumParams, float fSampleRateInHz, int iNumChannels);
    ~CAudioEffectPan();
    
    Error_t init(EffectParam_t params[], float values[], int iNumParams, float fSampleRateInHz, int iNumChannels);
    Error_t reset();
    
    Error_t setParam(EffectParam_t eParam, float fValue);
    float getParam(EffectParam_t eParam);
    
    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
    
protected:
    
    float m_fPan;
    
};

#endif /* AudioEffectPan_h */
