function [y, MSE] = compexp(audio_test, x,comp,release,attack,a,Fs)
% This is a Compressor/expander effect. 
%
%The structure is:  y=compexp(audio_test, x,comp,release,attack,a,Fs)
%
% where audio_test = filtered audio from C++ code
%       x = input audio signal
%       comp = ampunt of compression - compression: 0>comp>-1, expansion: 0<comp<1
%       a = filter parameter – <1
%
% Chapter 7: Digital Audio Effects by Prof David Marshall and Dr Kirill Sidorov, 
% Cardiff School of Computer Science.

h=filter([(1-a)^2],[1.0000 -2*a a^2],abs(x)); 
h=h/max(h);
h=h.^comp;
y=x.*h;
y=y*max(abs(x))/max(abs(y));

MSE = mean( (y - audio_test).^2 )

figure(1);
hold on
subplot(3,1,1);
plot(y);
title('Compressor/Expander (MATLAB)');
subplot(3,1,2);
plot(audio_test);
title('Compressor/Expander (C++)');
subplot(3,1,3);
plot(audio_test - y);
title('Difference');
