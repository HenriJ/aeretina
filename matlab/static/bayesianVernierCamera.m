%% Init
clear all; close all;

portx = 768;
porty = 512;

width = 30;

sensorSize = 60;
deltamax = sensorSize*3;

centerx = floor(portx/2);
centery = floor(porty/2);

shades = 256;

%sensors = genCamera(sensorSize, portx, porty);
sensors = genRetina(16, sensorSize, portx, porty);
sensorNum = length(sensors);

oc_cell_val_direction = zeros(sensorNum, shades, 3);

%kernel = ones(sensorSize, sensorSize);
kernel = fspecial('gaussian', sensorSize, sensorSize/6);

iterations = 100;

noise = 0.0;


%% Loop

deltas = -deltamax:deltamax;
locs_oc_cell_val_direction = cell(1, length(deltas));
sofar = 0;
parfor d = 1:length(deltas);
    delta = deltas(d);
    direction = sign(delta) + 2;
    
    loc_oc_cell_val_direction = zeros(size(oc_cell_val_direction));
    for iter = 1:iterations
        %% Input & vals
        input = generateSingleGrating(portx, porty, width, delta, noise);
        sensorVals = computeSensorInput(sensors, input, kernel);

        %% Bayesian update
        for s=1:sensorNum
            shade = min(floor(sensorVals(s)*shades) + 1, shades);
            loc_oc_cell_val_direction(s, shade, direction) = loc_oc_cell_val_direction(s, shade, direction) + 1;
            %oc_cell_val_direction(s, shade, direction) = oc_cell_val_direction(s, shade, direction) + 1;
        end
    end
    delta
    locs_oc_cell_val_direction{d} = loc_oc_cell_val_direction;
end


for d = 1:length(deltas)
    oc_cell_val_direction = oc_cell_val_direction + locs_oc_cell_val_direction{d};
end


%% Bayesian inversion
oc_direction = squeeze(sum(sum(oc_cell_val_direction)));
pb_direction = oc_direction / sum(oc_direction);

oc_cell_val = sum(oc_cell_val_direction, 3);
pb_cell_val = oc_cell_val ./ repmat(sum(oc_cell_val,2), 1, shades);

pb_cell_val_direction = zeros(size(oc_cell_val_direction));
for c = 1:sensorNum
    pb_cell_val_direction(c, :, :) = oc_cell_val_direction(c, :, :) ./ repmat(sum(oc_cell_val_direction(c, :, :)), 1, shades);
end


pb_cell_direction_val = zeros(size(pb_cell_val_direction));
for c = 1:sensorNum
    for v = 1:shades
        for d = 1:3
            if pb_cell_val(c, v) ~= 0
                pb_cell_direction_val(c, v, d) = pb_cell_val_direction(c, v, d) * pb_direction(d) / pb_cell_val(c, v);
            end
        end
    end
end


%% Bayesian fit
iterations = 25;

deltamax = sensorSize*2;

deltas = -deltamax:deltamax;
successes = zeros(1, length(deltas));
directions = zeros(3, length(deltas));

for iter = 1:iterations
    for d = 1:length(deltas)
        delta = deltas(d);
        direction = sign(delta) + 2;
    
        input = generateSingleGrating(portx, porty, width, delta, noise);
        sensorVals = computeSensorInput(sensors, input, kernel);

        loc_pb_direction = ones(3, 1);

        for c = 1:sensorNum
            shade = min(floor(sensorVals(c)*shades) + 1, shades);
            loc_pb_direction = loc_pb_direction .* squeeze(pb_cell_direction_val(c, shade, :));
        end
        
        [a b] = max(loc_pb_direction);
        if (direction ==b)
            successes(d) = successes(d) + 1 ;
        end
        
        directions(:, d) = directions(:, d) + loc_pb_direction;
    end
        
    M = max(max(directions));

    plot(directions(1, :) / M, 'red');
    hold on; 
    plot(directions(2, :) / M, 'blue');
    plot(directions(3, :) / M, 'green');
    plot(successes / iter, 'black');
    hold off;
    
    drawnow();
    iter
    sum(successes / iter < 0.5)
end

sum(successes / iter < 0.5)