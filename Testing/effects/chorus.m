function [modulated_output] = chorus(input, Fs, max_time_delay, modulation_depth, modulation_rate, feedback)
% function [y, MSE] = chorus(audio_test, x, Fs, max_time_delay, rate)


%This is a chorus effect. 
%The structure is:  function [modulated_output, MSE] = chorus(audio_test, input, Fs, max_time_delay, modulation_depth, modulation_rate, feedback)
%
%where audio_test = filtered audio from C++ code (size = (2 x num_samples))
%      input = the input signal 
%      Fs = Sampling rate 
%      max_time_delay = delay time in seconds
%      modulation_depth = depth of modulation in seconds
%      modulation_rate = frequency of LFO in Hz
%      feedback = feedback amount (0.0 for all dry, 1.0 for all wet)
%
% Source: https://github.com/danpprince/matlab-chorus/


delay_length_samples     = round(max_time_delay * Fs);
modulation_depth_samples = round(modulation_depth * Fs);

modulated_output = zeros(length(input), 1);
delay_buffer     = zeros(delay_length_samples + modulation_depth_samples, 1);

% Argument for sin() modulation function. Converts the loop's control variable into 
% the appropriate argument in radians to achieve the specified modulation rate
modulation_argument = 2 * pi * modulation_rate / Fs;


for i = 1:(length(input))
	% Find index to read from for modulated output
	modulated_sample = modulation_depth_samples * sin(modulation_argument * i);
	modulated_sample = modulated_sample + delay_length_samples;

	% Get values to interpolate between
	interp_y1 = delay_buffer(floor(modulated_sample));
	interp_y2 = delay_buffer( ceil(modulated_sample));

	query_sample = modulated_sample - floor(modulated_sample);

	% Interpolate to find the output value
	modulated_output(i) = interp_y1 + (interp_y2 - interp_y1) * (query_sample);

	% Save the input's current value in the buffer and advance to the next value
	new_sample = (input(i) + modulated_output(i) * feedback);
	delay_buffer = [ new_sample; delay_buffer(1 : length(delay_buffer)-1) ];
end
