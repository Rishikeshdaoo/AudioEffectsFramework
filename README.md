## Audio Effects Framework

Cross platform C++ library with low-latency audio effects processing algorithms designed for real-time audio processing. 

The Doxygen documentation can be accessed here: http://rishikeshdaoo.github.io/AudioEffectsFramework

## Motivation

This project aims to provide an easy to use API for creating audio effects processing applications. With well documented, low-level implementation of audio effects in C++, the library is designed to be simple and efficient. The effects use a process function designed for variable-length input blocks, making them suitable for real-time plugin applications such as JUCE.

We designed this library for users of all levels of experience. To this end, the effects in this framework contain commonly used default values for effect parameters and receive variable-length lists of parameters and values when initialized. This means that they can be used simply by instantiating the audio effect and running its process function and that more advanced users can edit any set of parameters they choose.

We tested these effect implementations against reference MATLAB code from http://dafx.de/DAFX_Book_Page/index.html, as well as C++ unit testing.

This repository also contains an executable .cpp file. A user can enter an input and output file path and effect type/subtype in order to generate an instance of that effect on an audio file, using default parameters.


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


## Getting Started

### Instructions

- Clone the repository with the command : `git clone https://github.com/Rishikeshdaoo/AudioEffectsFramework.git` or download a zip project folder from the Download button.
- Install <a href="https://cmake.org/">CMake</a> and compile the project.

### Effect Subtype Categorization

- Some of the effects have an hierarchical structure, where a class hold several effect subtypes. To use a specific effect subtype, the correct subtype enum should be passed to in the constructor/init function. The below diagram explains the categorization and the enum corresponding to the subtypes.

![Effect Subtype Categories](../assets/EffectSubtypes.png)

### Usage

<!-- - <span style="font-family:Arial; font-size:1.5em;">Create the Effect instance </span> -->
#### Create the Effect Instance

<span style="font-family:Courier; text-align:center; font-size:1.0em">CAudioEffectDelay       *phAudioEffectDelay = nullptr; <br />
phAudioEffectDelay = new CAudioEffectDelay(fSampleRateInHz, iNumChannels, subType, fMaxDelayInSec, params[] , values[], iNumParams); <br /></span>
- <i>fSampleRateInHz, iNumChannels</i> - Required arguments that should be taken from the host application. Both are required arguments <br />
- <i> subType</i> - Required argument for the effect subtype the user wishes to use. Exists only for effects that have subtypes. Refer above section on effect subtype categorization.
- <i>fMaxDelayInSec</i> - Required argument the user sets according to their requirement for parameter value range (Only delay based effects have this argument). <br />
- <i>params, values, iNumParams</i> - Optional arguments that need to be initialized as below. These are the values of the effect parameters. These are optional arguments, which will take default values if they are not passed by the user.

<span style="font-family:Courier; text-align:center; font-size:1.0em; text-align:center">int iNumParams = 5; <br />
CAudioEffect::EffectParam_t param[iNumParams]; <br />
float value[iNumParams]; </span>

param[0] = CAudioEffect::kParamDelayInSecs; <br />
value[0] = 0.002f; <br />
param[1] = CAudioEffect::kParamModRateInHz; <br />
value[1] = 0.5f; <br />
param[2] = CAudioEffect::kParamModWidthInSecs; <br />
value[2] = 0.002f; <br />
param[3] = CAudioEffect::kParamDryWetMix; <br />
value[3] = 0.7f; <br /> 
param[4] = CAudioEffect::kParamFeedback; <br />
value[4] = 0.5f; <br />

<!-- - <span style="font-family:Arial; font-size:1.5em;"> Call the effect's process function </span> -->
#### Call the effect's process function
    
<span style="font-family:Courier; text-align:center; font-size:1.0em; text-align:center">Error_t error = phAudioEffectDelay -> process(ppfAudioInput, ppfAudioOutput, iNumFrames); <br /> </span>
- <i>ppfAudioInput, ppfAudioOutput</i> - Input/output buffers used by the library. The user should take care of creating and destroying these buffers. <br />
- <i>iNumFrames</i> - Argument that tells the process function the number of samples in a block of buffered audio.

#### Video demonstration of the framework

[![AudioFxFramework_Demo](http://img.youtube.com/vi/NgGLyauAIhE/0.jpg)](http://www.youtube.com/watch?v=NgGLyauAIhE "Audio Effects Framework - Video demonstration")
