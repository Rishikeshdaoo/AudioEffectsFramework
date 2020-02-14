//
//  AudioEffectGain.cpp
//  AudioEffectsFramework
//
//  Created by Jason Smith on 2/05/20.
//  Copyright © 2020 Jason Smith. All rights reserved.
//

#ifndef AudioEffectGain_h
#define AudioEffectGain_h

#include "AudioEffect.h"

#include <stdio.h>
#include <iostream>

class CAudioEffectGain: public CAudioEffect
{
public:
    CAudioEffectGain();
    CAudioEffectGain(EffectParam_t params[], float values[], int iNumParams, float fSampleRateInHz, int iNumChannels);
    ~CAudioEffectGain();
    
    Error_t init(EffectParam_t params[], float values[], int iNumParams, float fSampleRateInHz, int iNumChannels);
    Error_t reset();
    
    Error_t setParam(EffectParam_t eParam, float fValue);
    float getParam(EffectParam_t eParam);
    
    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
    
protected:
    
    float m_fGain;
    
};

#endif /* AudioEffectGain_h */
