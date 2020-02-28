#ifndef AudioEffectPan_h
#define AudioEffectPan_h

#include "AudioEffect.h"

#include <stdio.h>
#include <iostream>

class CAudioEffectPan: public CAudioEffect
{
public:
    CAudioEffectPan();
    CAudioEffectPan(float fSampleRateInHz, int iNumChannels, EffectParam_t params[], float values[], int iNumParams);
    ~CAudioEffectPan();
    
    Error_t init(float fSampleRateInHz, int iNumChannels, EffectParam_t params[], float values[], int iNumParams);
    Error_t reset();
    
    Error_t setParam(EffectParam_t eParam, float fValue);
    float getParam(EffectParam_t eParam);
    
    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
    
protected:
    
    float m_fPan;
    
};

#endif /* AudioEffectPan_h */
