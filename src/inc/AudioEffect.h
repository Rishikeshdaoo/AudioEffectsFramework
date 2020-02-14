//
//  AudioEffect.h
//  AudioEffectsFramework
//
//  Created by Jason Smith on 1/31/20.
//  Copyright © 2020 Jason Smith. All rights reserved.
//

#ifndef AudioEffect_h
#define AudioEffect_h

#include <stdio.h>
#include <ErrorDef.h>

class CAudioEffect
{
public:
    CAudioEffect();
    ~CAudioEffect();
    
    enum EffectType_t
    {
        kEffectNone,
        kEffectGain,
        kEffectPan
    };
    
    enum EffectParam_t
    {
        kParamGain
    };

private:
    
protected:
    
    bool m_bIsInitialized;
    EffectType_t m_eEffectType;
    float m_fSampleRateInHz;
    int m_iNumChannels;
    
};

#endif /* AudioEffect_h */
