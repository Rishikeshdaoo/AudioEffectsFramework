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

float CAudioEffect::getTailLength()
{
    return 0.f;
}
