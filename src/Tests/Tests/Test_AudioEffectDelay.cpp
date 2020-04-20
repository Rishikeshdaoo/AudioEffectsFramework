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
                CVectorFloat::setZero(m_ppfOutputData[i], m_iDataLength);            }
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
        int iNumParams = 2;
        CAudioEffect::EffectParam_t param[2];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamGain;
        value[0] = 0.5f;
        param[1] = CAudioEffect::kParamDelayInSecs;
        value[1] = 0.5f;
        m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels, m_iMaxDelayInSec, param, value, iNumParams);
        m_phDelay -> setDelayType(CAudioEffectDelay::kDelay);
        
        TestProcess();

        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

        m_phDelay->reset();
    }
    
    
    // delay: zero gain -> same output
    TEST_FIXTURE(DelayData, DelayZeroGain)
    {
        int iNumParams = 2;
        CAudioEffect::EffectParam_t param[2];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamGain;
        value[0] = 0.f;
        param[1] = CAudioEffect::kParamDelayInSecs;
        value[1] = 0.5f;
        m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels, m_iMaxDelayInSec, param, value, iNumParams);
        m_phDelay -> setDelayType(CAudioEffectDelay::kDelay);
        
        for (int c = 0; c < m_iNumChannels; c++)
        CSynthesis::generateSine (m_ppfInputData[c], 440, m_fSampleRate, m_iDataLength, .8F, static_cast<float>(c*M_PI_2));
        
        TestProcess();

        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

        m_phDelay->reset();
    }
    
    
    // delay: check array
    TEST_FIXTURE(DelayData, CheckArray)
    {
        int iNumParams = 2;
        CAudioEffect::EffectParam_t param[2];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamGain;
        value[0] = 0.4f;
        param[1] = CAudioEffect::kParamDelayInSecs;
        value[1] = 0.8f;
        m_fGain = value[0];
        m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels, m_iMaxDelayInSec, param, value, iNumParams);
        m_phDelay -> setDelayType(CAudioEffectDelay::kDelay);
        
        int iDelayInSamples = static_cast<int>(value[1]*m_fSampleRate+0.5);
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
                    CHECK_CLOSE(fAmplitude, m_ppfOutputData[c][i], 1e-3F);
                else if (i == c+iDelayInSamples)
                    CHECK_CLOSE(fAmplitude*m_fGain, m_ppfOutputData[c][i], 1e-3F);
                else
                    CHECK_CLOSE(0.F, m_ppfOutputData[c][i], 1e-3F);
            }
        }

        m_phDelay->reset();

    }
    
    
    // flanger: zero gain -> same output
    TEST_FIXTURE(DelayData, FlangerZeroGain)
    {
        int iNumParams = 4;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamDelayInSecs;
        value[0] = 0.02f;
        param[1] = CAudioEffect::kParamModRateInHz;
        value[1] = 5.f;
        param[2] = CAudioEffect::kParamModWidthInSecs;
        value[2] = 0.02f;
        param[3] = CAudioEffect::kParamGain;
        value[3] = 0.f;
                
        m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels, m_iMaxDelayInSec, param, value, iNumParams);
        m_phDelay -> setDelayType(CAudioEffectDelay::kFlanger);
        
        for (int c = 0; c < m_iNumChannels; c++)
        CSynthesis::generateSine (m_ppfInputData[c], 440, m_fSampleRate, m_iDataLength, .8F, static_cast<float>(c*M_PI_2));
        
        TestProcess();

        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

        m_phDelay->reset();
    }
    
    // chorus: zero gain -> same output
    TEST_FIXTURE(DelayData, ChorusZeroGain)
    {
        int iNumParams = 4;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamDelayInSecs;
        value[0] = 0.015f;
        param[1] = CAudioEffect::kParamModRateInHz;
        value[1] = 5.f;
        param[2] = CAudioEffect::kParamModWidthInSecs;
        value[2] = 0.5f;
        param[3] = CAudioEffect::kParamGain;
        value[3] = 0.f;
                
        m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels, m_iMaxDelayInSec, param, value, iNumParams);
        m_phDelay -> setDelayType(CAudioEffectDelay::kChorus);
        
        for (int c = 0; c < m_iNumChannels; c++)
        CSynthesis::generateSine (m_ppfInputData[c], 440, m_fSampleRate, m_iDataLength, .8F, static_cast<float>(c*M_PI_2));
        
        TestProcess();

        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

        m_phDelay->reset();
    }
    

   
    // chorus: zero ModWidth
    TEST_FIXTURE(DelayData, ChorusZeroModWidth)
    {
        int iNumParams = 4;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamDelayInSecs;
        value[0] = 0.015f;
        param[1] = CAudioEffect::kParamModRateInHz;
        value[1] = 5.f;
        param[2] = CAudioEffect::kParamModWidthInSecs;
        value[2] = 0.f;
        param[3] = CAudioEffect::kParamGain;
        value[3] = 0.7f;

        m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels, m_iMaxDelayInSec, param, value, iNumParams);
        m_phDelay -> setDelayType(CAudioEffectDelay::kChorus);

        int iDelayInSamples = static_cast<int>(value[0]*m_fSampleRate+0.5);
        float fAmplitude    = .77F;

        for (int c = 0; c < m_iNumChannels; c++)
            m_ppfInputData[c][c]    = fAmplitude;

        TestProcess();

        for (int c = 0; c < m_iNumChannels; c++)
        {
            for (int i= 0; i < m_iDataLength; i++)
            {
                if (i == c)
                    CHECK_CLOSE(fAmplitude, m_ppfOutputData[c][i], 1e-3F);
                else if (i == c+iDelayInSamples)
                    CHECK_CLOSE(fAmplitude * value[3], m_ppfOutputData[c][i], 1e-3F);
            }
        }

        m_phDelay->reset();
    }
    
    // flanger: zero ModWidth
    TEST_FIXTURE(DelayData, FlangerZeroModWidth)
    {
        int iNumParams = 4;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamDelayInSecs;
        value[0] = 0.02f;
        param[1] = CAudioEffect::kParamModRateInHz;
        value[1] = 5.f;
        param[2] = CAudioEffect::kParamModWidthInSecs;
        value[2] = 0.f;
        param[3] = CAudioEffect::kParamGain;
        value[3] = 0.7f;

        m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels, m_iMaxDelayInSec, param, value, iNumParams);
        m_phDelay -> setDelayType(CAudioEffectDelay::kFlanger);

        float fAmplitude    = .77F;

        for (int c = 0; c < m_iNumChannels; c++)
            m_ppfInputData[c][c]    = fAmplitude;

        TestProcess();

        for (int c = 0; c < m_iNumChannels; c++)
        {
            for (int i= 0; i < m_iDataLength; i++)
            {
                if (i == c)
                    CHECK_CLOSE((fAmplitude + fAmplitude * value[3]), m_ppfOutputData[c][i], 1e-3F);
                else
                    CHECK_CLOSE(0.F, m_ppfOutputData[c][i], 1e-3F);
            }
        }

        m_phDelay->reset();
    }


    
    
    // Delay: varying block length
    TEST_FIXTURE(DelayData, DelayVaryingBlocksize)
    {
        int iNumParams = 2;
        CAudioEffect::EffectParam_t param[2];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamGain;
        value[0] = 0.5f;
        param[1] = CAudioEffect::kParamDelayInSecs;
        value[1] = 0.5f;
        m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels, m_iMaxDelayInSec, param, value, iNumParams);
        m_phDelay -> setDelayType(CAudioEffectDelay::kDelay);
        
        TestProcess();

        m_phDelay->reset();

        m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels, m_iMaxDelayInSec, param, value, iNumParams);

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
    
    // Flanger: varying block length
    TEST_FIXTURE(DelayData, FlangerVaryingBlocksize)
    {
        int iNumParams = 4;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamDelayInSecs;
        value[0] = 0.02f;
        param[1] = CAudioEffect::kParamModRateInHz;
        value[1] = 5.f;
        param[2] = CAudioEffect::kParamModWidthInSecs;
        value[2] = 0.02f;
        param[3] = CAudioEffect::kParamGain;
        value[3] = 0.7f;
        
        m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels, m_iMaxDelayInSec, param, value, iNumParams);
        m_phDelay -> setDelayType(CAudioEffectDelay::kFlanger);

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
    
    // Chorus: varying block length
    TEST_FIXTURE(DelayData, ChorusVaryingBlocksize)
    {
        int iNumParams = 4;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamDelayInSecs;
        value[0] = 0.015f;
        param[1] = CAudioEffect::kParamModRateInHz;
        value[1] = 0.5f;
        param[2] = CAudioEffect::kParamModWidthInSecs;
        value[2] = 0.01f;
        param[3] = CAudioEffect::kParamGain;
        value[3] = 0.7f;
        
        m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels, m_iMaxDelayInSec, param, value, iNumParams);
        m_phDelay -> setDelayType(CAudioEffectDelay::kChorus);

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
