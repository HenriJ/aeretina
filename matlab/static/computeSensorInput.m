function [ sensorVals ] = computeSensorInput( sensors, input, kernel )
    sensorNum  = length(sensors);
    sensorSize = length(kernel);
    
    sensorVals = zeros(1, sensorNum);

    sensor_max = 0;
    for s=1:sensorNum
        sensor = sensors(:, s);
        x = sensor(1);
        y = sensor(2);

        sensorVals(s) = sum(sum(kernel .* input(x:(x+sensorSize-1), y:(y+sensorSize-1))));
        if (sensorVals(s) > sensor_max)
            sensor_max = sensorVals(s);
        end
    end

    sensorVals = sensorVals / sensor_max;
end

