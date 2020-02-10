function [y,b,a]=ffcomb(x,g,d)

%This is a feedforward comb filter function.
%
%The structure is:  [y,b,a] = ffcomb(x,g,d)
%
%where x = the input signal
%      g = the gain of the delayed signal
%      d = the delay length
%      y = the output signal
%      b = the numerator coefficients of the transfer function
%      a = the denominator coefficients of the transfer function
%
%
% Gautham J. Mysore - gauthamjm@yahoo.com
%


%Set the b and a coefficients of the transfer function depending on g and d.
b=[1 zeros(1,d-1) g];
a=[1];

%filter the input signal 
y=filter(b,a,x);