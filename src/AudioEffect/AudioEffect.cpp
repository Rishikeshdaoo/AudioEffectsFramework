//
//  AudioEffect.cpp
//  AudioEffectsFramework
//
//  Created by Jason Smith on 1/31/20.
//  Copyright © 2020 Jason Smith. All rights reserved.
//

#include "AudioEffect.h"
#include <iostream>

using namespace std;

CAudioEffect::CAudioEffect()
{
    m_eEffectType = kNone;
    m_bIsInitialized = false;
};

CAudioEffect::~CAudioEffect()
{

};

Effect_t CAudioEffect::getEffectType()
{
    return m_eEffectType;
}

//Error_t CAudioEffect::init(Effect_t eEffectType, EffectParam_t params[], int iNumParams, float fSampleRateInHz, int iNumChannels)
//{
//    std::cout << "Init Intern from Base Class." << std::endl;
//    return kNoError;
//};
//
//Error_t CAudioEffect::reset()
//{
//    return kNoError;
//};
//
//Error_t CAudioEffect::setParam(EffectParam_t eParam, float fValue)
//{
//    return kNoError;
//};
//
//float CAudioEffect::getParam(EffectParam_t eParam)
//{
//    return kNoError;
//};
//
//Error_t CAudioEffect::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
//{
//    std::cout << "Process Base Class" << std::endl;
//    return kNoError;
//};
