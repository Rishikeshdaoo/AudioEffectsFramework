//
// Created by Jason Smith on 2/20/20.
//  Copyright © 2020 Jason Smith. All rights reserved.
//

#ifndef AUDIOFXFRAMEWORK_AudioEffectBiquad_H
#define AUDIOFXFRAMEWORK_AudioEffectBiquad_H


#include "AudioEffect.h"

#include <stdio.h>
#include <iostream>

class CAudioEffectBiquad: public CAudioEffect
{
public:
    
    enum FilterType_t
    {
      kLowpass,
      kHighpass,
      kBandpass,
      kAllpass,
      kPeak,
      kNotch,
      kLowshelf,
      kHighshelf
    };
    
    CAudioEffectBiquad();
    CAudioEffectBiquad(float fSampleRateInHz, int iNumChannels, EffectParam_t params[], float values[], int iNumParams);
    ~CAudioEffectBiquad();

    Error_t init(float fSampleRateInHz, int iNumChannels, EffectParam_t params[], float values[], int iNumParams);
    Error_t reset();

    Error_t setParam(EffectParam_t eParam, float fValue);
    float getParam(EffectParam_t eParam);
    
    Error_t setFilterType(FilterType_t eValue);
    FilterType_t getFilterType();

    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);

private:
    
    Error_t setFilterConstants();
    
    float m_fCenterFrequencyInHz;
    float m_fQ;
    float m_fGain;
    
    FilterType_t m_eFilterType;
    float m_fa0;
    float m_fa1;
    float m_fa2;
    float m_fb0;
    float m_fb1;
    float m_fb2;
    
    float * m_fxn1;
    float * m_fxn2;
    float * m_fyn1;
    float * m_fyn2;
    
};


#endif //AUDIOFXFRAMEWORK_AudioEffectBiquad_H
