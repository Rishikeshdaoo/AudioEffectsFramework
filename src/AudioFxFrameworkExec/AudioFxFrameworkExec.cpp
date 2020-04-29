#include <iostream>
#include <ctime>

#include "AudioFileIf.h"
#include "AudioEffectPan.h"
#include "AudioEffectGain.h"
#include "AudioEffectDistortion.h"
#include "AudioEffectCompressor.h"
#include "AudioEffectBiquad.h"
#include "AudioEffectReverb.h"
#include <AudioEffectDelay.h>

using std::cout;
using std::endl;

// local function declarations
void showClInfo ();
void testCases();
static void show_usage(char *argv);

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,                 //!< file paths
                            sOutputFilePath;

    static const int        kBlockSize = 1024;

    clock_t                 time = 0;

    float                   **ppfAudioInput = 0,
                            **ppfAudioOutput = 0;

    CAudioFileIf            *phAudioInputFile = 0,
                            *phAudioOutputFile = 0;
    
    CAudioEffectGain                *phAudioEffectGain = 0;
    CAudioEffectPan                 *phAudioEffectPan = 0;
    CAudioEffectDistortion          *phAudioEffectDistortion = 0;
    CAudioEffectCompressorExpander  *phAudioEffectCompressorExpander = 0;
    CAudioEffectBiquad              *phAudioEffectBiquad = 0;
    CAudioEffectReverb              *phAudioEffectReverb = 0;
    CAudioEffectDelay               *phAudioEffectDelay = 0;

    std::fstream             hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;

    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments (not sure if needed)
    std::string argEffect;
    
    //setting file path within exe
