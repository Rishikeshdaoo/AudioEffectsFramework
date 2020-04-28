//
// Created by Rishikesh Daoo on 2/13/20.
// Copyright © 2020 Rishikesh Daoo. All rights reserved.
//

#ifndef AUDIOFXFRAMEWORK_EFFECTTYPE_H
#define AUDIOFXFRAMEWORK_EFFECTTYPE_H

#endif //AUDIOFXFRAMEWORK_EFFECTTYPE_H

/*! list of audio effects */
enum Effect_t
{
    kNone,  //!< No effect
    kGain,  //!< Gain
    kPan,  //!< Mono to Stereo panning, steteo level adjustment
    kDistortion,  //!< Distortion effect
    kCompressorExpander,  //!< Compressor, Expander
    kDelay,  //!< Basic delay, Flanger, Chorus, Vibrato, Tremolo
    kReverb,  //!< Schroeder Reverb
    kBiquad,  //!< Biquad filters
};
