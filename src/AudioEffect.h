//
//  AudioEffect.h
//  AudioEffectsFramework
//
//  Created by Jason Smith on 1/31/20.
//  Copyright © 2020 Jason Smith. All rights reserved.
//

#ifndef AudioEffect_h
#define AudioEffect_h

#include "AudioEffectIF.h"

#include <stdio.h>

class CAudioEffect: public CAudioEffectIF
{
public:
    CAudioEffect();
    ~CAudioEffect();
    
    void initIntern(EffectType_t eEffectType, EffectParam_t params[], int iNumParams, float fSampleRateInHz, int iNumChannels);
    void resetIntern();
    
    void setParamIntern(EffectType_t eParam, float fValue);
    void getParamIntern(EffectType_t eParam);
    
    void processIntern(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
    
protected:
    
    EffectType_t eEffectType;
    
};

#endif /* AudioEffect_h */
