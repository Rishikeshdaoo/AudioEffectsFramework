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
        kParamGain, //!< Parameter for gain of output audio
        kParamPan, //!< Parameter for pan value of output audio
        kParamDelayInSecs, //!< Parameter for delay used in delay-based effects

        kParamFeedback, //!< Parameter for feedback of output audio
        kParamModRateInHz, //!< Parameter for Modulation Frequency used in modulated effects
        kParamModWidthInSecs, //!< Parameter Modulation Amplitude used in modulated effects
        kParamTremoloAmount, //!< Parameter for amount of Tremolo

        kParamCenterFrequency, //!< Parameter for Center Frequency of a filter
        kParamQ, //!< Parameter for Q value of a filter

        kParamThreshold, //!< Parameter for threshold of a Compressor/Expander
        kParamSlope, //!< Parameter for slope of a Compressor/Expander
        kParamDryWetMix, //!< Parameter for amount of dry/wet mix in output audio

        kParamNumFilters, //!< Parameter for number of filters
        kParamFilterGains, //!< Parameter for number of gains
        kParamFilterDelays, //!< Parameter for number of delay lines
        
    };
    
    /*! return the value of audio effect type
    \return Effect_t
    */
    Effect_t getEffectType();
    
    
    /*! return the total delay in samples
    \return float
     */
    float getTailLength();

private:
    
protected:
    
    bool m_bIsInitialized;  //!<  true if initialized
    Effect_t m_eEffectType;  //!<  audio effect type
    float m_fSampleRateInHz;  //!<  sample rate in Hz
    int m_iNumChannels;  //!<  number of channels
    
};

#endif /* AudioEffect_h */
