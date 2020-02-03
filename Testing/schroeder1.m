function [y,b,a]=schroeder1(x,n,g,d,k)
%This is a reverberator based on Schroeder's design which consists of n all pass filters in series.
%
%The structure is:  [y,b,a] = schroeder1(x,n,g,d,k) 
%
%where x = the input signal
%      n = the number of allpass filters	
%      g = the gain of the allpass filters (this should be less than 1 for stability)
%      d = a vector which contains the delay length of each allpass filter
%      k = the gain factor of the direct signal
%      y = the output signal
%      b = the numerator coefficients of the transfer function
%      a = the denominator coefficients of the transfer function
%
% note: Make sure that d is the same length as n.
%
%
% Gautham J. Mysore - gauthamjm@yahoo.com
%


addpath(genpath('./Schroeder/'));
% send the input signal through the first allpass filter
[y,b,a] = allpass(x,g,d(1));

% send the output of each allpass filter to the input of the next allpass filter
for i = 2:n,
   [y,b1,a1] = allpass(y,g,d(i));
   [b,a] = seriescoefficients(b1,a1,b,a);
end   

% add the scaled direct signal
y = y + k*x;

% normalize the output signal
y = y/max(y);





