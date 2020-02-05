//
//  AudioEffectIF.h
//  AudioEffectsFramework
//
//  Created by Jason Smith on 1/31/20.
//  Copyright © 2020 Jason Smith. All rights reserved.
//

#ifndef AudioEffectIF_h
#define AudioEffectIF_h

#include <stdio.h>
#include <ErrorDef.h>

class CAudioEffectIF
{
public:
    
    enum EffectType_t
    {
        kEffectGain,
        kEffectPan
    };
    
    enum EffectParam_t
    {
      kParamGain
    };
    
    static Error_t create(CAudioEffectIF *& pCAudioEffect);
    static Error_t destroy(CAudioEffectIF *& pCAudioEffect);
    Error_t init(EffectType_t eEffectType, EffectParam_t params[], int iNumParams, float fSampleRateInHz, int iNumChannels);
    Error_t reset();
    Error_t setParam(EffectType_t eParam, float fValue);
    float getParam(EffectType_t eParam);
    
    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
    
protected:
    CAudioEffectIF();
    virtual ~CAudioEffectIF();
    
private:
    virtual Error_t initIntern(EffectType_t eEffectType, EffectParam_t params[], int iNumParams, float fSampleRateInHz, int iNumChannels) = 0;
    virtual Error_t resetIntern() = 0;

    virtual Error_t setParamIntern(EffectType_t eParam, float fValue) = 0;
    virtual float getParamIntern(EffectType_t eParam) const = 0;

    virtual Error_t processIntern(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames) = 0;

    bool m_bIsInitialized;
    CAudioEffectIF *m_pCAudioEffect;

    float m_fSampleRate;
    
};

#endif /* AudioEffectIF_h */
