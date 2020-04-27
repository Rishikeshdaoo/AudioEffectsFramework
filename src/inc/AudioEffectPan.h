#ifndef AudioEffectPan_h
#define AudioEffectPan_h

#include "AudioEffect.h"

#include <stdio.h>
#include <iostream>

/*! \brief audio effect class for pan
*/
class CAudioEffectPan: public CAudioEffect
{
public:
    CAudioEffectPan();
    CAudioEffectPan(float fSampleRateInHz, int iNumChannels, EffectParam_t params[] = NULL, float values[] = NULL, int iNumParams = 0);
    ~CAudioEffectPan();
    
    /*! initializes a pan instance
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
    
    float m_fPan;  //!<  amount of pan, left=-1, right=1, center=0
    
};

#endif /* AudioEffectPan_h */
