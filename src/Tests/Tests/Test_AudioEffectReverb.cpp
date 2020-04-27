#include "AudioFxFrameworkConfig.h"

#ifdef WITH_TESTS
#include <cassert>
#include <cstdio>

#include "UnitTest++.h"

#include "Synthesis.h"
#include "Vector.h"
#include "AudioEffect.h"
#include "AudioEffectReverb.h"
#include <algorithm>

SUITE(Reverb)
{
    struct ReverbData
    {
        ReverbData():
            m_phReverb(0),
            m_ppfInputData(0),
            m_ppfOutputData(0),
            m_iDataLength(35131),
            m_iBlockLength(171),
            m_iNumChannels(3),
            m_fSampleRate(8000)
        {
            m_phReverb = new CAudioEffectReverb();

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
        
        ~ReverbData()
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

            m_phReverb->reset();
            delete m_phReverb;
            m_phReverb = 0;
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
                m_phReverb->process(m_ppfInputTmp, m_ppfOutputTmp, iNumFrames);

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
                m_phReverb->process(m_ppfInputTmp, m_ppfInputTmp, iNumFrames);

                iNumFramesRemaining -= iNumFrames;
            }
        }
        
        CAudioEffectReverb    *m_phReverb = 0;
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
    TEST_FIXTURE(ReverbData, ZeroInput)
    {
        int iNumParams = 2;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamNumFilters;
        value[0] = 3;
        param[1] = CAudioEffect::kParamFilterGains;
        value[1] = 0.707;
        
        float a_fFilterDelays[3] = {0.5f, 0.3f, 0.2f};

        m_phReverb->init(m_fSampleRate,m_iNumChannels,1.F,param,value,iNumParams);
        m_phReverb->setFilterDelays(a_fFilterDelays, 3);

        TestProcess();

        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

//        m_phReverb->reset();

    }
    
    
    // varying block length
    TEST_FIXTURE(ReverbData, VaryingBlocksize)
    {
        int iNumParams = 2;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamNumFilters;
        value[0] = 3;
        param[1] = CAudioEffect::kParamFilterGains;
        value[1] = 0.707;
        
        m_phReverb->init(m_fSampleRate,m_iNumChannels,1.F,param,value,iNumParams);

        TestProcess();

        m_phReverb->reset();
        m_phReverb->init(m_fSampleRate,m_iNumChannels,1.F,param,value,iNumParams);
        
        {
            int iNumFramesRemaining = m_iDataLength;
            while (iNumFramesRemaining > 0)
            {

                int iNumFrames = std::min(static_cast<float>(iNumFramesRemaining), static_cast<float>(rand())/RAND_MAX*17000.F);

                for (int c = 0; c < m_iNumChannels; c++)
                {
                    m_ppfInputTmp[c]    = &m_ppfInputData[c][m_iDataLength - iNumFramesRemaining];
                }
                m_phReverb->process(m_ppfInputTmp, m_ppfInputTmp, iNumFrames);

                iNumFramesRemaining -= iNumFrames;
            }
        }

        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

    }
    
}

#endif //WITH_TESTS
