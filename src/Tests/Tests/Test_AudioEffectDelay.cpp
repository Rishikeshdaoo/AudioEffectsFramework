#include "AudioFxFrameworkConfig.h"

#ifdef WITH_TESTS
#include <cassert>
#include <cstdio>

#include "UnitTest++.h"

#include "Synthesis.h"
#include "Vector.h"
#include "AudioEffect.h"
#include "AudioEffectDelay.h"
#include <algorithm>

#include <stdio.h>

    
SUITE(Delay)
{
    struct DelayData
    {
        DelayData():
            m_phDelay(0),
            m_ppfInputData(0),
            m_ppfOutputData(0),
            m_iDataLength(35131),
            m_iBlockLength(171),
            m_iNumChannels(3),
            m_fSampleRate(8000)
        {
            m_ppfInputData  = new float*[m_iNumChannels];
            m_ppfOutputData = new float*[m_iNumChannels];
            m_ppfInputTmp   = new float*[m_iNumChannels];
            m_ppfOutputTmp  = new float*[m_iNumChannels];
            for (int i = 0; i < m_iNumChannels; i++)
            {
                m_ppfInputData[i]   = new float [m_iDataLength];
                CVectorFloat::setZero(m_ppfInputData[i], m_iDataLength);
                m_ppfOutputData[i]  = new float [m_iDataLength];
//                CVectorFloat::setZero(m_ppfOutputData[i], m_iDataLength);
                
            }
        }
        
        ~DelayData()
        {
            for (int i = 0; i < m_iNumChannels; i++)
            {
                delete [] m_ppfOutputData[i];
                delete [] m_ppfInputData[i];
            }
            delete [] m_ppfOutputTmp;
            delete [] m_ppfInputTmp;
            delete [] m_ppfOutputData;
            delete [] m_ppfInputData;

            //destroy?
            m_phDelay->reset();
            delete m_phDelay;
            m_phDelay = 0;
        }
        
        void TestProcess()
        {
            int iNumFramesRemaining = m_iDataLength;
            while (iNumFramesRemaining > 0)
            {
                int iNumFrames = std::min(iNumFramesRemaining, m_iBlockLength);

                for (int c = 0; c < m_iNumChannels; c++)
                {
                    m_ppfInputTmp[c]    = &m_ppfInputData[c][m_iDataLength - iNumFramesRemaining];
                    m_ppfOutputTmp[c]   = &m_ppfOutputData[c][m_iDataLength - iNumFramesRemaining];
                }
                m_phDelay->process(m_ppfInputTmp, m_ppfOutputTmp, iNumFrames);

                iNumFramesRemaining -= iNumFrames;
            }
        }
        
        void TestProcessInplace()
        {
            int iNumFramesRemaining = m_iDataLength;
            while (iNumFramesRemaining > 0)
            {
                int iNumFrames = std::min(iNumFramesRemaining, m_iBlockLength);

                for (int c = 0; c < m_iNumChannels; c++)
                {
                    m_ppfInputTmp[c]    = &m_ppfInputData[c][m_iDataLength - iNumFramesRemaining];
                }
                m_phDelay->process(m_ppfInputTmp, m_ppfInputTmp, iNumFrames);

                iNumFramesRemaining -= iNumFrames;
            }
        }
        
        CAudioEffectDelay    *m_phDelay = 0;
        float               **m_ppfInputData,
                            **m_ppfOutputData,
                            **m_ppfInputTmp,
                            **m_ppfOutputTmp;
        int                 m_iDataLength;
        int                 m_iBlockLength;
        int                 m_iNumChannels;
        float               m_fSampleRate;
        int                 m_iMaxDelayInSec = 1;
        float               m_fGain = 0;
        
    };
    
    // delay: zero input -> zero output (same output)
    TEST_FIXTURE(DelayData, ZeroInput)
    {
        int iNumParams = 5;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamDryWetMix;
        value[0] = 0.5f;
        param[1] = CAudioEffect::kParamDelayInSecs;
        value[1] = 0.3f;
        param[2] = CAudioEffect::kParamModRateInHz;
        value[2] = 0.2f;
        param[3] = CAudioEffect::kParamModWidthInSecs;
        value[3] = 0.01f;
        param[4] = CAudioEffect::kParamFeedback;
        value[4] = 0.f;

        m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels,1.F,CAudioEffectDelay::kBasicDelay,param,value,iNumParams);
        
        TestProcess();

        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

        m_phDelay->reset();
    }
    
    
    // delay: zero gain -> same output
    TEST_FIXTURE(DelayData, DelayZeroGain)
    {
        int iNumParams = 5;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamDryWetMix;
        value[0] = 0.f;
        param[1] = CAudioEffect::kParamDelayInSecs;
        value[1] = 0.3f;
        param[2] = CAudioEffect::kParamModRateInHz;
        value[2] = 0.2f;
        param[3] = CAudioEffect::kParamModWidthInSecs;
        value[3] = 0.01f;
        param[4] = CAudioEffect::kParamFeedback;
        value[4] = 0.f;

        m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels,1.F,CAudioEffectDelay::kBasicDelay,param,value,iNumParams);

        for (int c = 0; c < m_iNumChannels; c++)
        CSynthesis::generateSine (m_ppfInputData[c], 440, m_fSampleRate, m_iDataLength, .8F, static_cast<float>(c*M_PI_2));

        TestProcess();

        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

        m_phDelay->reset();
    }

    // delay: zero gain -> same output
    TEST_FIXTURE(DelayData, ZeroBasicDelay)
    {
        m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels,1.F,CAudioEffectDelay::kBasicDelay);

        TestProcess();

        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

        m_phDelay->reset();
    }
    
    // flanger: zero gain -> same output
    TEST_FIXTURE(DelayData, ZeroFlanger)
    {
        m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels,1.F,CAudioEffectDelay::kFlanger);

        TestProcess();

        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

        m_phDelay->reset();
    }
    
    // chorus: zero gain -> same output
    TEST_FIXTURE(DelayData, ZeroChorus)
    {
        m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels,1.F,CAudioEffectDelay::kChorus);

        TestProcess();

        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

        m_phDelay->reset();
    }
    
    // vibrato: zero gain -> same output
    TEST_FIXTURE(DelayData, ZeroVibrato)
    {
        m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels,1.F,CAudioEffectDelay::kVibrato);

        TestProcess();

        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

        m_phDelay->reset();
    }
    
    // tremolo: zero gain -> same output
    TEST_FIXTURE(DelayData, ZeroTremolo)
    {
        m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels,1.F,CAudioEffectDelay::kTremolo);

        TestProcess();

        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

        m_phDelay->reset();
    }
    
    

    // delay: check delay length
    TEST_FIXTURE(DelayData, CheckDelayLength)
    {
        int iNumParams = 5;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamDryWetMix;
        value[0] = 0.3f;
        param[1] = CAudioEffect::kParamDelayInSecs;
        value[1] = 0.3f;
        param[2] = CAudioEffect::kParamModRateInHz;
        value[2] = 0.2f;
        param[3] = CAudioEffect::kParamModWidthInSecs;
        value[3] = 0.01f;
        param[4] = CAudioEffect::kParamFeedback;
        value[4] = 0.f;
        
        m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels,1.F,CAudioEffectDelay::kBasicDelay,param,value,iNumParams);

        int iDelayInSamples = static_cast<int>(value[1]*m_fSampleRate+1);
        float fAmplitude    = .77F;

        //dirac
        for (int c = 0; c < m_iNumChannels; c++)
            m_ppfInputData[c][c]    = fAmplitude;

        TestProcess();

        for (int c = 0; c < m_iNumChannels; c++)
        {
            for (int i= 0; i < m_iDataLength; i++)
            {
                if (i == c)
                    CHECK_CLOSE(fAmplitude*(1-value[0]), m_ppfOutputData[c][i], 1e-3F);
                else if (i == c+iDelayInSamples)
                    CHECK_CLOSE(fAmplitude*value[0], m_ppfOutputData[c][i], 1e-3F);
                else
                    CHECK_CLOSE(0.F, m_ppfOutputData[c][i], 1e-3F);
            }
        }

        m_phDelay->reset();

    }
    
    // delay: feedback check
    TEST_FIXTURE(DelayData, CheckFeedback)
    {
        int iNumParams = 5;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamDryWetMix;
        value[0] = 1.f;
        param[1] = CAudioEffect::kParamDelayInSecs;
        value[1] = 0.3f;
        param[2] = CAudioEffect::kParamModRateInHz;
        value[2] = 0.2f;
        param[3] = CAudioEffect::kParamModWidthInSecs;
        value[3] = 0.01f;
        param[4] = CAudioEffect::kParamFeedback;
        value[4] = 0.7f;
        
        m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels,1.F,CAudioEffectDelay::kBasicDelay,param,value,iNumParams);

        int iDelayInSamples = static_cast<int>(value[1]*m_fSampleRate+1);
        float fAmplitude    = .77F;

        //dirac
        for (int c = 0; c < m_iNumChannels; c++)
            m_ppfInputData[c][c]    = fAmplitude;

        TestProcess();

        for (int c = 0; c < m_iNumChannels; c++)
        {
            for (int i= 0; i < m_iDataLength; i++)
            {
                if (i == c)
                    CHECK_CLOSE(0, m_ppfOutputData[c][i], 1e-3F);
                else if (i == c+iDelayInSamples)
                    CHECK_CLOSE(fAmplitude, m_ppfOutputData[c][i], 1e-3F);
                else if (i == c+2*iDelayInSamples)
                    CHECK_CLOSE(fAmplitude*value[4], m_ppfOutputData[c][i], 1e-3F);
                else if (i == c+3*iDelayInSamples)
                    CHECK_CLOSE(fAmplitude*value[4]*value[4], m_ppfOutputData[c][i], 1e-3F);
                else if ((i-c)%(iDelayInSamples) !=0)
                    CHECK_CLOSE(0.F, m_ppfOutputData[c][i], 1e-3F);
                
            }
        }

        m_phDelay->reset();

    }

    

    // Delay: varying block length
    TEST_FIXTURE(DelayData, DelayVaryingBlocksize)
    {
        int iNumParams = 5;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamDryWetMix;
        value[0] = 0.5f;
        param[1] = CAudioEffect::kParamDelayInSecs;
        value[1] = 0.3f;
        param[2] = CAudioEffect::kParamModRateInHz;
        value[2] = 0.2f;
        param[3] = CAudioEffect::kParamModWidthInSecs;
        value[3] = 0.01f;
        param[4] = CAudioEffect::kParamFeedback;
        value[4] = 0.f;
        
        m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels,1.F,CAudioEffectDelay::kBasicDelay,param,value,iNumParams);

        TestProcess();

        m_phDelay->reset();

        m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels,1.F,CAudioEffectDelay::kBasicDelay,param,value,iNumParams);
        {
            int iNumFramesRemaining = m_iDataLength;
            while (iNumFramesRemaining > 0)
            {

                int iNumFrames = std::min(static_cast<float>(iNumFramesRemaining), static_cast<float>(rand())/RAND_MAX*17000.F);

                for (int c = 0; c < m_iNumChannels; c++)
                {
                    m_ppfInputTmp[c]    = &m_ppfInputData[c][m_iDataLength - iNumFramesRemaining];
                }
                m_phDelay->process(m_ppfInputTmp, m_ppfInputTmp, iNumFrames);

                iNumFramesRemaining -= iNumFrames;
            }
        }

        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

    }

    
}

#endif //WITH_TESTS
