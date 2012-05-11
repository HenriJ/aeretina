clear all;

realSize = 1280;
real = zeros(realSize);


% Drawing
raw = zeros(size(real));
penSize = 50;
while 1
    imagesc(raw);
    [x, y, button] = ginput(1);
    
    if button == 2
        break;
    end
    
    val = 1;
    if (button == 3)
        val = 0;
    end
    x = ceil(x);
    y = ceil(y);
    for i = x-penSize:x+penSize
        for j = y-penSize:y+penSize
            raw(j, i) = val;
        end
    end
end
clear 'x' 'y' 'i' 'j' 'button' 'val' 'penSize'
real = conv2(raw, fspecial('gaussian', [100 100], 100), 'same');
real = real + 0.1;
imagesc(real);


% Compute sensor perceived intensities
sensorSize = 64;
sensorResolution = 10;

posXRange = 1:600;
posYRange = 250;

sensor3d = zeros([sensorResolution sensorResolution length(posXRange)]);

time = 0;
tic
for posY = posYRange
    for posX = posXRange

        time = time + 1;
        newsensor = zeros(sensorResolution);

        for i = 1:sensorResolution
            vals = zeros(1, sensorResolution);
            for j = 1:sensorResolution
                tempX = posX+(i-1)*sensorSize;
                tempY = posY+(j-1)*sensorSize;
                
                val = sum(sum(real(tempX:(tempX+sensorSize), tempY:(tempY+sensorSize))));
                vals(j) = val;
            end
            sensor3d(i, :, time) = vals;
        end

        imagesc(squeeze(sensor3d(:,:, time)));
        drawnow();
        posX

    end
end
toc

plot(squeeze(sensor3d(5,5,:)));


lsensor3d = log(sensor3d);


t = [];
x = [];
y = [];
p = [];

theta = 0.1;

for i = 1:sensorResolution
    for j = 1:sensorResolution
        last_event = 1;
        for time = 1:length(sensor3d)
           temp = lsensor3d(i, j, time) - lsensor3d(i, j, last_event);
           if (abs(temp)>theta)
               t = [t time];
               x = [x i];
               y = [y j];
               p = [p sign(temp)];
               last_event = time;
           end
        end
    end
end

scatter3(x, y, t);
scatter(x(y==10), t(y==10));

testRange = 0:0.01:20;
testmax = zeros(1, length(testRange));

tic
parfor j = 1:length(testRange)
    ratio = testRange(j);
    vectDict = [ratio 1];
    prodScale = cell(1, sensorResolution);
    for i = 1:length(t)
        prodScale{y(i)} = [prodScale{y(i)} sum(vectDict .* [x(i) t(i)])];
    end


    proj = zeros(sensorResolution, 80);
    for i = 1:sensorResolution
        proj(i, :) = hist(prodScale{i}, 0:10:799);
    end
    imagesc(proj);
    drawnow();

    testmax(j) = max(max(proj));
end
toc

plot(testmax);
