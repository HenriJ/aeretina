function sensors = genCamera( sensorSize, portx, porty )
    sensorNum = floor((portx-1)/sensorSize) * floor((porty-1)/sensorSize);
    
    sensors = zeros(2, sensorNum);
    
    x = 1;
    y = 1;

    for s=1:sensorNum
        sensors(:, s) = [x y];
        x = x + sensorSize;
        if ( x + sensorSize > porty)
            x = 1;
            y = y + sensorSize;
        end

    end
    
end

