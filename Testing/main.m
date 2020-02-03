%% Audio from C++ code
% audio_path = ' ';
% Filtered_audio = audioread(audio_path, Fs);

%% MATLAB implementation
[audio,Fs] = audioread('Guitar_sample.wav');


% distorted = gdist(0.9, audio);
% comp = unicomb(audio, 0.5, -0.5, 1, 10000);
% del = delay(audio,10);

%set the number of allpass filters
n = 6;
%set the gain of the allpass filters
g = 0.9;
%set delay of each allpass filter in number of samples
%Compute a random set of milliseconds and use sample rate
% rand(?state?,sum(100*clock))
d = floor(0.05*50000*rand([1,n])*Fs);
%set gain of direct signal
k= 0.2;
% [y, b, a] = schroeder1(audio,n,g,d,k);
flange = flanger(audio,Fs)

sound(flange, Fs)


