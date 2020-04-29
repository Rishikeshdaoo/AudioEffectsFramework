//
// Created by Rishikesh Daoo on 2/24/20.
// Copyright © 2020 Rishikesh Daoo. All rights reserved.
//

#ifndef AUDIOFXFRAMEWORK_AUDIOEFFECTCOMPRESSOREXPANDER_H
#define AUDIOFXFRAMEWORK_AUDIOEFFECTCOMPRESSOREXPANDER_H

#include <AudioEffect.h>
#include "RingBuffer.h"

/*! \brief audio effect class for compression and expander
*/
class CAudioEffectCompressorExpander: public CAudioEffect
{
public:

    /*! list of sub types for compression and expander */
    enum EffectSubtype_t
    {
        kNone = 0,  //!< none
        kCompressor = 1,  //!< compressor
        kExpander = 2  //!< expander
    };

    CAudioEffectCompressorExpander();
    CAudioEffectCompressorExpander(Effect_t effectType, float fSampleRateInHz, int iNumChannels, EffectSubtype_t subType = kCompressor, int iLookaheadBufferSize = 150, EffectParam_t params[] = NULL, float values[] = NULL, int iNumParams = 0);
    ~CAudioEffectCompressorExpander();

    /*! initializes a distortion instance
    \param effectType audio effect type
    \param subType audio effect sub type
    \param fSampleRateInHz sample rate in Hz
    \param iNumChannels number of audio channels
    \param iLookaheadBufferSize look ahead buffer size in samples
    \param params[] list of parameter types
    \param values[] list of parameter values
    \param iNumParams number of parameters
    \return Error_t
    */
    Error_t init(Effect_t effectType, float fSampleRateInHz, int iNumChannels, EffectSubtype_t subType = kCompressor, int iLookaheadBufferSize = 150, EffectParam_t params[] = NULL, float values[] = NULL, int iNumParams = 0);

    /*! resets the internal variables (requires new call of init)
    \return Error_t
    */
    Error_t reset();

    /*! sets a compression/expander parameter
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

    /*! sets a compression/expander sub type
    \param eValue value of the sub type
    \return Error_t
    */
    Error_t setEffectSubtype(EffectSubtype_t eValue);
    
    /*! return the value of the specified sub type
    \return EffectSubtype_t
    */
    EffectSubtype_t getEffectSubtype();

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

    EffectSubtype_t m_eCompressorType;  //!< compressor/expander sub type

    int m_iLookaheadBufferSize;  //!< look ahead buffer size in sample

    float m_fAveragingTime;  //!< averaging time for RMS measurement in seconds, default=0.01
    float m_fAttackTime;  //!< compressor/expander attack time in seconds, default=0.03
    float m_fReleaseTime;  //!< compressor/expander release time in seconds, default=0.003

    float m_fThreshold;  //!< compressor/expander threshold in dB
    float m_fSlope;  //!< compressor/expander slope, 0<compressor slope<1, expander slope<0

    float* m_pfRmsSignal;  //!< RMS of signal in dB
    float* m_pf_gain;  //!< calculated linear gain of signal

    CRingBuffer<float> **m_ppfDelayBuffer;  //!< delay ring buffer
};

#endif //AUDIOFXFRAMEWORK_AUDIOEFFECTCOMPRESSOREXPANDER_H



