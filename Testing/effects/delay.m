function [y] = delay(x, BL, FB, FF, M)

%This is a delay effect that utilizes an universal comb filter.
%
%The structure is:  [y] = delay(audio_test, x, BL, FB, FF, M)
%
% where audio_test = filtered audio from C++ code
%       x = input signal
%       BL = Blend
%       FB = Feedback
%       FF = Feedforward
%       M = delay in samples
% Chapter 7: Digital Audio Effects by Prof David Marshall and Dr Kirill Sidorov, 
% Cardiff School of Computer Science.

Delayline=zeros(M,1); % memory allocation for length 10
for n=1:length(x)
    xh=x(n)+FB*Delayline(M);
    y(n)=FF*Delayline(M)+BL*xh; 
    Delayline=[xh;Delayline(1:M-1)];
end

end
