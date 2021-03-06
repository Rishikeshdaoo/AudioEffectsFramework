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

/*! \brief audio effect class for reverb
*/
class CAudioEffectReverb: public CAudioEffect
{
public:
    
    CAudioEffectReverb();
    CAudioEffectReverb(float fSampleRateInHz, int iNumChannels, float fMaxDelayInSec = 5.f, EffectParam_t params[] = NULL, float values[] = NULL, int iNumParams = 0, float filterDelaysInSec[] = NULL);
    ~CAudioEffectReverb();

    /*! initializes a reverb instance
    \param fSampleRateInHz sample rate in Hz
    \param iNumChannels number of audio channels
    \param fMaxDelayInSec max delay length in seconds
    \param params[] list of parameter types
    \param values[] list of parameter values
    \param iNumParams number of parameters
    \param filterDelaysInSec[] list of filter delay values
    \return Error_t
    */
    Error_t init(float fSampleRateInHz, int iNumChannels, float fMaxDelayInSec = 5.f, EffectParam_t params[] = NULL, float values[] = NULL, int iNumParams = 0, float filterDelaysInSec[] = NULL);
    
    /*! resets the internal variables (requires new call of init)
    \return Error_t
    */
    Error_t reset();

    /*! sets a reverb parameter
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
    
    /*! sets delay lengths for filters of reverb
    \param eValue value of delay lengths for each filter in reverb
    \return Error_t
    */
    Error_t setFilterDelays(float fValues[], int iNumFilters);
    
    /*! return the total delay in samples
    \return float
     */
    float getTailLength();

    /*! processes one block of audio
    \param ppfInputBuffer input buffer [numChannels][iNumberOfFrames]
    \param ppfOutputBuffer output buffer [numChannels][iNumberOfFrames]
    \param iNumberOfFrames buffer length (per channel)
    \return Error_t
    */
    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);

private:
    
    CAudioEffectBiquad **m_ppCAudioEffectBiquad;  //!< audio effect biquad instances
    
    int m_iNumFilters;  //!<  number of filters
    float m_fFilterGain;  //!<  filter gain
    float * m_afFilterDelaysInSec;  //!<  filter delay lengths in seconds
    float m_fGain;  //!< scaling gain for direct signal
    
};


#endif //AUDIOFXFRAMEWORK_AUDIOEFFECTREVERB_H
