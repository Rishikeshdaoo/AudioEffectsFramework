#include "AudioFxFrameworkConfig.h"

#ifdef WITH_TESTS
#include <cassert>
#include <cstdio>

#include "UnitTest++.h"

#include "Synthesis.h"
#include "Vector.h"
#include "AudioEffect.h"
#include "AudioEffectGain.h"
#include "AudioEffectPan.h"
#include "AudioEffectDelay.h"
#include <algorithm>

SUITE(Gain)
{
    struct GainData
    {
        GainData():
            m_phGain(0),
            m_ppfInputData(0),
            m_ppfOutputData(0),
            m_iDataLength(351),
            m_iBlockLength(171),
            m_iNumChannels(3),
            m_fSampleRate(8000),
            m_fGain(.5F)
        {
            m_phGain = new CAudioEffectGain();

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
        
        ~GainData()
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
            m_phGain->reset();
            delete m_phGain;
            m_phGain = 0;
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
                m_phGain->process(m_ppfInputTmp, m_ppfOutputTmp, iNumFrames);

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
                m_phGain->process(m_ppfInputTmp, m_ppfInputTmp, iNumFrames);

                iNumFramesRemaining -= iNumFrames;
            }
        }
        
        CAudioEffectGain    *m_phGain = 0;
        float               **m_ppfInputData,
                            **m_ppfOutputData,
                            **m_ppfInputTmp,
                            **m_ppfOutputTmp;
        int                 m_iDataLength;
        int                 m_iBlockLength;
        int                 m_iNumChannels;
        float               m_fSampleRate;
        float               m_fGain;
    };
    
    // zero input -> zero output (same output)
    TEST_FIXTURE(GainData, ZeroInput)
    {
        int iNumParams = 1;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamGain;
        value[0] = 0.5f;
        m_phGain->init(m_fSampleRate,m_iNumChannels,param,value,iNumParams);
        
        TestProcess();

        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

        m_phGain->reset();
    }
    
    // one gain -> same output
    TEST_FIXTURE(GainData, OneGain)
    {
        int iNumParams = 1;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamGain;
        value[0] = 1.f;
        m_phGain->init(m_fSampleRate,m_iNumChannels,param,value,iNumParams);
        
        for (int c = 0; c < m_iNumChannels; c++)
        CSynthesis::generateSine (m_ppfInputData[c], 440, m_fSampleRate, m_iDataLength, .8F, static_cast<float>(c*M_PI_2));
        
        TestProcess();

        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

        m_phGain->reset();

    }
    
    // varying block length
    TEST_FIXTURE(GainData, VaryingBlocksize)
    {
        int iNumParams = 1;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamGain;
        value[0] = 0.5f;
        m_phGain->init(m_fSampleRate,m_iNumChannels,param,value,iNumParams);

        TestProcess();

        m_phGain->reset();
        m_phGain->init(m_fSampleRate,m_iNumChannels,param,value,iNumParams);
        
        {
            int iNumFramesRemaining = m_iDataLength;
            while (iNumFramesRemaining > 0)
            {

                int iNumFrames = std::min(static_cast<float>(iNumFramesRemaining), static_cast<float>(rand())/RAND_MAX*17000.F);

                for (int c = 0; c < m_iNumChannels; c++)
                {
                    m_ppfInputTmp[c]    = &m_ppfInputData[c][m_iDataLength - iNumFramesRemaining];
                }
                m_phGain->process(m_ppfInputTmp, m_ppfInputTmp, iNumFrames);

                iNumFramesRemaining -= iNumFrames;
            }
        }

        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

    }
    
}


