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
    CAudioEffectDistortion(EffectParam_t params[], float values[], int iNumParams, float fSampleRateInHz, int iNumChannels);
    ~CAudioEffectDistortion();
    
    Error_t init(EffectParam_t params[], float values[], int iNumParams, float fSampleRateInHz, int iNumChannels);
    Error_t reset();
    
    Error_t setParam(EffectParam_t eParam, float fValue);
    float getParam(EffectParam_t eParam);
    
    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
    
protected:
    
    float m_fDistortion;
    
};
#endif /* AudioEffectDistortion_h */
