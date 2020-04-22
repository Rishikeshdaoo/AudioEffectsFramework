//
// Created by Rishikesh Daoo on 2/15/20.
//  Copyright © 2020 Rishikesh Daoo. All rights reserved.
//

#ifndef AUDIOFXFRAMEWORK_AUDIOEFFECTDELAY_H
#define AUDIOFXFRAMEWORK_AUDIOEFFECTDELAY_H


#include "AudioEffect.h"
#include "RingBuffer.h"
#include "Lfo.h"

#include <stdio.h>
#include <iostream>

/*! \brief audio effect class for delay related effects
*/
class CAudioEffectDelay: public CAudioEffect
{
public:
    
    /*! list of delay related effect sub types */
    enum DelayType_t
    {
//        kNone = 0,
        kDelay = 16,
        kChorus = 18,
        kFlanger = 21,
        kTremolo = 8,
        kVibrato = 2
    };
    
    CAudioEffectDelay();
    CAudioEffectDelay(float fSampleRateInHz, int iNumChannels, float iMaxDelayInSec, EffectParam_t params[], float values[], int iNumParams);
    ~CAudioEffectDelay();
    
    /*! initializes a delay related effect instance
    \param fSampleRateInHz sample rate in Hz
    \param iNumChannels number of audio channels
    \param iMaxDelayInSec max delay length in seconds
    \param params[] list of parameter types
    \param values[] list of parameter values
    \param iNumParams number of parameters
    \return Error_t
    */
    Error_t init(float fSampleRateInHz, int iNumChannels, float iMaxDelayInSec, EffectParam_t params[], float values[], int iNumParams);
    
    /*! resets the internal variables (requires new call of init)
    \return Error_t
    */
    Error_t reset();
    
    /*! sets a delay related effect parameter
    \param eParam what parameter
    \param fValue value of the parameter
    \return Error_t
    */
    Error_t setParam(EffectParam_t eParam, float fValue);
    
    /*! return the value of the specified parameter
    \param eParam
    \return float
    */
    float getParam(EffectParam_t eParam);
    
    /*! sets a delay related effect sub type
    \param eValue value of the sub type
    \return Error_t
    */
    Error_t setDelayType(DelayType_t eValue);
    
    /*! return the value of the specified sub type
    \return DelayType_t
    */
    DelayType_t getDelayType();
    
    /*! processes one block of audio
    \param ppfInputBuffer input buffer [numChannels][iNumberOfFrames]
    \param ppfOutputBuffer output buffer [numChannels][iNumberOfFrames]
    \param iNumberOfFrames buffer length (per channel)
    \return Error_t
    */
    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
    
private:
    
    CRingBuffer<float>  **m_ppCRingBuffer;  //!< audio ring buffer
    DelayType_t m_eDelayType;  //!< delay related effect sub type
    
    float m_fDryWetMix;  //!< mix of original and distorted sound, 1=only distorted, 0=only dry
    float m_fDelayInSamples;  //!< delay length in samples
    float m_fMaxDelayInSamples;  //!< max delay length in samples
    float m_fModWidthInSamples;  //!< modulation wdith in samples
    float m_fFeedback;  //!< feedback amount
    float m_fTremoloAmount;  //!< tremolo amount
    
    CLfo *m_pCLfo;  //!< instance of Lfo
};


#endif //AUDIOFXFRAMEWORK_AUDIOEFFECTDELAY_H
