function [y] = flanger(x, Fs, max_time_delay, rate)

%This is a flanger effect. Creates a single FIR delay with the delay time oscillating from
% Either 0-3 ms or 0-15 ms at 0.1 - 5 Hz
%
%The structure is:  [y] = flanger(audio_tset, x,Fs, max_time_delay, rate)
%
%where audio_test = filtered audio from C++ code 
%      x = the input signal 
%      Fs = Sampling rate 
%      max_time_delay = delay time in seconds
%      rate = rate of flange in Hz
%
% Chapter 7: Digital Audio Effects by Prof David Marshall and Dr Kirill Sidorov, 
% Cardiff School of Computer Science.

index=1:length(x);
% sin reference to create oscillating delay
sin_ref = (sin(2*pi*index*(rate/Fs)))';
%convert delay in ms to max delay in samples
max_samp_delay=round(max_time_delay*Fs);
% create empty output vector
y = zeros(length(x),1);
% to avoid referencing of negative samples
y(1:max_samp_delay)=x(1:max_samp_delay);
% set amp suggested coefficient from page 71 DAFX
amp=0.7;
% for each sample
for i = (max_samp_delay+1):length(x)
cur_sin=abs(sin_ref(i)); %abs of current sin val 0-1
% generate delay from 1-max_samp_delay and ensure whole number
cur_delay=ceil(cur_sin*max_samp_delay);
% add delayed sample
y(i) = (amp*x(i)) + amp*(x(i-cur_delay));
end