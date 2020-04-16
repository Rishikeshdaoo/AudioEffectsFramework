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
#include <EffectDef.h>


class CAudioEffect
{
public:
    CAudioEffect();
    ~CAudioEffect();
            
    enum EffectParam_t
    {
        kParamGain,
        kParamPan,
        kParamDelayInSecs,

        kParamModRateInHz,
        kParamModWidthInSecs,
        
        kParamCenterFrequency,
        kParamQ,
        
        kParamThreshold,
        kParamSlope,
        kParamDryWetMix,
        
        kParamNumFilters,
        kParamFilterGains,
        
    };
    
    Effect_t getEffectType();

private:
    
protected:
    
    bool m_bIsInitialized;
    Effect_t m_eEffectType;
    float m_fSampleRateInHz;
    int m_iNumChannels;
    
};

#endif /* AudioEffect_h */
