% main function
addpath('./effects/');

%% biquad test

audio_path_raw = 'Guitar_sample.wav';
audio_path_cpp = 'Guitar_sample_biquad.wav';

[audio_raw,~] = audioread(audio_path_raw);
[audio_cpp,Fs] = audioread(audio_path_cpp);

% params for biquad effect: {type freqvalue qvalue gain Fs}
param_biquad = {'bandpass' 1000 0.707 0.5 Fs}; 
[audio_mat_biquad, MSE_biquad] = test(audio_raw, audio_cpp, "biquad", param_biquad);

%% distortion test

audio_path_raw = 'Guitar_sample.wav';
audio_path_cpp = 'Guitar_sample_distortion.wav';

[audio_raw,~] = audioread(audio_path_raw);
[audio_cpp,Fs] = audioread(audio_path_cpp);

% params for distortion: [gain, mix]
param_distortion = [5, 0.5]; 
[audio_mat_distortion, MSE_distortion] = test(audio_raw, audio_cpp, "distortion", param_distortion);

%% other test

% param_flanger = [Fs, 0.002, 1]; % params for flanger effect: [Fs, max_time_delay, rate]
% [audio_mat_flanger, MSE_flanger] = test(audio_raw, audio_cpp, "flanger", param_flanger);
% 
% param_chorus = [Fs, 0.013, 0.003, 1, 0.8] % params for chorus effect: [Fs, max_time_delay, modulation_depth, modulation_rate, feedback]
% [audio_mat_chorus, MSE_chorus] = test(audio_raw, audio_cpp, "chorus", param_chorus);
% 
% rand(’state’,sum(100*clock))
% param_reverb = [6, 0.9, floor(0.05*rand([1,n])*Fs), 0.2] % params for reverb effect: [no. of all pass filters,list of gains,list of delay times, gain]
% [audio_mat_reverb, MSE_reverb] = test(audio_raw, audio_cpp, "chorus", param_reverb);
% 
% param_comp = [Fs, 0.013, 0.003, 1, 0.8] % params for chorus effect: [Fs, max_time_delay, modulation_depth, modulation_rate, feedback]
% [audio_mat_chorus, MSE_chorus] = test(audio_raw, audio_cpp, "compressor", param_comp);
