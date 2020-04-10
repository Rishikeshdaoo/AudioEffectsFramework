//
// Created by Rishikesh Daoo on 2/24/20.
// Copyright © 2020 Rishikesh Daoo. All rights reserved.
//

#ifndef AUDIOFXFRAMEWORK_AUDIOEFFECTCOMPRESSOREXPANDER_H
#define AUDIOFXFRAMEWORK_AUDIOEFFECTCOMPRESSOREXPANDER_H

#include <AudioEffect.h>
#include "RingBuffer.h"

class CAudioEffectCompressorExpander: public CAudioEffect
{
public:

    CAudioEffectCompressorExpander();
    CAudioEffectCompressorExpander(Effect_t effectType, float fSampleRateInHz, int iNumChannels, int iMaxDelayInSec, EffectParam_t params[], float values[], int iNumParams);
    ~CAudioEffectCompressorExpander();

    Error_t init(Effect_t effectType, float fSampleRateInHz, int iNumChannels, EffectParam_t params[], float values[], int iNumParams);
    Error_t reset();

    Error_t setParam(EffectParam_t eParam, float fValue);
    float getParam(EffectParam_t eParam);

    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);

private:

    float m_fAveragingTime;
    float m_fAttackTime;
    float m_fReleaseTime;

    float m_fThreshold;
    float m_fSlope;

    float* m_pfRmsSignal;
    float* m_pf_gain;

    CRingBuffer<float> **m_ppfDelayBuffer;
};

#endif //AUDIOFXFRAMEWORK_AUDIOEFFECTCOMPRESSOREXPANDER_H



