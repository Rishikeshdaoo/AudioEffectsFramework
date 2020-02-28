//
// Created by Rishikesh Daoo on 2/24/20.
// Copyright © 2020 Rishikesh Daoo. All rights reserved.
//

#ifndef AUDIOFXFRAMEWORK_AUDIOEFFECTCOMPRESSOR_H
#define AUDIOFXFRAMEWORK_AUDIOEFFECTCOMPRESSOR_H

#include <AudioEffect.h>
#include "RingBuffer.h"

class CAudioEffectCompressor: public CAudioEffect
{
public:

    CAudioEffectCompressor();
    CAudioEffectCompressor(float fSampleRateInHz, int iNumChannels, int iMaxDelayInSec, EffectParam_t params[], float values[], int iNumParams);
    ~CAudioEffectCompressor();

    Error_t init(float fSampleRateInHz, int iNumChannels, int iMaxDelayInSec, EffectParam_t params[], float values[], int iNumParams);
    Error_t reset();

    Error_t setParam(EffectParam_t eParam, float fValue);
    float getParam(EffectParam_t eParam);

    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);

private:

    const float m_fTav;
    const float m_fAt;
    const float m_fRt;

    float m_fCompressorThreshold;
    float m_fCompressorSlope;
    float m_fExpanderThreshold;
    float m_fExpanderSlope;
    int m_iMaxDelay;

    CRingBuffer<float> **m_ppfDelayBuffer;
};

#endif //AUDIOFXFRAMEWORK_AUDIOEFFECTCOMPRESSOR_H



