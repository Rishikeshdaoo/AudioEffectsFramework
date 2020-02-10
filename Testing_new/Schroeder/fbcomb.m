function [y,b,a]=fbcomb(x,g,d)

%This is a feedback comb filter function.
%
%The structure is:  [y,b,a] = fbcomb(x,g,d)
%
%where x = the input signal
%      g = the feedback gain (this should be less than 1 for stability)
%      d = the delay length
%      y = the output signal
%      b = the numerator coefficients of the transfer function
%      a = the denominator coefficients of the transfer function
%
%
% Gautham J. Mysore - gauthamjm@yahoo.com
%


%If the feedback gain is more than 1, set it to 0.7 .
if g>=1
   g=0.7;
end   

%Set the b and a coefficients of the transfer function depending on g and d.
b=[zeros(1,d) 1];
a=[1 zeros(1,d-1) -g];

%filter the input signal 
y=filter(b,a,x);