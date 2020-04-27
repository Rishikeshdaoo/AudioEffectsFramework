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
    kNone,  //!< none
    kGain,  //!< gain
    kPan,  //!< mono to stereo panning, steteo level adjustment
    kDistortion,  //!< distortion effect
    kCompressorExpander,  //!< compressor, expander
    kDelay,  //!< basic delay, flanger, chorus, vibrato, tremolo
    kReverb,  //!< schroeder reverb
    kBiquad,  //!< biquad filters
};
