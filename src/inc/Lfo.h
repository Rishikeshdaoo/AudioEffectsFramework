#if !defined(__Lfo_hdr__)
#define __Lfo_hdr__

#define _USE_MATH_DEFINES
#include <math.h>

#include "ErrorDef.h"
#include "RingBuffer.h"
#include "Synthesis.h"

class CLfo
{
public:
    CLfo(float fSampleRate) :
    m_fSampleRate(fSampleRate),
    m_fReadIndex(0),
    m_eType(kSine),
    m_pCRingBuff(0)
    {
        for (int i = 0; i < kNumLfoParams; i++)
            m_afParam[i]    = 0;
        
        m_pCRingBuff = new CRingBuffer<float>(m_kiBufferLength);
        
        setLfoType(kSine);
    }
    virtual ~CLfo()
    {
        delete m_pCRingBuff;
    }
    
    enum LfoType_t
    {
        kSine,
        kSaw,
        kRect,
        
        kNumLfoTypes
    };
    enum LfoParam_t
    {
        kLfoParamAmplitude,
        kLfoParamFrequency,
        
        kNumLfoParams
    };
    Error_t setLfoType (LfoType_t eType)
    {
        m_eType = eType;
        computeWaveTable();
        return kNoError;
    }
    Error_t setParam(LfoParam_t eParam, float fValue)
    {
        m_afParam[eParam]   = fValue;
        
        return kNoError;
    }
    float getParam (LfoParam_t eParam) const
    {
        return m_afParam[eParam];
    }
    
    float getNext()
    {
        float fValue = m_afParam[kLfoParamAmplitude] * m_pCRingBuff->get(m_fReadIndex);
        m_fReadIndex = m_fReadIndex + m_afParam[kLfoParamFrequency]/m_fSampleRate * m_kiBufferLength;
        
        if (m_fReadIndex >= m_kiBufferLength)
            m_fReadIndex -= m_kiBufferLength;
        return fValue;
    }
private:
    void computeWaveTable ()
    {
        float *pfBuff = new float [m_kiBufferLength];
        switch (m_eType)
        {
            case kSine:
                CSynthesis::generateSine (pfBuff, 1.F, 1.F*m_kiBufferLength, m_kiBufferLength);
                break;
            case kSaw:
                CSynthesis::generateSaw (pfBuff, 1.F, 1.F*m_kiBufferLength, m_kiBufferLength);
                break;
            case kRect:
                CSynthesis::generateRect (pfBuff, 1.F, 1.F*m_kiBufferLength, m_kiBufferLength);
                break;
        }
        
        m_pCRingBuff->put(pfBuff, m_kiBufferLength);
        
        delete [] pfBuff;
    }
    static const int m_kiBufferLength = 4096;
    
    float m_fSampleRate;
    float m_fReadIndex;
    float m_afParam[kNumLfoParams];
    
    LfoType_t m_eType;
    
    CRingBuffer<float> *m_pCRingBuff;
};
#endif // __Lfo_hdr__
