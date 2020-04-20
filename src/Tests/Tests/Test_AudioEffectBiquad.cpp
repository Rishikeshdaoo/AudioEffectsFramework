#include "AudioFxFrameworkConfig.h"

#ifdef WITH_TESTS
#include <cassert>
#include <cstdio>

#include "UnitTest++.h"

#include "Synthesis.h"
#include "Vector.h"
#include "AudioEffect.h"
#include "AudioEffectBiquad.h"
#include <algorithm>

SUITE(Biquad)
{
    struct BiquadData
    {
        BiquadData():
            m_phBiquad(0),
            m_ppfInputData(0),
            m_ppfOutputData(0),
            m_iDataLength(35131),
            m_iBlockLength(171),
            m_iNumChannels(3),
            m_fSampleRate(8000)
        {
            m_phBiquad = new CAudioEffectBiquad();
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
        
        ~BiquadData()
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

            m_phBiquad->reset();
            delete m_phBiquad;
            m_phBiquad = 0;
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
                m_phBiquad->process(m_ppfInputTmp, m_ppfOutputTmp, iNumFrames);

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
                m_phBiquad->process(m_ppfInputTmp, m_ppfInputTmp, iNumFrames);

                iNumFramesRemaining -= iNumFrames;
            }
        }
        
        CAudioEffectBiquad    *m_phBiquad = 0;
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
    TEST_FIXTURE(BiquadData, ZeroInput)
    {
        int iNumParams = 3;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamGain;
        value[0] = 0.5f;
        param[1] = CAudioEffect::kParamCenterFrequency;
        value[1] = 1000.0f;
        param[2] = CAudioEffect::kParamQ;
        value[2] = 0.707f;
        
        m_phBiquad->init(m_fSampleRate,m_iNumChannels,param,value,iNumParams);
        m_phBiquad->setFilterType(CAudioEffectBiquad::FilterType_t::kBandpass);
        
        TestProcess();

        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

        m_phBiquad->reset();
    }
    
    
    // varying block length
    TEST_FIXTURE(BiquadData, VaryingBlocksize)
    {
        int iNumParams = 3;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamGain;
        value[0] = 0.5f;
        param[1] = CAudioEffect::kParamCenterFrequency;
        value[1] = 1000.0f;
        param[2] = CAudioEffect::kParamQ;
        value[2] = 0.707f;
        
        m_phBiquad->init(m_fSampleRate,m_iNumChannels,param,value,iNumParams);
        m_phBiquad->setFilterType(CAudioEffectBiquad::FilterType_t::kBandpass);
        
        TestProcess();

        m_phBiquad->reset();
        m_phBiquad->init(m_fSampleRate,m_iNumChannels,param,value,iNumParams);
        m_phBiquad->setFilterType(CAudioEffectBiquad::FilterType_t::kBandpass);
        
        {
            int iNumFramesRemaining = m_iDataLength;
            while (iNumFramesRemaining > 0)
            {

                int iNumFrames = std::min(static_cast<float>(iNumFramesRemaining), static_cast<float>(rand())/RAND_MAX*17000.F);

                for (int c = 0; c < m_iNumChannels; c++)
                {
                    m_ppfInputTmp[c]    = &m_ppfInputData[c][m_iDataLength - iNumFramesRemaining];
                }
                m_phBiquad->process(m_ppfInputTmp, m_ppfInputTmp, iNumFrames);

                iNumFramesRemaining -= iNumFrames;
            }
        }

        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

    }
    
}

#endif //WITH_TESTS
