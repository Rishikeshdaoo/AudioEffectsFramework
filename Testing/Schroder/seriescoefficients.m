function [b,a]=seriescoefficients(b1,a1,b2,a2)

%This function gives the filter coefficients of the series connection of two filters.
%
%The structure is:  [b,a] = seriescoefficients(b1,a1,b2,a2)
%
%where b1 = the numerator coefficients of the 1st transfer function
%      a1 = the denominator coefficients of the 1st transfer function
%      b2 = the numerator coefficients of the 2nd transfer function
%      a2 = the denominator coefficients of the 2nd transfer function
%      b = the numerator coefficients of the composite transfer function
%      a = the denominator coefficients of the composite transfer function
%
%
% Gautham J. Mysore - gauthamjm@yahoo.com
%

b = conv(b1,b2);

a = conv(a1,a2);


