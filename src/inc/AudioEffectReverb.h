//
// Created by Jason Smith on 3/19/20.
//  Copyright © 2020 Jason Smith. All rights reserved.
//

#ifndef AUDIOFXFRAMEWORK_AUDIOEFFECTREVERB_H
#define AUDIOFXFRAMEWORK_AUDIOEFFECTREVERB_H


#include "AudioEffect.h"
#include "AudioEffectBiquad.h"

#include "RingBuffer.h"

#include <stdio.h>
#include <iostream>

class CAudioEffectReverb: public CAudioEffect
{
public:
    
    CAudioEffectReverb();
    CAudioEffectReverb(float fSampleRateInHz, int iNumChannels, float fMaxDelayInSec, EffectParam_t params[], float values[], int iNumParams);
    ~CAudioEffectReverb();

    Error_t init(float fSampleRateInHz, int iNumChannels, float fMaxDelayInSec, EffectParam_t params[], float values[], int iNumParams);
    Error_t reset();

    Error_t setParam(EffectParam_t eParam, float fValue);
    float getParam(EffectParam_t eParam);
    
    Error_t setFilterDelays(float fValues[], int iNumFilters);

    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);

private:
    
    CAudioEffectBiquad **m_ppCAudioEffectBiquad;
    
    int m_iNumFilters;
    float m_fFilterGain;
    float * m_afFilterDelaysInSec;
    
};


#endif //AUDIOFXFRAMEWORK_AUDIOEFFECTREVERB_H
