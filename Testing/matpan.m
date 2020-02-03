% matpan.m
% script based on DAFZ p 140 to
% perform matrix based panning of mono sound to stereo

close all;
clear all;

% read the sample waveform
filename='Guitar_sample.wav';
% [monox,Fs,bits] = wavread(filename);
[monox, Fs] = audioread(filename);

initial_angle = -40; %in degrees
final_angle = 40;    %in degrees
segments = 32;
angle_increment = (initial_angle - final_angle)/segments * pi / 180;
                % in radians
lenseg = floor(length(monox)/segments) - 1;
pointer = 1;
angle = initial_angle * pi / 180; %in radians

y=[[];[]];

for i=1:segments
 A =[cos(angle), sin(angle); -sin(angle), cos(angle)];
 stereox = [monox(pointer:pointer+lenseg)'; monox(pointer:pointer+lenseg)'];
 y = [y, A * stereox];
 angle = angle + angle_increment; pointer = pointer + lenseg;
end;

% sound(y, 44100)
% write output
% wavwrite(y',Fs,bits,'out_stereopan.wav');

figure(1);
hold on
subplot(2,1,1);
plot(y(1,:));
title('Stereo Panned Signal Channel 1 (L)');
subplot(2,1,2);
plot(y(2,:));
title('Stereo Panned Signal Channel 2 (R)');
