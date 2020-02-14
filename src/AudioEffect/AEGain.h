//
// Created by Rishikesh on 2/13/20.
//

#include <ErrorDef.h>

#ifndef AUDIOFXFRAMEWORK_AEGAIN_H
#define AUDIOFXFRAMEWORK_AEGAIN_H

#endif //AUDIOFXFRAMEWORK_AEGAIN_H

class CAEGain{

public:
    CAEGain();
    ~CAEGain();

    enum ParamType_t
    {
        kGain
    };

    static Error_t create(CAEGain*& pCAEGain);
    static Error_t destroy(CAEGain*& pCAEGain);

    Error_t init(float gain, float fSampleRateInHz, int iNumOfChannels);
    Error_t reset();

    Error_t setParam(ParamType_t eParam, float paramValue);
    float getParam(ParamType_t eParam) const;

    Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);

private:
    bool m_bIsInitialized;
    CAEGain *m_pCAEGain;

    int m_iNumOfChannels;
    float m_fSampleRate;
    float m_gain;
};