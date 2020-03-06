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


SUITE(Pan)
{
    struct PanData
    {
        PanData():
            m_phPan(0),
            m_ppfInputData(0),
            m_ppfOutputData(0),
            m_iDataLength(35131),
            m_iBlockLength(171),
            m_iNumChannelsInput(0),
            m_iNumChannelsOutput(0),
            m_fSampleRate(8000)
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
    
}

#endif //WITH_TESTS
