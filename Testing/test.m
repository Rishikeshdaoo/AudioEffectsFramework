function [audio_mat, MSE] = test(audio_raw, audio_cpp, effect, param)

% Compares two input signals, calculate MSE, and plot the two signals 
% along with the difference signal.
%
%where input:
%      audio_raw = raw audio signal 
%      audio_cpp = audio signal generated from cpp implementation 
%      effect = name of the effect to be tested
%      param = array of parameters required for the effect to be tested
%      output:
%      audio_mat = audio signal generated from matlab implementation
%      MSE = mean square error of the signals from cpp and matlab
%      implementation

switch effect
    case "flanger"
        audio_mat = flanger(audio_raw, param(1), param(2), param(3));
        MSE = compare(audio_cpp, audio_mat);
    case "chorus"
        audio_mat = flanger(audio_raw, param(4), param(5), param(6), param(7), param(8));
        MSE = compare(audio_cpp, audio_mat);
    case "reverb"
        audio_mat = flanger(audio_raw, param(9), param(10), param(11), param(12));
        MSE = compare(audio_cpp, audio_mat);
end
end
