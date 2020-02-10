% main function

%% flanger test

audio_path_raw = 'Guitar_sample.wav';
audio_path_cpp = 'Guitar_sample.wav'; % same as raw now

[audio_raw,Fs] = audioread(audio_path_raw);
[audio_cpp,Fs] = audioread(audio_path_cpp);

param_flanger = [Fs, 0.002, 1]; % params for flanger effect: [Fs, max_time_delay, rate]
[audio_mat_flanger, MSE_flanger] = test(audio_raw, audio_cpp, "flanger", param_flanger);

%% other test
