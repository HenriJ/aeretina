function displaySensors( sensors, sensorVals, sensorSize )

    sensorDisp = zeros(max(sensors, [], 2)');

    for s=1:length(sensors)
        sensor = sensors(:, s);
        sensorDisp(sensor(1), sensor(2)) = sensorVals(s);
    end

    imagesc(conv2(sensorDisp, ones(sensorSize)/(sensorSize^2), 'full'));

end

