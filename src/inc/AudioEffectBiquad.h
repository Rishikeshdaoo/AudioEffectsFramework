//
// Created by Jason Smith on 2/20/20.
//  Copyright © 2020 Jason Smith. All rights reserved.
//

#ifndef AUDIOFXFRAMEWORK_AudioEffectBiquad_H
#define AUDIOFXFRAMEWORK_AudioEffectBiquad_H


#include "AudioEffect.h"
#include "RingBuffer.h"

/*! \brief audio effect class for biquad filter
*/
class CAudioEffectBiquad: public CAudioEffect
{
public:
    
    /*! list of sub types for biquad filter */
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
    CAudioEffectBiquad(float fSampleRateInHz, int iNumChannels, FilterType_t filterType = kLowpass, EffectParam_t params[] = NULL, float values[] = NULL, int iNumParams = 0, float fMaxDelayInSec = 5.f);
    ~CAudioEffectBiquad();

    /*! initializes a distortion instance
    \param fSampleRateInHz sample rate in Hz
    \param iNumChannels number of audio channels
    \param params[] list of parameter types
    \param values[] list of parameter values
    \param iNumParams number of parameters
    \param fMaxDelayInSec max delay lenghth in seconds
    \return Error_t
    */
    Error_t init(float fSampleRateInHz, int iNumChannels, FilterType_t filterType = kLowpass, EffectParam_t params[] = NULL, float values[] = NULL, int iNumParams = 0, float fMaxDelayInSec = 5.f);
    
    /*! resets the internal variables (requires new call of init)
    \return Error_t
    */
    Error_t reset();

    /*! sets a biquad filter parameter
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
    
    /*! sets a biquad filter sub type
    \param eValue value of the sub type
    \return Error_t
    */
    Error_t setFilterType(FilterType_t eValue);
    
    /*! return the value of the specified sub type
    \return Filtertype_t
    */
    FilterType_t getFilterType();
    
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
    
    Error_t setFilterConstants();  
    
    float m_fCenterFrequencyInHz;  //!<  center frequency in Hz
    float m_fQ;  //!<  Q factor
    float m_fGain;  //!<  gain
    
    FilterType_t m_eFilterType;  //!<  biquad filter sub type
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
    
    CRingBuffer<float>  **m_ppCRingBuffer;  //!<  audio ring buffer
    float m_fMaxDelayInSamples;  //!<  max delay length in samples
    float m_fDelayInSamples;  //!<  delay length in samples
    
};


#endif //AUDIOFXFRAMEWORK_AudioEffectBiquad_H
