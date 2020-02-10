function y=delay(x,d)

%This function delays a signal by d samples.
%
%The structure is: y = delay(x,d)
%
%where x = input signal
%      d = the number of samples to delay the input signal
%
%
% Gautham J. Mysore - gauthamjm@yahoo.com
%

y = [zeros(d,1); x];

