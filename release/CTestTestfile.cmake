# CMake generated Testfile for 
# Source directory: /Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework
# Build directory: /Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/release
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(AudioIo "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/bin/TestExec" "AudioIo" "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/src/Tests/TestData/")
set_tests_properties(AudioIo PROPERTIES  _BACKTRACE_TRIPLES "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/CMakeLists.txt;64;add_test;/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/CMakeLists.txt;0;")
add_test(AudioEffectGain "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/bin/TestExec" "AudioEffectGain")
set_tests_properties(AudioEffectGain PROPERTIES  _BACKTRACE_TRIPLES "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/CMakeLists.txt;65;add_test;/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/CMakeLists.txt;0;")
add_test(AudioEffectPan "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/bin/TestExec" "AudioEffectPan")
set_tests_properties(AudioEffectPan PROPERTIES  _BACKTRACE_TRIPLES "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/CMakeLists.txt;66;add_test;/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/CMakeLists.txt;0;")
add_test(AudioEffectDelay "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/bin/TestExec" "AudioEffectDelay")
set_tests_properties(AudioEffectDelay PROPERTIES  _BACKTRACE_TRIPLES "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/CMakeLists.txt;67;add_test;/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/CMakeLists.txt;0;")
add_test(AudioEffectCompressor "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/bin/TestExec" "AudioEffectCompressor")
set_tests_properties(AudioEffectCompressor PROPERTIES  _BACKTRACE_TRIPLES "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/CMakeLists.txt;68;add_test;/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/CMakeLists.txt;0;")
add_test(AudioEffectBiquad "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/bin/TestExec" "AudioEffectBiquad")
set_tests_properties(AudioEffectBiquad PROPERTIES  _BACKTRACE_TRIPLES "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/CMakeLists.txt;69;add_test;/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/CMakeLists.txt;0;")
add_test(AudioEffectDistortion "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/bin/TestExec" "AudioEffectDistortion")
set_tests_properties(AudioEffectDistortion PROPERTIES  _BACKTRACE_TRIPLES "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/CMakeLists.txt;70;add_test;/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/CMakeLists.txt;0;")
add_test(AudioEffectReverb "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/bin/TestExec" "AudioEffectReverb")
set_tests_properties(AudioEffectReverb PROPERTIES  _BACKTRACE_TRIPLES "/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/CMakeLists.txt;71;add_test;/Users/marketinggramusic/Documents/Sem2/AudioSoftwareEngg/FinaleProjecto/AudioEffectsFramework/CMakeLists.txt;0;")
subdirs("AudioFxFrameworkExec")
subdirs("AudioEffect")
subdirs("AudioFileIO")
subdirs("TestExec")
