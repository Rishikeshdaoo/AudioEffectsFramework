function [MSE] = compare(effect, audio_cpp, audio_mat)

% Compares two input signals, calculate MSE, and plot the two signals 
% along with the difference signal.
%
%where input:
%      effect = effect type
%      audio_cpp = audio signal generated from cpp implementation 
%      audio_mat = audio signal generated from matlab implementation 
%      output:
%      MSE = mean square error of the two input signals

MSE = immse(audio_cpp, audio_mat); % mean square error

figure(1);
sgtitle(effect);

subplot(3,1,1);
plot(audio_mat);
title('MATLAB');

subplot(3,1,2);
plot(audio_cpp);
title('C++');

subplot(3,1,3);
plot(audio_cpp - audio_mat);
title('Difference');

end
