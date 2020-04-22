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

/*! \brief audio effect base class
*/
class CAudioEffect
{
public:
    CAudioEffect();
    ~CAudioEffect();
          
    /*! list of parameters used in audio effects */
    enum EffectParam_t
    {
        kParamGain,
        kParamPan,
        kParamDelayInSecs,

        kParamFeedback,
        kParamModRateInHz,
        kParamModWidthInSecs,
        kParamTremoloAmount,
        
        kParamCenterFrequency,
        kParamQ,
        
        kParamThreshold,
        kParamSlope,
        kParamDryWetMix,
        
        kParamNumFilters,
        kParamFilterGains,
        kParamFilterDelays,
        
    };
    
    /*! return the value of audio effect type
    \return Effect_t
    */
    Effect_t getEffectType();

private:
    
protected:
    
    bool m_bIsInitialized;  //!<  true if initialized
    Effect_t m_eEffectType;  //!<  audio effect type
    float m_fSampleRateInHz;  //!<  sample rate in Hz
    int m_iNumChannels;  //!<  number of channels
    
};

#endif /* AudioEffect_h */
