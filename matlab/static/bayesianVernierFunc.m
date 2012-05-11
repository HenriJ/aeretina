% bayesianVernierFunc( 0.01, 60, 20, 512 )
function [ true_success ] = bayesianVernierFunc( noise, shades, width, cells )

%rand('seed', now);
h = fopen('/dev/random', 'r');
seed = fread(h, 1);
s = RandStream('mt19937ar', 'Seed', seed);
RandStream.setDefaultStream(s);
fclose(h);


%% Init
portx = 768;
porty = 512;

sensorSize = 60;
deltamax = 60*3;


if cells == 0
    sensors = genCamera(sensorSize, portx, porty);
else
    sensors = genRetina(cells, sensorSize, portx, porty);
end
sensorNum = length(sensors);

oc_cell_val_direction = zeros(sensorNum, shades, 3);

%kernel = ones(sensorSize, sensorSize);
kernel = fspecial('gaussian', sensorSize, sensorSize/6);

iterations = 256;


%% Loop

deltas = -deltamax:deltamax;
locs_oc_cell_val_direction = cell(1, length(deltas));

for d = 1:length(deltas);
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
    strcat('montecarlo: ', num2str(floor(100*d/length(deltas))), '%')
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
        
        [~, b] = max(loc_pb_direction);
        if (direction == b)
            successes(d) = successes(d) + 1 ;
        end
        
        directions(:, d) = directions(:, d) + loc_pb_direction;
    end

    strcat('success: ', num2str(floor(100*iter/iterations)), '%')
end

    true_success = sum(successes / iter < 0.5);
    
    export = [noise shades width sensorNum cells true_success seed]
    
    outfile = '/home/omarre/h2/out/job';
    for val = export
        outfile = strcat(outfile, '_', num2str(val));
    end
    outfile = strcat(outfile, '_', num2str(floor(rand()*1000)));
    outfile
    
    save(outfile, 'export', '-ascii');
end
