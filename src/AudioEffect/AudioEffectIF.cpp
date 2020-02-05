//
//  AudioEffectIF.cpp
//  AudioEffectsFramework
//
//  Created by Jason Smith on 1/31/20.
//  Copyright © 2020 Jason Smith. All rights reserved.
//

#include "AudioEffectIF.h"
#include "AudioEffect.h"

static const char*  kCMyProjectBuildDate             = __DATE__;


CAudioEffectIF::CAudioEffectIF () :
        m_bIsInitialized(false),
        m_pCAudioEffect(0),
        m_fSampleRate(0)
{
    reset ();
}

CAudioEffectIF::~CAudioEffectIF(){
    reset ();
}

Error_t CAudioEffectIF::create(CAudioEffectIF *& pCAudioEffect){

    pCAudioEffect   = new CAudioEffect();

    if (!pCAudioEffect)
        return kMemError;

    return kNoError;
}

Error_t CAudioEffectIF::destroy (CAudioEffectIF *& pCAudioEffect){

    delete pCAudioEffect;
    pCAudioEffect = 0;

    return kNoError;
}

Error_t CAudioEffectIF::init(EffectType_t eEffectType, EffectParam_t params[], int iNumParams, float fSampleRateInHz, int iNumChannels)
{
    Error_t error = kNoError;
    if(!m_bIsInitialized){
        error = initIntern(eEffectType, params, iNumParams, fSampleRateInHz, iNumChannels);
    }
    if(error != kNoError){
        return kNotInitializedError;
    }else{
        m_bIsInitialized = true;
    }
    return error;
}


Error_t CAudioEffectIF::reset ()
{
    Error_t error = kNoError;
    if(m_bIsInitialized){
        error = resetIntern();
    }
    if(error != kNoError){
        return kNotInitializedError;
    } else{
        m_bIsInitialized = false;
    }

    return error;
}