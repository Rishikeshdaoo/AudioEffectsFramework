% main function
close all;
clear all;
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

%% delay test

audio_path_raw = 'Guitar_sample.wav';
audio_path_cpp = 'Guitar_sample_delay.wav';

[audio_raw,~] = audioread(audio_path_raw);
[audio_cpp,Fs] = audioread(audio_path_cpp);

% params for distortion: [BL, FB, FF, M]
param_delay = [1, 0, 0.2, 0.8*Fs]; 
[audio_mat_delay, MSE_delay] = test(audio_raw, audio_cpp, "delay", param_delay);

%% compressor test

audio_path_raw = 'Guitar_sample.wav';
audio_path_cpp = 'Guitar_sample_compressor.wav';

[audio_raw,~] = audioread(audio_path_raw);
[audio_cpp,Fs] = audioread(audio_path_cpp);

% params for compressor: [CT, CS, ET, ES]
param_compressor = [-50, 0.5, 0, 0]; 
[audio_mat_compressor, MSE_compressor] = test(audio_raw, audio_cpp, "compressor", param_compressor);


%% expander test

audio_path_raw = 'Guitar_sample.wav';
audio_path_cpp = 'Guitar_sample_expander.wav';

[audio_raw,~] = audioread(audio_path_raw);
[audio_cpp,Fs] = audioread(audio_path_cpp);

% params for expander: [CT, CS, ET, ES]
param_expander = [0, 0, -20, -5]; 
[audio_mat_expander, MSE_expander] = test(audio_raw, audio_cpp, "expander", param_expander);

%% flanger

audio_path_raw = 'Guitar_sample.wav';
audio_path_cpp = 'Guitar_sample_flanger.wav';

[audio_raw,~] = audioread(audio_path_raw);
[audio_cpp,Fs] = audioread(audio_path_cpp);

% params for flanger: [Fs, DelayinSecs, ModRateinHz]
param_flanger = [Fs, 0.002, 0.25]; 
[audio_mat_flanger, MSE_flanger] = test(audio_raw, audio_cpp, "flanger", param_flanger);

% figure;
% plot(audio_cpp);
% hold on;
% plot(audio_mat_flanger);

%% chorus

audio_path_raw = 'Guitar_sample.wav';
audio_path_cpp = 'Guitar_sample_chorus.wav';

[audio_raw,~] = audioread(audio_path_raw);
[audio_cpp,Fs] = audioread(audio_path_cpp);

% params for chorus: [Fs, max_time_delay, modulation_depth, modulation_rate, feedback]
% param_chorus = [Fs, 0.015, 0.01, 0.25, 0.7]; 
param_chorus = [Fs, 0.015, 0.01, 0.5, 0.7]; 
[audio_mat_chorus, MSE_chorus] = test(audio_raw, audio_cpp, "chorus", param_chorus);

figure;
plot(audio_cpp);
hold on;
plot(audio_mat_chorus);

%% Reverb
audio_path_raw = 'Guitar_sample.wav';
audio_path_cpp = 'Guitar_sample_reverb.wav';

[audio_raw,~] = audioread(audio_path_raw);
[audio_cpp,Fs] = audioread(audio_path_cpp);
audio_cpp = audio_cpp/max(audio_cpp);

% params for reverb: [Number of filters, gain of filters, vector of delay lengths, gain factor of the direct signal]
% [y,b,a] = schroeder(audio_raw,3,0.707,[0.5*Fs 0.3*Fs 0.2*Fs],1);
param_reverb = {3,0.707,[0.5*Fs 0.3*Fs 0.2*Fs],1}; 
[audio_mat_reverb, MSE_reverb] = test(audio_raw, audio_cpp, "reverb", param_reverb);
%% other test

% param_chorus = [Fs, 0.013, 0.003, 1, 0.8] % params for chorus effect: [Fs, max_time_delay, modulation_depth, modulation_rate, feedback]
% [audio_mat_chorus, MSE_chorus] = test(audio_raw, audio_cpp, "chorus", param_chorus);
%
% param_reverb = [6, 0.9, floor(0.05*rand([1,n])*Fs), 0.2] % params for reverb effect: [no. of all pass filters,list of gains,list of delay times, gain]
% [audio_mat_reverb, MSE_reverb] = test(audio_raw, audio_cpp, "chorus", param_reverb);
