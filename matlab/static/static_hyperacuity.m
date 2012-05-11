%% Init
clear all; close all;

port = 1024;

width = 10;
delta = 40;
center = floor(port/2);

sensorSize = 150;

iterations = 100;
deltas = zeros(1, iterations);

cols = hsv(iterations); c = 0;
figure(1); figure(2), hold on; 

% Generates Input
input = randn(port, port).^2/20;

input(:, (512 - delta - width):(512 - delta + width)) = 1;
input(:, (512 + delta - width):(512 + delta + width)) = 1;

input = conv2(input, fspecial('gaussian', 60, 10), 'same');

%hist(input(:), 0:0.1:1)
plot(sum(input))
iter = 1;

%% Loop
for iter = 1:iterations

    %% Generates sensors
    close all;
    sensors = genRetina(100000, sensorSize, port);
    %sensors = genCamera(sensorSize, port);
    sensorNum = length(sensors);


    %% Computes sensor vs input

    for s=1:sensorNum
        sensor = sensors(:, s);
        x = sensor(1);
        y = sensor(2);

        sensors(3, s) = sensors(3, s) + sum(sum(input(x:(x+sensorSize), y:(y+sensorSize))));
    end


    %% Display sensors

    sensorDisp = zeros(port, port);
    for s=1:sensorNum
        sensor = sensors(:, s);
        sensorDisp(sensor(1), sensor(2)) = sensor(3);% + sensorDisp(sensor(1), sensor(2));
    end
    imagesc(sensorDisp);
    
    %sensorDisp = conv2(sensorDisp, ones(sensorSize, sensorSize), 'same');

    figure(3);
    imagesc(sensorDisp);


    %% Re-Compute delta
    vals = conv(sum(sensorDisp), fspecial('gaussian',[100 ,1], 10), 'same');
    %vals2 = vals(2:end)-vals(1:end-1);
    %[pks, locs] = findpeaks(vals(vals2~=0), 'sortstr', 'descend');
    [pks, locs] = findpeaks(vals, 'sortstr', 'descend');
    
    [null b] = sort(pks, 'descend');
    
    detected_delta = abs(locs(b(1)) - locs(b(2)))/2;
    deltas(iter) = detected_delta;
    
    figure(1);
    plot(deltas);
    [median(deltas(1:iter)) std(deltas(1:iter))]

    figure(2);
    plot(vals/max(vals) + iter, 'color', cols(iter, :))
    
    drawnow();

end
