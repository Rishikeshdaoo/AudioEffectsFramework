
#include <cassert>
#include <iostream>

#include "Util.h"

#include "AudioFileIf.h"
#include "AudioFile.h"

using namespace std;

// consts
const long long int CAudioFileIf::m_kiDefBlockLength      = 1024;

Error_t CAudioFileIf::create( CAudioFileIf*& pCInstance )
{
#ifdef WITH_SNDLIB
    pCInstance   = new CAudioFileSndLib ();
#else
    pCInstance   = new CAudioFileRaw ();
#endif

    if (!pCInstance)
        return kMemError;

    return kNoError;
}

Error_t CAudioFileIf::destroy( CAudioFileIf*& pCInstance )
{
    delete pCInstance;
    pCInstance  = 0;

    return kNoError;
}

CAudioFileIf::CAudioFileIf() : 
    m_eIoType(kFileRead),
    m_bWithClipping(true),
    m_bIsInitialized(false),
    m_iNumBytesPerSample(2)
{
        this->initDefaults();
}

CAudioFileIf::~CAudioFileIf()
{
}

Error_t CAudioFileIf::reset( bool bFreeMemory /*= false*/ )
{
    Error_t eErr = closeFile ();
    if (eErr != kNoError)
        return eErr;

    if (bFreeMemory)
    {
        eErr = freeMemory ();
        if (eErr != kNoError)
            return eErr;
    }
  
    eErr = initDefaults ();
    if (eErr != kNoError)
        return eErr;

    return eErr;
}

Error_t CAudioFileIf::freeMemory()
{
    return kNoError;
}

Error_t CAudioFileIf::allocMemory()
{
    freeMemory ();

    return kNoError;
}

Error_t CAudioFileIf::initDefaults()
{
    m_CurrFileSpec.eBitStreamType  = kFileBitStreamInt16;
    m_CurrFileSpec.eFormat         = kFileFormatRaw;
    m_CurrFileSpec.fSampleRateInHz = 48000;
    m_CurrFileSpec.iNumChannels    = 2;

    setIoType(kFileRead);

    setInitialized(false);
    setClippingEnabled ();

    return kNoError;
}

Error_t CAudioFileIf::readData( float **ppfAudioData, long long int &iLength )
{
    // check parameters
    if (!ppfAudioData || !ppfAudioData[0] || iLength < 0)
        return kFunctionInvalidArgsError;

    // check file status
    if (!isOpen())
        return kUnknownError;

    // check file properties
    if (!isInitialized())
        return kNotInitializedError;

    // update iLength to the number of frames actually read
    iLength = readDataIntern (ppfAudioData, iLength);
    if (iLength < 0)
        return kFileAccessError;

    return kNoError;
}

Error_t CAudioFileIf::writeData( float **ppfAudioData, long long int iLength )
{

    // check parameters
    if (!ppfAudioData || !ppfAudioData[0] || iLength < 0)
        return kFunctionInvalidArgsError;

    // check file status
    if (!isOpen())
        return kUnknownError;

    // check file properties
    if (!isInitialized())
        return kNotInitializedError;

    // update iLength
    iLength = writeDataIntern (ppfAudioData, iLength);
    if (iLength < 0)
        return kFileAccessError;

    return kNoError;
}

long long CAudioFileIf::convFrames2Bytes( long long iNumFrames )
{
    return m_iNumBytesPerSample*iNumFrames*getNumChannels();
}

long long CAudioFileIf::convBytes2Frames( long long iNumBytes )
{
    return iNumBytes/(m_iNumBytesPerSample * getNumChannels());    
}

Error_t CAudioFileIf::getFileSpec( FileSpec_t &sFileSpec )
{
    memcpy (&sFileSpec, &m_CurrFileSpec, sizeof(FileSpec_t));

    return kNoError;
}

Error_t CAudioFileIf::setClippingEnabled( bool bIsEnabled /*= true*/ )
{
    m_bWithClipping = bIsEnabled;
    return kNoError;
}

Error_t CAudioFileIf::setPosition( long long iFrame /*= 0*/ )
{
    // check file status
    if (!isOpen())
        return kUnknownError;

    // check file properties
    if (!isInitialized())
        return kNotInitializedError;

    if (iFrame < 0 || iFrame >= getLengthIntern())
        return kFunctionInvalidArgsError;

    return setPositionIntern(iFrame);

}

Error_t CAudioFileIf::setPosition( double dTimeInS /*= .0*/ )
{
    long long iPosInFrames = CUtil::double2int<long long>(dTimeInS * m_CurrFileSpec.fSampleRateInHz);

    return setPosition (iPosInFrames);
}

Error_t CAudioFileIf::getLength( long long &iLengthInFrames ) 
{
    iLengthInFrames = -1;

    // check file status
    if (!isOpen())
        return kUnknownError;

    // check file properties
    if (!isInitialized())
        return kNotInitializedError;

    iLengthInFrames = getLengthIntern ();

    return kNoError;
}

Error_t CAudioFileIf::getPosition( long long &iFrame )
{
    // check file status
    if (!isOpen())
        return kUnknownError;

    // check file properties
    if (!isInitialized())
        return kNotInitializedError;

    iFrame = getPositionIntern();

    return kNoError;
}

Error_t CAudioFileIf::getPosition( double &dTimeInS )
{
    long long iFrame;

    // check file status
    if (!isOpen())
        return kUnknownError;

    // check file properties
    if (!isInitialized())
        return kNotInitializedError;

    dTimeInS = -1.;
    Error_t iErr = getPosition(iFrame);

    if (iErr != kNoError)
        return iErr;

    dTimeInS = iFrame * (1./m_CurrFileSpec.fSampleRateInHz);
    return kNoError;
}

Error_t CAudioFileIf::getLength( double &dLengthInSeconds ) 
{
    long long iLengthInFrames;
    dLengthInSeconds = -1.;
    Error_t iErr = getLength (iLengthInFrames);

    if (iErr != kNoError)
        return iErr;

    dLengthInSeconds = iLengthInFrames * (1./m_CurrFileSpec.fSampleRateInHz);
    return kNoError;
}

bool CAudioFileIf::isInitialized()
{
    return m_bIsInitialized;
}

Error_t CAudioFileIf::setInitialized( bool bInitialized /*= true*/ )
{
    m_bIsInitialized    = bInitialized;

    return kNoError;
}

Error_t CAudioFileIf::setIoType( FileIoType_t eIoType )
{
    m_eIoType   = eIoType;
    return kNoError;
}

CAudioFileIf::FileIoType_t CAudioFileIf::getIoType() const
{
    return m_eIoType;
}

Error_t CAudioFileIf::setFileSpec( const FileSpec_t *pFileSpec )
{
    memcpy (&m_CurrFileSpec, pFileSpec, sizeof (FileSpec_t));
    return kNoError;
}

int CAudioFileIf::getNumChannels() const
{
    return m_CurrFileSpec.iNumChannels;
}

Error_t CAudioFileIf::setNumBytesPerSample( int iNumBytes )
{
    assert (iNumBytes > 0);
    m_iNumBytesPerSample    = iNumBytes;
    return kNoError;
}

int CAudioFileIf::getNumBytesPerSample() const
{
    return m_iNumBytesPerSample;
}
