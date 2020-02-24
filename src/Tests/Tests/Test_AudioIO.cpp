#include "MUSI6106Config.h"

#ifdef WITH_TESTS
#include <cassert>
#include <cstdio>

#include "UnitTest++.h"

#include "Synthesis.h"

#include "AudioFileIf.h"

extern std::string cTestDataDir;

SUITE(AudioIo)
{
    struct AudioIoData
    {
        AudioIoData() 
        {
            float fPhase    = 0;

            m_stFileSpec.eBitStreamType  = CAudioFileIf::kFileBitStreamInt16;
            m_stFileSpec.eFormat         = CAudioFileIf::kFileFormatRaw;
            m_stFileSpec.fSampleRateInHz = 44100.F;
            m_stFileSpec.iNumChannels    = m_iNumChannels;

            CAudioFileIf::create(m_pCAudioFile);
            m_ppfAudioData  = 0;
            m_ppfTmp        = 0;
            m_ppfAudioData  = new float*[m_iNumChannels];
            m_ppfTmp        = new float*[m_iNumChannels];
            for (int i = 0; i < m_iNumChannels; i++)
                m_ppfAudioData[i]   = new float[m_iBuffLength];

            for (int i = 0; i < m_iNumChannels; i++)
            {
                CSynthesis::generateSine(m_ppfAudioData[i], 441.F, m_stFileSpec.fSampleRateInHz, m_iBuffLength, .6F, fPhase);
                fPhase  += static_cast<float>(M_PI_2);
            }

        }
        
        ~AudioIoData() 
        {
            assert (m_ppfAudioData != 0);
            for (int i = 0; i < m_iNumChannels; i++)
                delete [] m_ppfAudioData[i];
            delete [] m_ppfAudioData;
            delete [] m_ppfTmp;

            CAudioFileIf::destroy(m_pCAudioFile);
       }


        void writeWav()
        {
            const std::string cExt(".wav");
            int iNumRemainingFrames = m_iBuffLength;
            Error_t err = kUnknownError;

            m_stFileSpec.eFormat         = CAudioFileIf::kFileFormatWav;

            err = m_pCAudioFile->openFile (cTestDataDir+"/test"+cExt, CAudioFileIf::kFileWrite, &m_stFileSpec);
            CHECK (err == kNoError);

            // put data
            while (iNumRemainingFrames > 0)
            {
                for (int i = 0; i < m_iNumChannels; i++)
                    m_ppfTmp[i] = &m_ppfAudioData[i][m_iBuffLength - iNumRemainingFrames];
                int iPutFrames = std::min(m_iBlockLength, iNumRemainingFrames);

                m_pCAudioFile->writeData (m_ppfTmp, iPutFrames);

                iNumRemainingFrames -= iPutFrames;
            }
            m_pCAudioFile->closeFile ();
            m_pCAudioFile->reset ();
        }

        void deleteFile (std::string cExt)
        {
            // cleanup: delete file from disk -- permissions problem under win
            remove ((cTestDataDir+"/test"+cExt).c_str());
        }

        float **m_ppfAudioData;
        float **m_ppfTmp;
        CAudioFileIf    *m_pCAudioFile;
        CAudioFileIf::FileSpec_t m_stFileSpec;

        static const int m_iBuffLength;
        static const int m_iBlockLength;
        static const int m_iNumChannels;
    };

    const int AudioIoData::m_iBuffLength  = 1027;
    const int AudioIoData::m_iBlockLength = 17;
    const int AudioIoData::m_iNumChannels = 2;

    TEST_FIXTURE(AudioIoData, FileReadRaw)
    {
        if (cTestDataDir.empty())
        {
            CHECK(false);
        }
        long long int iReadIdx = 0;
        float **ppfReadData  = new float*[m_iNumChannels];
        for (int i = 0; i < m_iNumChannels; i++)
            ppfReadData[i]   = new float[m_iBuffLength];
 
        m_pCAudioFile->openFile (cTestDataDir+"/ref.pcm", CAudioFileIf::kFileRead, &m_stFileSpec);

        while (!m_pCAudioFile->isEof ())
        {
            long long int iNumRead = m_iBlockLength;
            for (int i = 0; i < m_iNumChannels; i++)
                m_ppfTmp[i] = &ppfReadData[i][iReadIdx];

            m_pCAudioFile->readData (m_ppfTmp, iNumRead);
            iReadIdx   += iNumRead;
        }

        // check identity
        for (int i = 0; i < m_iNumChannels; i++)
            CHECK_ARRAY_CLOSE(m_ppfAudioData[i], ppfReadData[i], m_iBuffLength, 1e-3); // succeeds

        // free allocated memory
        for (int i = 0; i < m_iNumChannels; i++)
            delete [] ppfReadData[i];
        delete [] ppfReadData;
    }

    TEST_FIXTURE(AudioIoData, FileReadRawOffset)
    {
        if (cTestDataDir.empty())
        {
            CHECK(false);
            return;
        }
        const long long iOffset = 327;
        long long int iReadIdx = 0;
        float **ppfReadData  = new float*[m_iNumChannels];
        for (int i = 0; i < m_iNumChannels; i++)
            ppfReadData[i]   = new float[m_iBuffLength];

        m_pCAudioFile->openFile (cTestDataDir+"/ref.pcm", CAudioFileIf::kFileRead, &m_stFileSpec);

        m_pCAudioFile->setPosition (iOffset);

        while (!m_pCAudioFile->isEof ())
        {
            long long int iNumRead = m_iBlockLength;
            for (int i = 0; i < m_iNumChannels; i++)
                m_ppfTmp[i] = &ppfReadData[i][iReadIdx];

            m_pCAudioFile->readData (m_ppfTmp, iNumRead);
            iReadIdx   += iNumRead;
        }

        // check identity
        for (int i = 0; i < m_iNumChannels; i++)
            CHECK_ARRAY_CLOSE(&m_ppfAudioData[i][iOffset], ppfReadData[i], m_iBuffLength-iOffset, 1e-3); // succeeds

        // free allocated memory
        for (int i = 0; i < m_iNumChannels; i++)
            delete [] ppfReadData[i];
        delete [] ppfReadData;

    }

    TEST_FIXTURE(AudioIoData, FileReadAllAtOnce)
    {
        if (cTestDataDir.empty())
        {
            CHECK(false);
            return;
        }
       //note that the file length is longer than the internal read block size
        long long   iFileLength = 0;
        long long   iNumRead    = 0;
        float **ppfReadData     = new float*[m_iNumChannels];
        
        for (int i = 0; i < m_iNumChannels; i++)
            ppfReadData[i]      = new float[m_iBuffLength];

        m_pCAudioFile->openFile (cTestDataDir+"/ref.pcm", CAudioFileIf::kFileRead, &m_stFileSpec);
        m_pCAudioFile->getLength (iFileLength);
        iNumRead    = static_cast<int>(iFileLength);

        m_pCAudioFile->readData (ppfReadData, iNumRead);

        // check identity
        CHECK(iNumRead == iFileLength);
        for (int i = 0; i < m_iNumChannels; i++)
            CHECK_ARRAY_CLOSE(m_ppfAudioData[i], ppfReadData[i], m_iBuffLength, 1e-3); // succeeds

        // free allocated memory
        for (int i = 0; i < m_iNumChannels; i++)
            delete [] ppfReadData[i];
        delete [] ppfReadData;
    }

    TEST_FIXTURE(AudioIoData, FileWriteReadRaw)
    {
        if (cTestDataDir.empty())
        {
            CHECK(false);
            return;
        }
        int iNumRemainingFrames = m_iBuffLength;
        Error_t err = kUnknownError;
        
        err = m_pCAudioFile->openFile (cTestDataDir+"/test.pcm", CAudioFileIf::kFileWrite, &m_stFileSpec);
        CHECK (err == kNoError);

        // put data
        while (iNumRemainingFrames > 0)
        {
            for (int i = 0; i < m_iNumChannels; i++)
                m_ppfTmp[i] = &m_ppfAudioData[i][m_iBuffLength - iNumRemainingFrames];
            int iPutFrames = std::min(m_iBlockLength, iNumRemainingFrames);

            m_pCAudioFile->writeData (m_ppfTmp, iPutFrames);

            iNumRemainingFrames -= iPutFrames;
        }
        m_pCAudioFile->closeFile ();
        m_pCAudioFile->reset ();

        // read the file and compare
        long long int iReadIdx = 0;
        float **ppfReadData  = new float*[m_iNumChannels];
        for (int i = 0; i < m_iNumChannels; i++)
            ppfReadData[i]   = new float[m_iBuffLength];

        err = m_pCAudioFile->openFile (cTestDataDir+"/test.pcm", CAudioFileIf::kFileRead, &m_stFileSpec);
        CHECK (err == kNoError);

        while (!m_pCAudioFile->isEof ())
        {
            long long int iNumRead = m_iBlockLength;
            for (int i = 0; i < m_iNumChannels; i++)
                m_ppfTmp[i] = &ppfReadData[i][iReadIdx];

            m_pCAudioFile->readData (m_ppfTmp, iNumRead);
            iReadIdx   += iNumRead;
        }

        // check identity
        for (int i = 0; i < m_iNumChannels; i++)
            CHECK_ARRAY_CLOSE(m_ppfAudioData[i], ppfReadData[i], m_iBuffLength, 1e-3); // succeeds

        // free allocated memory
        for (int i = 0; i < m_iNumChannels; i++)
            delete [] ppfReadData[i];
        delete [] ppfReadData;

        m_pCAudioFile->closeFile ();


        // cleanup: delete file from disk -- permissions problem under win
        deleteFile (".pcm");
        
    }
#ifdef WITH_SNDLIB
    TEST_FIXTURE(AudioIoData, FileWriteReadWav)
    {
       Error_t err = kUnknownError;
       if (cTestDataDir.empty())
        {
            CHECK(false);
            return;
        }
        const std::string cExt(".wav");
        writeWav();

        // read the file and compare
        int iReadIdx = 0;
        float **ppfReadData  = new float*[m_iNumChannels];
        for (int i = 0; i < m_iNumChannels; i++)
            ppfReadData[i]   = new float[m_iBuffLength];

        err = m_pCAudioFile->openFile (cTestDataDir+"/test"+cExt, CAudioFileIf::kFileRead, &m_stFileSpec);
        CHECK (err == kNoError);

        while (!m_pCAudioFile->isEof ())
        {
            int iNumRead = m_iBlockLength;
            for (int i = 0; i < m_iNumChannels; i++)
                m_ppfTmp[i] = &ppfReadData[i][iReadIdx];

            m_pCAudioFile->readData (m_ppfTmp, iNumRead);
            iReadIdx   += iNumRead;
        }

        // check identity
        for (int i = 0; i < m_iNumChannels; i++)
            CHECK_ARRAY_CLOSE(m_ppfAudioData[i], ppfReadData[i], m_iBuffLength, 1e-3); // succeeds

        // free allocated memory
        for (int i = 0; i < m_iNumChannels; i++)
            delete [] ppfReadData[i];
        delete [] ppfReadData;

        m_pCAudioFile->closeFile ();
        
        // cleanup
        deleteFile(cExt);

    }

    TEST_FIXTURE(AudioIoData, FileSpec)
    {
        if (cTestDataDir.empty())
        {
            CHECK(false);
            return;
        }
        CAudioFileIf::FileSpec_t stFileSpec;
        const std::string cExt(".wav");
        Error_t err = kUnknownError;

        writeWav();

        err = m_pCAudioFile->openFile (cTestDataDir+"/test"+cExt, CAudioFileIf::kFileRead);
        CHECK (err == kNoError);

        m_pCAudioFile->getFileSpec (stFileSpec);

        CHECK_EQUAL(44100, stFileSpec.fSampleRateInHz);
        CHECK_EQUAL(CAudioFileIf::kFileFormatWav, stFileSpec.eFormat);

        m_pCAudioFile->closeFile ();
        
        // cleanup
        deleteFile(cExt);
    }

#endif // WITH_SNDLIB
}

#endif //WITH_TESTS