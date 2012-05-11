function sensors = genRetina( sensorNum, sensorSize, portx, porty )
    sensors = zeros(2, sensorNum);

    for s=1:sensorNum
        sensors(:, s) = [randi(porty-sensorSize) randi(portx-sensorSize)];
    end

end