SUITE(Pan)
{
    struct PanData
    {
        PanData():
            m_phPan(0),
            m_ppfInputData(0),
            m_ppfOutputData(0),
            m_iDataLength(351),
            m_iBlockLength(171),
            m_iNumChannelsInput(0),
            m_iNumChannelsOutput(0),
            m_fSampleRate(8000),
            m_fPan(0.F)
        {
            m_phPan = new CAudioEffectPan();

//            m_ppfInputData  = new float*[m_iNumChannelsInput];
//            m_ppfInputTmp   = new float*[m_iNumChannelsInput];
//            for (int i = 0; i < m_iNumChannelsInput; i++)
//            {
//                m_ppfInputData[i]   = new float [m_iDataLength];
//                CVectorFloat::setZero(m_ppfInputData[i], m_iDataLength);
//            }
            
//            m_ppfOutputData = new float*[m_iNumChannelsOutput];
//            m_ppfOutputTmp  = new float*[m_iNumChannelsOutput];
//            for (int i = 0; i < m_iNumChannelsOutput; i++)
//            {
//                m_ppfOutputData[i]  = new float [m_iDataLength];
//                CVectorFloat::setZero(m_ppfOutputData[i], m_iDataLength);
//            }
        }
        
        ~PanData()
        {
            for (int i = 0; i < m_iNumChannelsInput; i++)
            {
                delete [] m_ppfInputData[i];
            }
            for (int i = 0; i < m_iNumChannelsOutput; i++)
            {
                delete [] m_ppfOutputData[i];
            }
            delete [] m_ppfOutputTmp;
            delete [] m_ppfInputTmp;
            delete [] m_ppfOutputData;
            delete [] m_ppfInputData;

            //destroy?
            m_phPan->reset();
            delete m_phPan;
            m_phPan = 0;
        }
        
        void TestProcess()
        {
            int iNumFramesRemaining = m_iDataLength;
            while (iNumFramesRemaining > 0)
            {
                int iNumFrames = std::min(iNumFramesRemaining, m_iBlockLength);

                for (int c = 0; c < m_iNumChannelsInput; c++)
                {
                    m_ppfInputTmp[c]    = &m_ppfInputData[c][m_iDataLength - iNumFramesRemaining];
                }
                for (int c = 0; c < m_iNumChannelsOutput; c++)
                {
                    m_ppfOutputTmp[c]   = &m_ppfOutputData[c][m_iDataLength - iNumFramesRemaining];
                }
                m_phPan->process(m_ppfInputTmp, m_ppfOutputTmp, iNumFrames);

                iNumFramesRemaining -= iNumFrames;
            }
        }
        
        void TestProcessInplace()
        {
            int iNumFramesRemaining = m_iDataLength;
            while (iNumFramesRemaining > 0)
            {
                int iNumFrames = std::min(iNumFramesRemaining, m_iBlockLength);

                for (int c = 0; c < m_iNumChannelsInput; c++)
                {
                    m_ppfInputTmp[c]    = &m_ppfInputData[c][m_iDataLength - iNumFramesRemaining];
                }
                for (int c = 0; c < m_iNumChannelsOutput; c++)
                {
                    m_ppfOutputTmp[c]   = &m_ppfOutputData[c][m_iDataLength - iNumFramesRemaining];
                }
                m_phPan->process(m_ppfInputTmp, m_ppfInputTmp, iNumFrames);

                iNumFramesRemaining -= iNumFrames;
            }
        }
        
        CAudioEffectPan     *m_phPan = 0;
        float               **m_ppfInputData,
                            **m_ppfOutputData,
                            **m_ppfInputTmp,
                            **m_ppfOutputTmp;
        int                 m_iDataLength;
        int                 m_iBlockLength;
        int                 m_iNumChannelsInput;
        int                 m_iNumChannelsOutput;
        float               m_fSampleRate;
        float               m_fPan;
    };
    
    // (in 2 out 2) zero input -> zero output (same output)
    TEST_FIXTURE(PanData, ZeroInput2_2)
    {
        m_iNumChannelsInput = 2;
        m_iNumChannelsOutput = 2;
        
        m_ppfInputData  = new float*[m_iNumChannelsInput];
        m_ppfInputTmp   = new float*[m_iNumChannelsInput];
        for (int i = 0; i < m_iNumChannelsInput; i++)
        {
            m_ppfInputData[i]   = new float [m_iDataLength];
            CVectorFloat::setZero(m_ppfInputData[i], m_iDataLength);
        }
        
        m_ppfOutputData = new float*[m_iNumChannelsOutput];
        m_ppfOutputTmp  = new float*[m_iNumChannelsOutput];
        
        for (int i = 0; i < m_iNumChannelsOutput; i++)
        {
            m_ppfOutputData[i]  = new float [m_iDataLength];
            CVectorFloat::setZero(m_ppfOutputData[i], m_iDataLength);
        }
        
        int iNumParams = 1;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamPan;
        value[0] = 0.5f;
        m_phPan->init(m_fSampleRate,m_iNumChannelsInput,param,value,iNumParams);
        
        TestProcess();

        for (int c = 0; c < m_iNumChannelsInput; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

        m_phPan->reset();
    }
    
    // (in 2 out 2) pan = 0 -> same input output
    TEST_FIXTURE(PanData, PanZero2_2)
    {
        m_iNumChannelsInput = 2;
        m_iNumChannelsOutput = 2;
        
        m_ppfInputData  = new float*[m_iNumChannelsInput];
        m_ppfInputTmp   = new float*[m_iNumChannelsInput];
        for (int i = 0; i < m_iNumChannelsInput; i++)
        {
            m_ppfInputData[i]   = new float [m_iDataLength];
            CVectorFloat::setZero(m_ppfInputData[i], m_iDataLength);
        }
        
        m_ppfOutputData = new float*[m_iNumChannelsOutput];
        m_ppfOutputTmp  = new float*[m_iNumChannelsOutput];
        for (int i = 0; i < m_iNumChannelsOutput; i++)
        {
            m_ppfOutputData[i]  = new float [m_iDataLength];
            CVectorFloat::setZero(m_ppfOutputData[i], m_iDataLength);
        }
        
        int iNumParams = 1;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamPan;
        value[0] = 0.f;
        m_phPan->init(m_fSampleRate,m_iNumChannelsInput,param,value,iNumParams);

        for (int c = 0; c < m_iNumChannelsInput; c++)
        CSynthesis::generateSine (m_ppfInputData[c], 440, m_fSampleRate, m_iDataLength, .8F, static_cast<float>(c*M_PI_2));

        TestProcess();

        CHECK_ARRAY_CLOSE(m_ppfInputData[0], m_ppfOutputData[0], m_iDataLength, 1e-3);
        CHECK_ARRAY_CLOSE(m_ppfInputData[1], m_ppfOutputData[1], m_iDataLength, 1e-3);

        m_phPan->reset();

    }
    
    // (in 2 out 2) pan = -1 -> right channel = zeros
    TEST_FIXTURE(PanData, PanLeft2_2)
    {
        m_iNumChannelsInput = 2;
        m_iNumChannelsOutput = 2;
        
        m_ppfInputData  = new float*[m_iNumChannelsInput];
        m_ppfInputTmp   = new float*[m_iNumChannelsInput];
        for (int i = 0; i < m_iNumChannelsInput; i++)
        {
            m_ppfInputData[i]   = new float [m_iDataLength];
            CVectorFloat::setZero(m_ppfInputData[i], m_iDataLength);
        }
        
        m_ppfOutputData = new float*[m_iNumChannelsOutput];
        m_ppfOutputTmp  = new float*[m_iNumChannelsOutput];
        for (int i = 0; i < m_iNumChannelsOutput; i++)
        {
            m_ppfOutputData[i]  = new float [m_iDataLength];
            CVectorFloat::setZero(m_ppfOutputData[i], m_iDataLength);
        }
        
        int iNumParams = 1;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamPan;
        value[0] = -1.f;
        m_phPan->init(m_fSampleRate,m_iNumChannelsInput,param,value,iNumParams);

        for (int c = 0; c < m_iNumChannelsInput; c++)
        CSynthesis::generateSine (m_ppfInputData[c], 440, m_fSampleRate, m_iDataLength, .8F, static_cast<float>(c*M_PI_2));

        TestProcess();

        float *pZeroData = new float [m_iDataLength];
        CVectorFloat::setZero(pZeroData, m_iDataLength);
        CHECK_ARRAY_CLOSE(m_ppfInputData[0], m_ppfOutputData[0], m_iDataLength, 1e-3);
        CHECK_ARRAY_CLOSE(m_ppfOutputData[1], pZeroData, m_iDataLength, 1e-3);
        delete [] pZeroData;
        pZeroData = 0;

        m_phPan->reset();

    }
    
    // (in 2 out 2) pan = 1 -> right channel = zeros
    TEST_FIXTURE(PanData, PanRight2_2)
    {
        m_iNumChannelsInput = 2;
        m_iNumChannelsOutput = 2;
        
        m_ppfInputData  = new float*[m_iNumChannelsInput];
        m_ppfInputTmp   = new float*[m_iNumChannelsInput];
        for (int i = 0; i < m_iNumChannelsInput; i++)
        {
            m_ppfInputData[i]   = new float [m_iDataLength];
            CVectorFloat::setZero(m_ppfInputData[i], m_iDataLength);
        }
        
        m_ppfOutputData = new float*[m_iNumChannelsOutput];
        m_ppfOutputTmp  = new float*[m_iNumChannelsOutput];
        for (int i = 0; i < m_iNumChannelsOutput; i++)
        {
            m_ppfOutputData[i]  = new float [m_iDataLength];
            CVectorFloat::setZero(m_ppfOutputData[i], m_iDataLength);
        }
        
        int iNumParams = 1;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamPan;
        value[0] = 1.f;
        m_phPan->init(m_fSampleRate,m_iNumChannelsInput,param,value,iNumParams);

        for (int c = 0; c < m_iNumChannelsInput; c++)
        CSynthesis::generateSine (m_ppfInputData[c], 440, m_fSampleRate, m_iDataLength, .8F, static_cast<float>(c*M_PI_2));

        TestProcess();

        float *pZeroData = new float [m_iDataLength];
        CVectorFloat::setZero(pZeroData, m_iDataLength);
        CHECK_ARRAY_CLOSE(m_ppfInputData[1], m_ppfOutputData[1], m_iDataLength, 1e-3);
        CHECK_ARRAY_CLOSE(m_ppfOutputData[0], pZeroData, m_iDataLength, 1e-3);
        delete [] pZeroData;
        pZeroData = 0;

        m_phPan->reset();

    }
    
    // (in 2 out 2) varying block length
    TEST_FIXTURE(PanData, VaryingBlocksize2_2)
    {
        m_iNumChannelsInput = 2;
        m_iNumChannelsOutput = 2;
        
        m_ppfInputData  = new float*[m_iNumChannelsInput];
        m_ppfInputTmp   = new float*[m_iNumChannelsInput];
        for (int i = 0; i < m_iNumChannelsInput; i++)
        {
            m_ppfInputData[i]   = new float [m_iDataLength];
            CVectorFloat::setZero(m_ppfInputData[i], m_iDataLength);
        }
        
        m_ppfOutputData = new float*[m_iNumChannelsOutput];
        m_ppfOutputTmp  = new float*[m_iNumChannelsOutput];
        for (int i = 0; i < m_iNumChannelsOutput; i++)
        {
            m_ppfOutputData[i]  = new float [m_iDataLength];
            CVectorFloat::setZero(m_ppfOutputData[i], m_iDataLength);
        }
        
        int iNumParams = 1;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamGain;
        value[0] = 0.5f;
        m_phPan->init(m_fSampleRate,m_iNumChannelsInput,param,value,iNumParams);

        TestProcess();

        m_phPan->reset();
        m_phPan->init(m_fSampleRate,m_iNumChannelsInput,param,value,iNumParams);
        
        {
            int iNumFramesRemaining = m_iDataLength;
            while (iNumFramesRemaining > 0)
            {

                int iNumFrames = std::min(static_cast<float>(iNumFramesRemaining), static_cast<float>(rand())/RAND_MAX*17000.F);

                for (int c = 0; c < m_iNumChannelsInput; c++)
                {
                    m_ppfInputTmp[c]    = &m_ppfInputData[c][m_iDataLength - iNumFramesRemaining];
                }
                m_phPan->process(m_ppfInputTmp, m_ppfInputTmp, iNumFrames);

                iNumFramesRemaining -= iNumFrames;
            }
        }

        for (int c = 0; c < m_iNumChannelsInput; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

    }
    
    // (in 1 out 2) zero input -> zero output (same output)
    TEST_FIXTURE(PanData, ZeroInput1_2)
    {
        m_iNumChannelsInput = 1;
        m_iNumChannelsOutput = 2;
        
        m_ppfInputData  = new float*[m_iNumChannelsInput];
        m_ppfInputTmp   = new float*[m_iNumChannelsInput];
        for (int i = 0; i < m_iNumChannelsInput; i++)
        {
            m_ppfInputData[i]   = new float [m_iDataLength];
            CVectorFloat::setZero(m_ppfInputData[i], m_iDataLength);
        }
        
        m_ppfOutputData = new float*[m_iNumChannelsOutput];
        m_ppfOutputTmp  = new float*[m_iNumChannelsOutput];
        for (int i = 0; i < m_iNumChannelsOutput; i++)
        {
            m_ppfOutputData[i]  = new float [m_iDataLength];
            CVectorFloat::setZero(m_ppfOutputData[i], m_iDataLength);
        }
        
        int iNumParams = 1;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamPan;
        value[0] = 0.5f;
        m_phPan->init(m_fSampleRate,m_iNumChannelsInput,param,value,iNumParams);
        
        TestProcess();
        
        float *pZeroData = new float [m_iDataLength];
        CVectorFloat::setZero(pZeroData, m_iDataLength);
        for (int c = 0; c < m_iNumChannelsOutput; c++)
            CHECK_ARRAY_CLOSE(pZeroData, m_ppfOutputData[c], m_iDataLength, 1e-3);
        pZeroData = 0;
        delete [] pZeroData;
        
        m_phPan->reset();
    }
    
    // (in 1 out 2) pan = 0 -> same left right channel
    TEST_FIXTURE(PanData, PanZero1_2)
    {
       m_iNumChannelsInput = 1;
        m_iNumChannelsOutput = 2;
        
        m_ppfInputData  = new float*[m_iNumChannelsInput];
        m_ppfInputTmp   = new float*[m_iNumChannelsInput];
        for (int i = 0; i < m_iNumChannelsInput; i++)
        {
            m_ppfInputData[i]   = new float [m_iDataLength];
            CVectorFloat::setZero(m_ppfInputData[i], m_iDataLength);
        }
        
        m_ppfOutputData = new float*[m_iNumChannelsOutput];
        m_ppfOutputTmp  = new float*[m_iNumChannelsOutput];
        for (int i = 0; i < m_iNumChannelsOutput; i++)
        {
            m_ppfOutputData[i]  = new float [m_iDataLength];
            CVectorFloat::setZero(m_ppfOutputData[i], m_iDataLength);
        }
        
        int iNumParams = 1;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamPan;
        value[0] = 0.f;
        m_phPan->init(m_fSampleRate,m_iNumChannelsInput,param,value,iNumParams);

        for (int c = 0; c < m_iNumChannelsInput; c++)
        CSynthesis::generateSine (m_ppfInputData[c], 440, m_fSampleRate, m_iDataLength, .8F, static_cast<float>(c*M_PI_2));

        TestProcess();

        CHECK_ARRAY_CLOSE(m_ppfOutputData[0], m_ppfOutputData[1], m_iDataLength, 1e-3);

        m_phPan->reset();

    }
    
    // (in 1 out 2) pan = -1 -> right channel = zeros
    TEST_FIXTURE(PanData, PanLeft1_2)
    {
        m_iNumChannelsInput = 1;
        m_iNumChannelsOutput = 2;
        
        m_ppfInputData  = new float*[m_iNumChannelsInput];
               m_ppfInputTmp   = new float*[m_iNumChannelsInput];
               for (int i = 0; i < m_iNumChannelsInput; i++)
               {
                   m_ppfInputData[i]   = new float [m_iDataLength];
                   CVectorFloat::setZero(m_ppfInputData[i], m_iDataLength);
               }
               
               m_ppfOutputData = new float*[m_iNumChannelsOutput];
               m_ppfOutputTmp  = new float*[m_iNumChannelsOutput];
               for (int i = 0; i < m_iNumChannelsOutput; i++)
               {
                   m_ppfOutputData[i]  = new float [m_iDataLength];
                   CVectorFloat::setZero(m_ppfOutputData[i], m_iDataLength);
               }
        
        int iNumParams = 1;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamPan;
        value[0] = -1.f;
        m_phPan->init(m_fSampleRate,m_iNumChannelsInput,param,value,iNumParams);

        for (int c = 0; c < m_iNumChannelsInput; c++)
        CSynthesis::generateSine (m_ppfInputData[c], 440, m_fSampleRate, m_iDataLength, .8F, static_cast<float>(c*M_PI_2));

        TestProcess();

        float *pZeroData = new float [m_iDataLength];
        CVectorFloat::setZero(pZeroData, m_iDataLength);
        CHECK_ARRAY_CLOSE(m_ppfInputData[0], m_ppfOutputData[0], m_iDataLength, 1e-3);
        CHECK_ARRAY_CLOSE(m_ppfOutputData[1], pZeroData, m_iDataLength, 1e-3);
        delete [] pZeroData;
        pZeroData = 0;

        m_phPan->reset();

    }
    
    // (in 1 out 2) pan = 1 -> right channel = zeros
    TEST_FIXTURE(PanData, PanRight1_2)
    {
        m_iNumChannelsInput = 1;
        m_iNumChannelsOutput = 2;
        
        m_ppfInputData  = new float*[m_iNumChannelsInput];
               m_ppfInputTmp   = new float*[m_iNumChannelsInput];
               for (int i = 0; i < m_iNumChannelsInput; i++)
               {
                   m_ppfInputData[i]   = new float [m_iDataLength];
                   CVectorFloat::setZero(m_ppfInputData[i], m_iDataLength);
               }
               
               m_ppfOutputData = new float*[m_iNumChannelsOutput];
               m_ppfOutputTmp  = new float*[m_iNumChannelsOutput];
               for (int i = 0; i < m_iNumChannelsOutput; i++)
               {
                   m_ppfOutputData[i]  = new float [m_iDataLength];
                   CVectorFloat::setZero(m_ppfOutputData[i], m_iDataLength);
               }
        
        int iNumParams = 1;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamPan;
        value[0] = 1.f;
        m_phPan->init(m_fSampleRate,m_iNumChannelsInput,param,value,iNumParams);

        for (int c = 0; c < m_iNumChannelsInput; c++)
        CSynthesis::generateSine (m_ppfInputData[c], 440, m_fSampleRate, m_iDataLength, .8F, static_cast<float>(c*M_PI_2));

        TestProcess();

        float *pZeroData = new float [m_iDataLength];
        CVectorFloat::setZero(pZeroData, m_iDataLength);
        CHECK_ARRAY_CLOSE(m_ppfInputData[0], m_ppfOutputData[1], m_iDataLength, 1e-3);
        CHECK_ARRAY_CLOSE(m_ppfOutputData[0], pZeroData, m_iDataLength, 1e-3);
        delete [] pZeroData;
        pZeroData = 0;

        m_phPan->reset();

    }
    
    
    SUITE(Delay)
    {
        struct DelayData
        {
            DelayData():
                m_phDelay(0),
                m_ppfInputData(0),
                m_ppfOutputData(0),
                m_iDataLength(351),
                m_iBlockLength(171),
                m_iNumChannels(3),
                m_fSampleRate(8000),
                m_fGain(.5F)
            {
//                m_phDelay = new CAudioEffectDelay();

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
            float               m_fGain, m_fDelayInSecs;
            int                 m_iMaxDelayInSec = 1;
            
        };
        
        // zero input -> zero output (same output)
        TEST_FIXTURE(DelayData, ZeroInput)
        {
            int iNumParams = 2;
            CAudioEffect::EffectParam_t param[2];
            float value[iNumParams];
            param[0] = CAudioEffect::kParamGain;
            value[1] = 0.5f;
            param[0] = CAudioEffect::kParamDelayInSecs;
            value[1] = 0.5f;
            m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels, m_iMaxDelayInSec, param, value, iNumParams);
            
            TestProcess();

            for (int c = 0; c < m_iNumChannels; c++)
                CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

            m_phDelay->reset();
        }
        
        // zero gain -> same output
        TEST_FIXTURE(DelayData, ZeroGain)
        {
            int iNumParams = 2;
            CAudioEffect::EffectParam_t param[2];
            float value[iNumParams];
            param[0] = CAudioEffect::kParamGain;
            value[1] = 0.f;
            param[0] = CAudioEffect::kParamDelayInSecs;
            value[1] = 0.5f;
            m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels, m_iMaxDelayInSec, param, value, iNumParams);
            
            TestProcess();

            for (int c = 0; c < m_iNumChannels; c++)
                CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);

            m_phDelay->reset();
        }
        
        // check array
        TEST_FIXTURE(DelayData, CheckArray)
        {
            int iNumParams = 2;
            CAudioEffect::EffectParam_t param[2];
            float value[iNumParams];
            param[0] = CAudioEffect::kParamGain;
            value[1] = 0.6f;
            param[0] = CAudioEffect::kParamDelayInSecs;
            value[1] = 0.4f;
            m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels, m_iMaxDelayInSec, param, value, iNumParams);

            int iDelayInSamples = static_cast<int>(value[1]*m_fSampleRate + .5F);
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
                    else if (i == c+2*iDelayInSamples)
                        CHECK_CLOSE(fAmplitude*m_fGain*m_fGain, m_ppfOutputData[c][i], 1e-3F);
                    else if (i == c+3*iDelayInSamples)
                        CHECK_CLOSE(fAmplitude*m_fGain*m_fGain*m_fGain, m_ppfOutputData[c][i], 1e-3F);
                    else if (i == c+4*iDelayInSamples)
                        CHECK_CLOSE(fAmplitude*m_fGain*m_fGain*m_fGain*m_fGain, m_ppfOutputData[c][i], 1e-3F);
                    else if (i == c+5*iDelayInSamples)
                        CHECK_CLOSE(fAmplitude*m_fGain*m_fGain*m_fGain*m_fGain*m_fGain, m_ppfOutputData[c][i], 1e-3F);
                    else if ((i-c)%(iDelayInSamples) !=0)
                        CHECK_CLOSE(0.F, m_ppfOutputData[c][i], 1e-3F);
                }
            }

            m_phDelay->reset();

        }
        
        // varying block length
        TEST_FIXTURE(DelayData, VaryingBlocksize)
        {
            int iNumParams = 2;
            CAudioEffect::EffectParam_t param[2];
            float value[iNumParams];
            param[0] = CAudioEffect::kParamGain;
            value[1] = 0.5f;
            param[0] = CAudioEffect::kParamDelayInSecs;
            value[1] = 0.5f;
            m_phDelay = new CAudioEffectDelay(m_fSampleRate, m_iNumChannels, m_iMaxDelayInSec, param, value, iNumParams);
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
        
    }

}

#endif //WITH_TESTS
