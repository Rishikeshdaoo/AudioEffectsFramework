function y = comp_exp(x, CT, CS, ET, ES)
%
% Author: M. Holters
%
% Structure: y = compexp(x, CT, CS, ET, ES)
%
% Where x = input signal
%       CT = Compressor threshold 
%       CS = Compressor slope 
%       ET = Expander threshold 
%       ES = Expander slope
%
%--------------------------------------------------------------------------
% This source code is provided without any warranties as published in 
% DAFX book 2nd edition, copyright Wiley & Sons 2011, available at 
% http://www.dafx.de. It may be used for educational purposes and not 
% for commercial applications without further permission.
%--------------------------------------------------------------------------


tav = 0.01;
at = 0.03;
rt = 0.003;
delay = 150;

xrms = 0;
g = 1;
buffer = zeros(1,delay);

for n = 1:length(x)
  xrms = (1-tav) * xrms + tav * x(n)^2;
  X = 10*log10(xrms);
  G = min([0, CS*(CT-X), ES*(ET-X)]);
  f = 10^(G/20);
  if f < g
    coeff = at;
  else
    coeff = rt;
  end;
  g = (1-coeff) * g + coeff * f;
  y(n) = g * buffer(end);
  buffer = [x(n) buffer(1:end-1)];
end;