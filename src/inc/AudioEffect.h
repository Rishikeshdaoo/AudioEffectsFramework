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
        /*! Parameter for gain of output audio */
        kParamGain,
        /*! Parameter for pan value of output audio */
        kParamPan,
        /*! Parameter for delay used in delay-based effects */
        kParamDelayInSecs,

        /*! Parameter for feedback of output audio */
        kParamFeedback,
        /*! Parameter for Modulation Frequency used in modulated effects  */
        kParamModRateInHz,
        /*! Parameter Modulation Amplitude used in modulated effects */
        kParamModWidthInSecs,
        /*! Parameter for amount of Tremolo */
        kParamTremoloAmount,

        /*! Parameter for Center Frequency of a filter */
        kParamCenterFrequency,
        /*! Parameter for Q value of a filter */
        kParamQ,

        /*! Parameter for threshold of a Compressor/Expander */
        kParamThreshold,
        /*! Parameter for slope of a Compressor/Expander */
        kParamSlope,
        /*! Parameter for amount of dry/wet mix in output audio */
        kParamDryWetMix,

        /*! Parameter for number of filters */
        kParamNumFilters,
        /*! Parameter for number of gains */
        kParamFilterGains,
        /*! Parameter for number of delay lines */
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