//    sInputFilePath = "/Users/yitingzhang/Desktop/Guitar_sample.wav";
//    sOutputFilePath = "/Users/yitingzhang/Desktop/Guitar_sample_test.wav";
    
    //command line parser
    if (argc < 7)
    {
        std::string arg = argv[1];
        if ((arg == "-h") || (arg == "--help")) {
            show_usage(argv[0]);
            return 0;
        }else{
        cout << "not enough arguments" << endl;
        show_usage(argv[0]);
        return -1;
        }
//        sInputFilePath = "/Users/yitingzhang/Desktop/MUSI 6106/AudioEffectsFramework/Testing/Guitar_sample.wav";
//        sOutputFilePath = "/Users/yitingzhang/Desktop/MUSI 6106/AudioEffectsFramework/Testing/Guitar_sample_test.wav";
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (i + 1 == argc){
            cout << "Invalid input arguments." << endl;
            show_usage(argv[0]);
            return -1;
        }
        if ((arg == "-i") || (arg == "--input")) {
            sInputFilePath = argv[++i];
        } else if ((arg == "-o") || (arg == "--output")) {
            sOutputFilePath = argv[++i];
        } else if ((arg == "-t") || (arg == "--type")) {
            argEffect = argv[++i];
            if(argEffect != "gain" && argEffect != "pan" && argEffect != "distortion" && argEffect != "compressor" &&
               argEffect != "biquad" && argEffect != "reverb" && argEffect != "delay"){
                cout << "Invalid effect type!" << endl;
                show_usage(argv[0]);
                return -1;
            }
        }
        else{
            cout << "Invalid input arguments." << endl;
            show_usage(argv[0]);
            return -1;
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    // Creating all instances and opening input file
    CAudioFileIf::create(phAudioInputFile);
    CAudioFileIf::create(phAudioOutputFile);

    phAudioInputFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    if (!phAudioInputFile->isOpen())
    {
        cout << "Wave file open error!";
        return -1;
    }
    phAudioInputFile->getFileSpec(stFileSpec);

    //////////////////////////////////////////////////////////////////////////////
    // open the output audio file
    phAudioOutputFile->openFile(sOutputFilePath, CAudioFileIf::kFileWrite, &stFileSpec);
    if (!phAudioOutputFile->isOpen()) {
        cout << "Wave file open error!";
        return -1;
    }

    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    ppfAudioInput = new float*[stFileSpec.iNumChannels];
    ppfAudioOutput = new float*[stFileSpec.iNumChannels];

    for (int i = 0; i < stFileSpec.iNumChannels; i++){
        ppfAudioInput[i] = new float[kBlockSize];
        ppfAudioOutput[i] = new float[kBlockSize];
    }
    time = clock();

    //////////////////////////////////////////////////////////////////////////////
    // Initializing the  effect
    
    //setting parameters in exe
//    argEffect = "gain";
//    int iNumParams = 1;
//    CAudioEffect::EffectParam_t param[iNumParams];
//    float value[iNumParams];
//    param[0] = CAudioEffect::kParamGain;
//    value[0] = 0.5f;
//    phAudioEffectGain = new CAudioEffectGain(stFileSpec.fSampleRateInHz,stFileSpec.iNumChannels, param,value,iNumParams);
    
    //setting parameters in command line
    if(argEffect=="gain"){
        cout << "Effect type: gain" << endl;
        phAudioEffectGain = new CAudioEffectGain(stFileSpec.fSampleRateInHz,stFileSpec.iNumChannels);
    }else if(argEffect=="pan"){
        cout << "Effect type: pan" << endl;
        phAudioEffectPan = new CAudioEffectPan(stFileSpec.fSampleRateInHz,stFileSpec.iNumChannels);
    }else if(argEffect=="distortion"){
        cout << "Effect type: distortion" << endl;
        phAudioEffectDistortion = new CAudioEffectDistortion(stFileSpec.fSampleRateInHz,stFileSpec.iNumChannels);
    }else if(argEffect=="compressor"){
        cout << "Effect type: compressor" << endl;
        phAudioEffectCompressorExpander = new CAudioEffectCompressorExpander(kCompressorExpander, stFileSpec.fSampleRateInHz, stFileSpec.iNumChannels, CAudioEffectCompressorExpander::kCompressor);
    }else if(argEffect=="biquad"){
        cout << "Effect type: biquad" << endl;
        int iNumParams = 3;
        CAudioEffect::EffectParam_t param[iNumParams];
        float value[iNumParams];
        param[0] = CAudioEffect::kParamGain;
        value[0] = 0.5f;
        param[1] = CAudioEffect::kParamCenterFrequency;
        value[1] = 1000.0f;
        param[2] = CAudioEffect::kParamQ;
        value[2] = 0.707f;
        phAudioEffectBiquad = new CAudioEffectBiquad(stFileSpec.fSampleRateInHz,stFileSpec.iNumChannels,CAudioEffectBiquad::FilterType_t::kBandpass,param,value,iNumParams,5.F);
    }else if(argEffect=="reverb"){
        cout << "Effect type: reverb" << endl;
            int iNumParams = 2;
            CAudioEffect::EffectParam_t param[iNumParams];
            float value[iNumParams];
            param[0] = CAudioEffect::kParamNumFilters;
            value[0] = 3;
            param[1] = CAudioEffect::kParamFilterGains;
            value[1] = 0.707;
            phAudioEffectReverb = new CAudioEffectReverb(stFileSpec.fSampleRateInHz,stFileSpec.iNumChannels,1.F,param,value,iNumParams);
            float a_fFilterDelays[3] = {0.5f, 0.3f, 0.2f};
            phAudioEffectReverb->setFilterDelays(a_fFilterDelays, 3);
    }else if(argEffect=="delay"){
        cout << "Effect type: delay" << endl;
        phAudioEffectDelay = new CAudioEffectDelay(stFileSpec.fSampleRateInHz,stFileSpec.iNumChannels,1.F,
                                                    CAudioEffectDelay::kBasicDelay);
    }

    
    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output file

    cout << "\r" << "reading and writing" << endl;
    
    while (!phAudioInputFile->isEof())
    {
        long long iNumFrames = kBlockSize;
        phAudioInputFile->readData(ppfAudioInput,iNumFrames);

        // Call to the process function; the effect implementation function
        if(argEffect=="gain"){
            Error_t error = phAudioEffectGain->process(ppfAudioInput, ppfAudioOutput, iNumFrames);
            if(error != kNoError){
                return -1;
            }
        }else if(argEffect=="pan"){
            Error_t error = phAudioEffectPan->process(ppfAudioInput, ppfAudioOutput, iNumFrames);
            if(error != kNoError){
                return -1;
            }
        }else if(argEffect=="distortion"){
            Error_t error = phAudioEffectDistortion->process(ppfAudioInput, ppfAudioOutput, iNumFrames);
            if(error != kNoError){
                return -1;
            }
        }else if(argEffect=="compressor"){
            Error_t error = phAudioEffectCompressorExpander->process(ppfAudioInput, ppfAudioOutput, iNumFrames);
            if(error != kNoError){
                return -1;
            }
        }else if(argEffect=="biquad"){
            Error_t error = phAudioEffectBiquad->process(ppfAudioInput, ppfAudioOutput, iNumFrames);
            if(error != kNoError){
                return -1;
            }
        }else if(argEffect=="reverb"){
            Error_t error = phAudioEffectReverb->process(ppfAudioInput, ppfAudioOutput, iNumFrames);
            if(error != kNoError){
                return -1;
            }
        }else if(argEffect=="delay"){
            Error_t error = phAudioEffectDelay->process(ppfAudioInput, ppfAudioOutput, iNumFrames);
            if(error != kNoError){
                return -1;
            }
        }
        

        phAudioOutputFile->writeData(ppfAudioOutput, iNumFrames);
    }

    cout << "\nreading/writing done in: \t" << (clock() - time)*1.F / CLOCKS_PER_SEC << " seconds." << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    CAudioFileIf::destroy(phAudioInputFile);
    CAudioFileIf::destroy(phAudioOutputFile);
    
        delete phAudioEffectGain;
        phAudioEffectGain = 0;
        delete phAudioEffectPan;
        phAudioEffectPan = 0;
        delete phAudioEffectDistortion;
        phAudioEffectDistortion = 0;
        delete phAudioEffectCompressorExpander;
        phAudioEffectCompressorExpander = 0;
        delete phAudioEffectBiquad;
        phAudioEffectBiquad = 0;
        delete phAudioEffectReverb;
        phAudioEffectReverb = 0;
        delete phAudioEffectDelay;
        phAudioEffectDelay = 0;


    for (int i = 0; i < stFileSpec.iNumChannels; i++){
        delete[] ppfAudioInput[i];
        delete[] ppfAudioOutput[i];
    }

    delete[] ppfAudioInput;
    delete[] ppfAudioOutput;
    ppfAudioInput = 0;
    ppfAudioOutput = 0;

    return 0;
}

void     showClInfo()
{
    cout << "GTCMT MUSI6106 Project - Audio Effects Framework" << endl;
    cout << "(c) 2020-2020 " << endl;
    cout  << endl;

    return;
}

static void show_usage(char *argv)
{
    std::cerr << "Usage: " << argv[0] << " <option(s)> "
    << "Options:\n"
    << "\t-h,--help\tShow help message\n"
    << "\t-i,--input\tInput wave file\n"
    << "\t-o,--output\tOutput wave file\n"
    << "\t-t,--effect effect types: gain\n"
    << std::endl;
}
