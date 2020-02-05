function [y, MSE] = panorama(audio_test, monox, initial_angle, final_angle, segments)


%This is a pan effect.
%
%The structure is:  [y,b,a] = schroeder1(x,n,g,d,k) 
%
%where audio_test = filtered audio from C++ code (size = (2 x num_samples))
%      monox = the input signal (mono)
%      initial_angle = initial angle in stereo field in degrees	
%      final_angle = final angle in stereo field in degrees
%      segments = 
%
% script based on DAFZ p 140 to perform matrix based panning of mono sound to stereo

% close all;
% clear all;

% initial_angle = -40; 
% final_angle = 40;    
% segments = 32;
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

figure(1);
hold on
subplot(6,1,1);
plot(y(1,:));
title('Stereo Panned Signal Channel 1 (L) (MATLAB)');
subplot(6,1,2);
plot(y(2,:));
title('Stereo Panned Signal Channel 2 (R) (MATLAB)');
subplot(6,1,3);
plot(audio_test(1,:));
title('Stereo Panned Signal Channel 1 (L) (C++)');
subplot(6,1,4);
plot(audio_test(2,:));
title('Stereo Panned Signal Channel 2 (R) (C++)');
subplot(6,1,5);
plot((audio_test - y)(1,:));
title('Difference Channel 1 (L)');
subplot(6,1,6);
plot((audio_test - y)(2,:));
title('Difference Channel 2 (R)');

