%% Init
clear all; close all;

portx = 256;
porty = 1024;

width = 10;
delta = 50;

centerx = floor(portx/2);
centery = floor(porty/2);

%% Generates Input
input = randn(porty, portx).^2/40;

input(1:centery, (centerx - width):(centerx + width)) = 1;
input(centery:porty, (centerx + delta - width):(centerx + delta + width)) = 1;

input = conv2(input, fspecial('gaussian', 30, 0.1), 'same');
input = input / max(max(input));

%hist(input(:), 0:0.1:1)
%plot(sum(input))
imagesc(input);


%% Generates sensors
sensorSize = 20;

%sensors = genRetina(1000, sensorSize, portx, porty);
sensors = genCamera(sensorSize, portx, porty);
sensorNum = length(sensors);

%sensors(3, :) = abs(randn(1, sensorNum)/100);


%% Computes sensor vs input

kernel = ones(sensorSize, sensorSize);
max = 0;

for s=1:sensorNum
    sensor = sensors(:, s);
    x = sensor(1);
    y = sensor(2);

    sensors(3, s) = log(sum(sum(kernel .* input(x:(x+sensorSize-1), y:(y+sensorSize-1)))));
    if (sensors(3, s) > max)
        max = sensors(3, s);
    end
end

sensors(3, :) = sensors(3, :) / max + randn(1, length(sensors))/10;



%% Display sensors

tobedisplayed = sensors;

sensorDisp = zeros(porty, portx);
for s=1:length(tobedisplayed)
    sensor = tobedisplayed(:, s);
    
    sensorDisp(sensor(1), sensor(2)) = sensor(3);
end

imagesc(conv2(sensorDisp, ones(sensorSize)/(sensorSize^2), 'full'));

%imagesc(sensorDisp);

%hist(sensorDisp(:), 0:0.05:1);


%% Where Bayesian

up = sum(sensorDisp(1:(centery-sensorSize), :));
plot(up);



%% Where Weighted Sum
threshold = 0.25;

plot_deltas = [];

detected_delta = 0;

up = 0;
upweight = 1;

dn = 0;
dnweight = 1;

%figure;
%i = 0;
for s=1:length(sensors)
    sensor = sensors(:, s);
    
    if (sensor(3) > threshold)
    
        weight = sensor(3);
        if (sensor(1) < (centery - sensorSize))
            up = up + sensor(2) * weight;
            upweight = upweight + weight;
        elseif (sensor(1) > (centery + sensorSize))
            dn = dn + sensor(2) * weight;
            dnweight = dnweight + weight;
        end

        if (mod(s, floor(length(sensors)/100)) == 0)
            plot_deltas = [ plot_deltas  dn/dnweight-up/upweight ];
            plot(plot_deltas);
            drawnow();
        end
    end

end

dn/dnweight-up/upweight