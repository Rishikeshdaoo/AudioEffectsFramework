function y=distortion(x, gain, mix)
% function y=expdist(x, gain, mix)
% Author: Bendiksen, Dutilleux, Z¨olzer, Dempwolf
% y=expdist(x, gain, mix)
% Distortion based on an exponential function
% x - input
% gain - amount of distortion, >0
% mix - mix of original and distorted sound, 1=only distorted
q=x*gain;
z=sign(q).*(1-exp(-abs(q)));
y=mix*z+(1-mix)*x;


% function [x]=distortion(a,x)
% %[Y] = distortion(audio_test, A, X) Guitar Distortion
% %
% %   Distortion creates a distortion effect like that of
% %   an overdriven guitar amplifier. This is a Matlab 
% %   implementation of an algorithm that was found on 
% %   www.musicdsp.org.
% %   audio_test = filtered audio from C++ code
% %   A = The amount of distortion.  A
% %       should be chosen so that -1<A<1.
% %   X = Input.  Should be a column vector 
% %       between -1 and 1.
% %
% %coded by: Steve McGovern, date: 09.29.04
% %URL: http://www.steve-m.us
% 
% k = 2*a/(1-a);
% x = (1+k)*(x)./(1+k*abs(x));
% 
% end
