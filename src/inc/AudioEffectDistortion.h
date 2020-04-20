//
//  AudioEffectDistortion.h
//  AudioEffectsFramework
//
//  Created by Snehesh Nag on 2/21/20.
//  Copyright © 2020 Snehesh Nag. All rights reserved.
//

#ifndef AudioEffectDistortion_h
#define AudioEffectDistortion_h

#include "AudioEffect.h"

#include <stdio.h>
#include <iostream>

/*! \brief audio effect class for distortion
*/
class CAudioEffectDistortion: public CAudioEffect
{
public:
    CAudioEffectDistortion();
    CAudioEffectDistortion(float fSampleRateInHz, int iNumChannels, EffectParam_t params[], float values[], int iNumParams);
    ~CAudioEffectDistortion();
    
    /*! initializes a distortion instance
    \param fSampleRateInHz sample rate in Hz
    \param iNumChannels number of audio channels
    \param params[] list of parameter types
    \param values[] list of parameter values
    \param iNumParams number of parameters
    \return Error_t
    */
    Error_t init(float fSampleRateInHz, int iNumChannels, EffectParam_t params[], float values[], int iNumParams);
    
    /*! resets the internal variables (requires new call of init)
    \return Error_t
    */
    Error_t reset();
    
    /*! sets a distortion parameter
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
    
    /*! processes one block of audio
    \param ppfInputBuffer input buffer [numChannels][iNumberOfFrames]
    \param ppfOutputBuffer output buffer [numChannels][iNumberOfFrames]
    \param iNumberOfFrames buffer length (per channel)
    \return Error_t
    */
    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
    
protected:
    
    float m_fGain;  //!<  amount of distortion, >0
    float m_fDryWetMix;  //!< mix of original and distorted sound, 1=only distorted, 0=only dry
    
};
#endif /* AudioEffectDistortion_h */
