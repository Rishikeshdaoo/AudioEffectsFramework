function [y,b,a]=schroeder(x,n,g,d,k)
%This is a reverberator based on Schroeder?s design which consists of n all
%pass filters in series.
%
%The structure is: [y,b,a] = schroeder1(x,n,g,d,k)
%
%where x = the input signal
%      n = the number of allpass filters
%      g = the gain of the allpass filters (should be less than 1 for stability)
%      d = a vector which contains the delay length of each allpass filter
%      k = the gain factor of the direct signal
%      y = the output signal
%      b = the numerator coefficients of the transfer function
%      a = the denominator coefficients of the transfer function
%
% Chapter 7: Digital Audio Effects by Prof David Marshall and Dr Kirill Sidorov, 
% Cardiff School of Computer Science.

% addpath('./Schroeder/');

[y,b,a] = allpass(x,g,d(1));
% send the output of each allpass filter to the input of the next allpass filter
for i = 2:n
[y,b1,a1] = allpass(y,g,d(i));
[b,a] = seriescoefficients(b1,a1,b,a);
end

% add the scaled direct signal
y = y + k*x;
% normalize the output signal
y = y/max(y);

end
