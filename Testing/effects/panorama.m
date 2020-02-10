function [y] = panorama(monox, initial_angle, final_angle, segments)


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
end

end
