#include "AudioFxFrameworkConfig.h"

#ifdef WITH_TESTS
#include <cassert>
#include <cstdio>

#include "UnitTest++.h"

#include "Synthesis.h"
#include "Vector.h"
#include "AudioEffect.h"
#include "AudioEffectDistortion.h"
#include <algorithm>

SUITE(Distortion)
{
    struct DistortionData
    {
        DistortionData():
            m_phDistortion(0),
            m_ppfInputData(0),
            m_ppfOutputData(0),
            m_iDataLength(35131),
            m_iBlockLength(171),
            m_iNumChannels(3),
            m_fSampleRate(8000)
        {
            m_phDistortion = new CAudioEffectDistortion();

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
        
        ~DistortionData()
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

            m_phDistortion->reset();
            delete m_phDistortion;
            m_phDistortion = 0;
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
                m_phDistortion->process(m_ppfInputTmp, m_ppfOutputTmp, iNumFrames);

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
                m_phDistortion->process(m_ppfInputTmp, m_ppfInputTmp, iNumFrames);

                iNumFramesRemaining -= iNumFrames;
            }
        }
        
        CAudioEffectDistortion    *m_phDistortion = 0;
        float               **m_ppfInputData,
                            **m_ppfOutputData,
                            **m_ppfInputTmp,
                            **m_ppfOutputTmp;
        int                 m_iDataLength;
        int                 m_iBlockLength;
        int                 m_iNumChannels;
        float               m_fSampleRate;
    };
    
    // zero input -> zero output (same output)
    TEST_FIXTURE(DistortionData, ZeroInput)
    {
        int iNumParams = 2;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamGain;
        value[0] = 5.f;
        param[1] = CAudioEffect::kParamDryWetMix;
        value[1] = 0.5f;
        
        m_phDistortion->init(m_fSampleRate,m_iNumChannels,param,value,iNumParams);
        
        TestProcess();

        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

        m_phDistortion->reset();
    }
    
    // DryWetMix = 0 -> same output
    TEST_FIXTURE(DistortionData, DryTest)
    {
        int iNumParams = 2;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamGain;
        value[0] = 5.f;
        param[1] = CAudioEffect::kParamDryWetMix;
        value[1] = 0.f;
        
        m_phDistortion->init(m_fSampleRate,m_iNumChannels,param,value,iNumParams);
        
        for (int c = 0; c < m_iNumChannels; c++)
        CSynthesis::generateSine (m_ppfInputData[c], 440, m_fSampleRate, m_iDataLength, .8F, static_cast<float>(c*M_PI_2));
        
        TestProcess();

        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

        m_phDistortion->reset();
    }
    
    
    // varying block length
    TEST_FIXTURE(DistortionData, VaryingBlocksize)
    {
        int iNumParams = 2;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamGain;
        value[0] = 5.f;
        param[1] = CAudioEffect::kParamDryWetMix;
        value[1] = 0.5f;
        
        m_phDistortion->init(m_fSampleRate,m_iNumChannels,param,value,iNumParams);

        TestProcess();

        m_phDistortion->reset();
        m_phDistortion->init(m_fSampleRate,m_iNumChannels,param,value,iNumParams);
        
        {
            int iNumFramesRemaining = m_iDataLength;
            while (iNumFramesRemaining > 0)
            {

                int iNumFrames = std::min(static_cast<float>(iNumFramesRemaining), static_cast<float>(rand())/RAND_MAX*17000.F);

                for (int c = 0; c < m_iNumChannels; c++)
                {
                    m_ppfInputTmp[c]    = &m_ppfInputData[c][m_iDataLength - iNumFramesRemaining];
                }
                m_phDistortion->process(m_ppfInputTmp, m_ppfInputTmp, iNumFrames);

                iNumFramesRemaining -= iNumFrames;
            }
        }

        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

    }
    
}

#endif //WITH_TESTS
