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
% interpolation method different

audio_path_raw = 'Guitar_sample.wav';
audio_path_cpp = 'Guitar_sample_flanger.wav';

[audio_raw,~] = audioread(audio_path_raw);
[audio_cpp,Fs] = audioread(audio_path_cpp);

% params for flanger: [Fs, DelayinSecs, ModRateinHz]
param_flanger = [Fs, 0.002, 0.25]; 
[audio_mat_flanger, MSE_flanger] = test(audio_raw, audio_cpp, "flanger", param_flanger);

%% Reverb
audio_path_raw = 'Guitar_sample.wav';
audio_path_cpp = 'Guitar_sample_reverb.wav';

[audio_raw,~] = audioread(audio_path_raw);
[audio_cpp,Fs] = audioread(audio_path_cpp);
% audio_cpp = audio_cpp/max(audio_cpp);

% params for reverb: [Number of filters, gain of filters, 
%                     vector of delay lengths, gain factor of the direct signal]
% [y,b,a] = schroeder(audio_raw,3,0.707,[0.5*Fs 0.3*Fs 0.2*Fs],1);
param_reverb = {3,0.707,[0.5*Fs 0.3*Fs 0.2*Fs],1}; 
[audio_mat_reverb, MSE_reverb] = test(audio_raw, audio_cpp, "reverb", param_reverb);

audiowrite("reverb_matlab.wav", audio_mat_reverb, Fs);
audiowrite("reverb_cpp.wav", audio_cpp, Fs);