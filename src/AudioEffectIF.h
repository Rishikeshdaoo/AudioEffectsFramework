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

class CAudioEffect;

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
    
    void create();
    void destroy();
    void init(EffectType_t eEffectType, EffectParam_t params[], int iNumParams, float fSampleRateInHz, int iNumChannels);
    void reset();
    void setParam(EffectType_t eParam, float fValue);
    void getParam(EffectType_t eParam);
    
    void process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
    
protected:
    CAudioEffectIF();
    virtual ~CAudioEffectIF();
    
private:
    bool m_bIsInitialized;
    
    CAudioEffect *m_pCAudioEffect;
    
    float m_fSampleRate;
    
};

#endif /* AudioEffectIF_h */
