//
//  AudioEffectDistortion.h
//  AudioEffectsFramework
//
//  Created by Snehesh Nag on 2/21/20.
//  Copyright © 2020 Snehesh Nag. All rights reserved.
//

#ifndef AudioEffectDistortion_h
#define AudioEffectDistortion_h

#include "AudioEffect.h"

#include <stdio.h>
#include <iostream>

class CAudioEffectDistortion: public CAudioEffect
{
public:
    CAudioEffectDistortion();
    CAudioEffectDistortion(float fSampleRateInHz, int iNumChannels, EffectParam_t params[], float values[], int iNumParams);
    ~CAudioEffectDistortion();
    
    Error_t init(float fSampleRateInHz, int iNumChannels, EffectParam_t params[], float values[], int iNumParams);
    Error_t reset();
    
    Error_t setParam(EffectParam_t eParam, float fValue);
    float getParam(EffectParam_t eParam);
    
    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
    
protected:
    
    float m_fGain;
    float m_fDryWetMix;
    
};
#endif /* AudioEffectDistortion_h */
