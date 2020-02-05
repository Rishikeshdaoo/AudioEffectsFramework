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
#include <ErrorDef.h>

class CAudioEffect: public CAudioEffectIF
{
public:
    CAudioEffect();
    ~CAudioEffect();

private:
    Error_t initIntern(EffectType_t eEffectType, EffectParam_t params[], int iNumParams, float fSampleRateInHz, int iNumChannels);
    Error_t resetIntern();

    Error_t setParamIntern(EffectType_t eParam, float fValue);
    float getParamIntern(EffectType_t eParam) const;

    Error_t processIntern(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
    
protected:
    
    EffectType_t eEffectType;
    
};

#endif /* AudioEffect_h */
