## Audio Effects Framework

Cross platform C++ library with low-latency audio effects processing algorithms designed for real-time audio processing.

## Motivation

This projects aims to provide an easy to use API for creating audio effects processing applications. With well documented, low-level implementation of audio effects in C++, the library is designed to be simple and efficient.

## Effects included in the library

- Gain
- Pan
- Delay
- Biquad
- Distortion
- Compressor
- Expander
- Flanger
- Chorus
- Reverb

## Use Cases

- Creating plugins for Digital Audio Workstations.
- For use in applications that require real-time audio effects.


## Getting Started

### Instructions

- Clone the repository with the command : 'git clone https://github.com/Rishikeshdaoo/AudioEffectsFramework.git'

### Usage

- Create the Effect instance
... '''CAudioEffectDelay       *phAudioEffectDelay = nullptr
	phAudioEffectDelay = new CAudioEffectDelay(sampleRate,numChannels,maxDelayInSecs,param,value,iNumParams)'''

.... 'sampleRate, numChannels' are arguments that you should get from your host application.
.... 'param, value, iNumParams' are arguments that need to be initialized as below
...... '''  int iNumParams = 4;
    		CAudioEffect::EffectParam_t param[iNumParams];
        	float value[iNumParams];

        	    param[0] = CAudioEffect::kParamDelayInSecs;
			    value[0] = 0.002f;
			    param[1] = CAudioEffect::kParamModRateInHz;
			    value[1] = 0.5f;
			    param[2] = CAudioEffect::kParamModWidthInSecs;
			    value[2] = 0.002f;
			    param[3] = CAudioEffect::kParamGain;
			    value[3] = 0.7f; '''
