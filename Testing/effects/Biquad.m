function outputvector = Biquad(inputvector, freqvalue, qvalue, gain, Fs)

%This is an implementation of a biquad filter.
%
%The structure is:  outputvector = myPeak(inputvector, freqvalue, qvalue, gain, samplerateinHz)

%
% where audio_test = filtered audio from C++ code
%       inputvector = input signal
%       freqvalue = center frequency in Hz 
%       qvalue = quality factor
%       gain = gain in dB
%       Fs = sample rate in Hz 
%
% Implemented as part of DSP Assignment (from Robert Bristow-Johnson's cookbook formulae for audio equalizer biquad filter coefficients)



    [a0, a1, a2, b0, b1, b2] = computeCoeffs(freqvalue, qvalue, gain, Fs);
    outputvector = filter([b0, b1, b2], [a0, a1, a2], inputvector);     
end


function a = computeA(gain)
    a = 10^(gain/40);
end

function w = computeOmega(freq, sampleRate)
    w = 2 * pi * (freq/sampleRate);
end

function alpha = computeAlpha(qvalue, omega)
    alpha = sin(omega)/(2*qvalue);
end


function [a0,a1,a2, b0, b1, b2] = computeCoeffs(freq, qvalue, gain, fs)
    A = computeA(gain);
    w = computeOmega(freq, fs);
    alpha = computeAlpha(qvalue, w);
    b0 = 1 + (alpha * A);
    b1 = -2 * cos(w);
    b2 = 1 - (alpha * A);
    a0 = 1  + (alpha/A);
    a1 = -2 * cos(w);
    a2 = 1 - (alpha/A);
end