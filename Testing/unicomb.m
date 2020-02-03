function [y] = unicomb(x, BL, FB, FF, M)

% x=zeros(100,1);x(1)=1; % unit impulse signal of length 100
% BL=0.5;
% FB=-0.5;
% FF=1;
% M=10;
Delayline=zeros(M,1); % memory allocation for length 10
for n=1:length(x)
    xh=x(n)+FB*Delayline(M);
    y(n)=FF*Delayline(M)+BL*xh; 
    Delayline=[xh;Delayline(1:M-1)];
end