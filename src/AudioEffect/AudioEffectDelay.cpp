//
// Created by Rishikesh Daoo on 2/15/20.
//  Copyright © 2020 Rishikesh Daoo. All rights reserved.
//

#include "AudioEffectDelay.h"

#include "RingBuffer.h"
#include "Lfo.h"
#include <math.h>

CAudioEffectDelay::CAudioEffectDelay()
{
    m_eEffectType = Effect_t::kDelay;
    m_fGain = 0.f;
    m_fDelay = 0.f;
    m_iNumChannels = 0;
    m_fSampleRateInHz = 0;
    m_bIsInitialized = false;
    m_ppCRingBuffer = 0;
    m_fMaxDelay = 0;
};

CAudioEffectDelay::CAudioEffectDelay(float fSampleRateInHz, int iNumChannels, float iMaxDelayInSec, EffectParam_t params[] = NULL, float values[] = NULL, int iNumParams = 0)
{
    m_eEffectType = Effect_t::kDelay;
    init(fSampleRateInHz, iNumChannels, iMaxDelayInSec, params, values, iNumParams);
};


CAudioEffectDelay::~CAudioEffectDelay()
{

};

Error_t CAudioEffectDelay::init(float fSampleRateInHz, int iNumChannels, float iMaxDelayInSec, EffectParam_t params[], float values[], int iNumParams)
{
    m_fSampleRateInHz = fSampleRateInHz;
    m_iNumChannels = iNumChannels;

    m_fMaxDelay = iMaxDelayInSec * m_fSampleRateInHz;
    
    m_pCLfo             = new CLfo(m_fSampleRateInHz);

    m_ppCRingBuffer = new CRingBuffer<float>*[m_iNumChannels];
//    for (int c = 0; c < m_iNumChannels; c++)
//        m_ppCRingBuffer[c]  = new CRingBuffer<float>(iMaxDelayInSec * m_fSampleRateInHz);
    for (int c= 0; c < m_iNumChannels; c++)
    {
        m_ppCRingBuffer[c]= new CRingBuffer<float>(CUtil::float2int<int>(iMaxDelayInSec*m_fSampleRateInHz*2+1));
        m_ppCRingBuffer[c]->setWriteIdx(CUtil::float2int<int>(iMaxDelayInSec*m_fSampleRateInHz+1));
        m_ppCRingBuffer[c]->setReadIdx(CUtil::float2int<int>(iMaxDelayInSec*m_fSampleRateInHz+1));
    }

    for (int i = 0; i < iNumParams; i++)
    {
        switch (params[i]) {
            case kParamGain:
                m_fGain = values[i];
                break;
            case kParamDelayInSecs:
                m_fDelay = values[i] * m_fSampleRateInHz;

                if(m_fDelay > m_fMaxDelay)
                    return kFunctionInvalidArgsError;

                for (int c = 0; c < m_iNumChannels; c++) {
                    for (int i = 0; i < m_fDelay; i++) {
                        m_ppCRingBuffer[c]->putPostInc(0.F);
                    }
                }
                break;
            case kParamModRateInHz :
                m_fModFreq = m_pCLfo->setParam(CLfo::kLfoParamFrequency, values[i]);
                break;
            case kParamModWidthInSecs :
                if(kParamModWidthInSecs > m_fDelay)
                    return kFunctionInvalidArgsError;
                m_fModWidthinSamps = m_pCLfo->setParam(CLfo::kLfoParamAmplitude, values[i] * m_fSampleRateInHz);
                break;
            
            default:
                return kNoError;
        }
    }

    m_bIsInitialized = true;

    return kNoError;
};

Error_t CAudioEffectDelay::reset()
{
    for (int c= 0; c < m_iNumChannels; c++)
        delete m_ppCRingBuffer[c];
    delete [] m_ppCRingBuffer;
    m_ppCRingBuffer       = 0;
    delete m_pCLfo;
    m_pCLfo             = 0;
    
    m_iNumChannels      = 0;
    m_bIsInitialized    = false;
    
    return kNoError;
};

Error_t CAudioEffectDelay::setParam(EffectParam_t eParam, float fValue)
{
    if (!m_bIsInitialized)
        return kNotInitializedError;

    switch (eParam) {
        case kParamGain:
            m_fGain = fValue;
            break;
        case kParamDelayInSecs:
            m_fDelay = fValue * m_fSampleRateInHz;

            if(m_fDelay > m_fMaxDelay)
                return kFunctionInvalidArgsError;

            for (int c = 0; c < m_iNumChannels; c++) {
                for (int i = 0; i < m_fDelay; i++) {
                    m_ppCRingBuffer[c]->putPostInc(0.F);
                }
            }
            break;

        case kParamModRateInHz:
            m_pCLfo->setParam(CLfo::kLfoParamFrequency, fValue);
            break;
        case kParamModWidthInSecs:
            if(kParamModWidthInSecs > m_fDelay)
                    return kFunctionInvalidArgsError;
            m_pCLfo->setParam(CLfo::kLfoParamAmplitude, fValue * m_fSampleRateInHz);
            break;


    
        default:
            return kFunctionInvalidArgsError;
    }

    return kNoError;
};

float CAudioEffectDelay::getParam(EffectParam_t eParam)
{
    if (!m_bIsInitialized)
        return kNotInitializedError;

    switch (eParam) {
        case kParamGain:
            return m_fGain;
        case kParamDelayInSecs:
            return m_fDelay;

            break;
        case kParamModRateInHz:
            return m_pCLfo->getParam(CLfo::kLfoParamFrequency);
            break;
        case kParamModWidthInSecs:
            return m_pCLfo->getParam(CLfo::kLfoParamAmplitude) / m_fSampleRateInHz;
            break;
        default:
            return 0.f;
            break;
    }
    
    
};

Error_t CAudioEffectDelay::setDelayType(DelayType_t eValue)
{
    if (!m_bIsInitialized)
        return kNotInitializedError;
    
    m_eDelayType = eValue;
    switch (m_eDelayType) {
        case kDelay:
            m_fModGain = 0;
            break;
        case kFlanger:
            m_fModGain = 1;
            
        default:
            break;
    }

    
    return kNoError;
};

CAudioEffectDelay::DelayType_t CAudioEffectDelay::getDelayType()
{
    return m_eDelayType;
};


Error_t CAudioEffectDelay::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
{
//    int count = 0;
//    m_fModGain = 1;
    for(int i = 0; i < iNumberOfFrames; i++)
    {
        
        float fOffset = m_pCLfo->getNext()*m_fModGain;
//        if (count % 1000 == 0)
//        {
//            std::cout << iOffset << std::endl;
//        }
//        int iCurrentDelay = ceil(m_fDelay + (fOffset * m_fModGain));
        
        for (int c = 0; c < m_iNumChannels; c++)
        {
            m_ppCRingBuffer[c]->putPostInc(ppfInputBuffer[c][i]);
//            ppfOu tputBuffer[c][i] = ppfInputBuffer[c][i] + m_fGain * m_ppCRingBuffer[c]->getPostInc(fCurrentDelay);
//            m_ppCRingBuffer[c]->putPostInc(ppfOutputBuffer[c][i]);
            ppfOutputBuffer[c][i]   = ppfInputBuffer[c][i]*m_fGain + m_ppCRingBuffer[c]->get(fOffset)*m_fGain;
            m_ppCRingBuffer[c]->getPostInc();
        }
    }
    return kNoError;
};



