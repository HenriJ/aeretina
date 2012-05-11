%% Init
clear all; close all;

port = 1024;

width = 20;
delta = 50;
center = floor(port/2);

iterations = 100;
deltas = zeros(1, iterations);


%% Generates Input
input = randn(port, port).^2/40;

input(1:center, (center - width):(center + width)) = 1;
input(center:port, (center + delta - width):(512 + delta + width)) = 1;

input = conv2(input, fspecial('gaussian', 30, 5), 'same');
input = input / max(max(input));

%hist(input(:), 0:0.1:1)
%plot(sum(input))
imagesc(input);


%% Generates sensors
sensorSize = 50;

%sensors = genRetina(400, sensorSize, port);
sensors = genCamera(sensorSize, port);
sensorNum = length(sensors);

sensors(3, :) = abs(randn(1, sensorNum)/100);


%% Computes sensor vs input

max = 0;

for s=1:sensorNum
    sensor = sensors(:, s);
    x = sensor(1);
    y = sensor(2);

    sensors(3, s) = sensors(3, s) + sum(sum(input(x:(x+sensorSize), y:(y+sensorSize))));
    if (sensors(3, s) > max)
        max = sensors(3, s);
    end
end

for s=1:sensorNum
    sensors(3, s) = sensors(3, s) / max;
end



%% Display sensors

tobedisplayed = sensors;

sensorDisp = zeros(port, port);
for s=1:length(tobedisplayed)
    sensor = tobedisplayed(:, s);
    
    sensorDisp(sensor(1), sensor(2)) = sensor(3);
end

imagesc(conv2(sensorDisp, ones(sensorSize)/(sensorSize^2), 'same'));

%imagesc(sensorDisp);

%hist(sensorDisp(:), 0:0.05:1);


%% Where
threshold = 0.25;

sensors = sortrows(sensors', 1)';

border = sensors(1, :) > (center - sensorSize);

sensorsUp = sensors;
sensorsUp(:, border) = [];

border = sensors(1, :) < (center + sensorSize);

sensorsDown = sensors;
sensorsDown(:, border) = [];

plot_deltas = [];

detected_delta = 0;
weightsum = 0;

%figure;
%i = 0;
for up=1:length(sensorsUp)
    sensor_up = sensorsUp(:, up);
    if (sensor_up(3) > threshold)
        for dn=1:length(sensorsDown)
            sensor_dn = sensorsDown(:, dn);
            if (sensor_dn(3) > threshold)
                weight = sensor_up(3)*sensor_dn(3);
                detected_delta = detected_delta + (sensor_dn(2)-sensor_up(2)) * weight;
                weightsum = weightsum + weight;
                %i = i + 1;
                
%                 if (mod(i, 5000) == 0)
%                     plot_deltas = [plot_deltas detected_delta/weightsum];
%                     hold on;
%                     plot(ones(1, length(plot_deltas)) * delta, 'red');
%                     plot(plot_deltas, 'blue');
%                     hold off;
%                     drawnow();
%                 end
            end
        end
    end
end

detected_delta/weightsum