//
//  AudioEffectGain.cpp
//  AudioEffectsFramework
//
//  Created by Jason Smith on 2/05/20.
//  Copyright © 2020 Jason Smith. All rights reserved.
//

#ifndef AudioEffectGain_h
#define AudioEffectGain_h

#include "AudioEffect.h"

#include <stdio.h>
#include <iostream>

/*! \brief audio effect class for gain
*/
class CAudioEffectGain: public CAudioEffect
{
public:
    CAudioEffectGain();
    CAudioEffectGain(float fSampleRateInHz, int iNumChannels, EffectParam_t params[] = NULL, float values[] = NULL, int iNumParams = 0);
    ~CAudioEffectGain();
    
    /*! initializes a gain instance
    \param fSampleRateInHz sample rate in Hz
    \param iNumChannels number of audio channels
    \param params[] list of parameter types
    \param values[] list of parameter values
    \param iNumParams number of parameters
    \return Error_t
    */
    Error_t init(float fSampleRateInHz, int iNumChannels, EffectParam_t params[] = NULL, float values[] = NULL, int iNumParams = 0);
    
    /*! resets the internal variables (requires new call of init)
    \return Error_t
    */
    Error_t reset();
    
    /*! sets a gain parameter
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
    
    float m_fGain;  //!<  amount of gain, 0<gain<1
    
};

#endif /* AudioEffectGain_h */
