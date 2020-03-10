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
    case "biquad"
        audio_mat = Biquad(audio_raw, param{1}, param{2}, param{3},param{4}, param{5});
        MSE = compare(effect, audio_cpp, audio_mat);
%     case "flanger"
%         audio_mat = flanger(audio_raw, param(1), param(2), param(3));
%         MSE = compare(audio_cpp, audio_mat);
%     case "chorus"
%         audio_mat = chorus(audio_raw, param(1), param(2), param(3));
%         MSE = compare(audio_cpp, audio_mat);
%     case "reverb"
%         audio_mat = schroeder(audio_raw, param(1), param(2), param(3));
%         MSE = compare(audio_cpp, audio_mat);
%     case "distortion"
%         audio_mat = distortion(audio_raw, param(1), param(2), param(3));
%         MSE = compare(audio_cpp, audio_mat);
%     case "compressor"
%         audio_mat = comp_exp(audio_raw, param(1), param(2), param(3));
%         MSE = compare(audio_cpp, audio_mat);
%     case "delay"
%         audio_mat = delay(audio_raw, param(1), param(2), param(3));
%         MSE = compare(audio_cpp, audio_mat);
%     case "pan"
%         audio_mat = panorama(audio_raw, param(1), param(2), param(3));
%         MSE = compare(audio_cpp, audio_mat);
    
end
end
