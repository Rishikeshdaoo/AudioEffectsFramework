//
// Created by Rishikesh Daoo on 2/15/20.
//

#ifndef AUDIOFXFRAMEWORK_AUDIOEFFECTDELAY_H
#define AUDIOFXFRAMEWORK_AUDIOEFFECTDELAY_H


#include "AudioEffect.h"

#include <stdio.h>
#include <iostream>

class CAudioEffectDelay: public CAudioEffect
{
public:
    CAudioEffectDelay();
    CAudioEffectDelay(EffectParam_t params[], float values[], int iNumParams, float fSampleRateInHz, int iNumChannels);
    ~CAudioEffectDelay();

    Error_t init(EffectParam_t params[], float values[], int iNumParams, float fSampleRateInHz, int iNumChannels);
    Error_t reset();

    Error_t setParam(EffectParam_t eParam, float fValue);
    float getParam(EffectParam_t eParam);

    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);

protected:

    float m_fGain;
    float m_fDelayInSec;

};


#endif //AUDIOFXFRAMEWORK_AUDIOEFFECTDELAY_H
