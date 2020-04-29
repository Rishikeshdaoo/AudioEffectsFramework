## Audio Effects Framework

Cross platform C++ library with low-latency audio effects processing algorithms designed for real-time audio processing. 

The Doxygen documentation can be accessed here: http://rishikeshdaoo.github.io/AudioEffectsFramework

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
- Tremolo
- Vibrato

## Use Cases

- Creating plugins for Digital Audio Workstations.
- For use in applications that require real-time audio effects.

### Effect Subtype Categorization

- Some of the effects have an hierarchical structure, where a class hold several effect subtypes. To use a specific effect subtype, the correct subtype enum should be passed to in the constructor/init function. The below diagram explains the categorization and the enum corresponding to the subtypes.

![Effect Subtype Categories](../assets/EffectSubtypes.png)


## Getting Started

### Instructions

- Clone the repository with the command : `git clone https://github.com/Rishikeshdaoo/AudioEffectsFramework.git` or download a zip project folder from the Download button.
- Install <a href="https://cmake.org/">CMake</a> and compile the project.

### Usage

<!-- - <span style="font-family:Arial; font-size:1.5em;">Create the Effect instance </span> -->
#### Create the Effect Instance

CAudioEffectDelay       *phAudioEffectDelay = nullptr; <br />
phAudioEffectDelay = new CAudioEffectDelay(sampleRate,numChannels,maxDelayInSecs,param,value,iNumParams); <br />
- <i>sampleRate, numChannels</i> are arguments that should be taken from the host application. <br />
- <i> maxDelayInSecs</i> is an argument the user sets according to their requirement for parameter value range (Only delay based effects have this argument). <br />
- <i>param, value, iNumParams</i> are arguments that need to be initialized as below.

int iNumParams = 4; <br />
CAudioEffect::EffectParam_t param[iNumParams]; <br />
float value[iNumParams]; 

param[0] = CAudioEffect::kParamDelayInSecs; <br />
value[0] = 0.002f; <br />
param[1] = CAudioEffect::kParamModRateInHz; <br />
value[1] = 0.5f; <br />
param[2] = CAudioEffect::kParamModWidthInSecs; <br />
value[2] = 0.002f; <br />
param[3] = CAudioEffect::kParamGain; <br />
value[3] = 0.7f; 


<!-- - <span style="font-family:Arial; font-size:1.5em;"> Call the effect's process function </span> -->
#### Call the effect's process function
    
Error_t error = phAudioEffectDelay -> process(ppfAudioInput, ppfAudioOutput, iNumFrames); <br />
- <i>ppfAudioInput, ppfAudioOutput</i> are the input/output buffers used by the library. The user should take care of creating and destroying these buffers. <br />
- <i>iNumFrames</i> is an argument that tells the process function the number of samples in a block of buffered audio.


