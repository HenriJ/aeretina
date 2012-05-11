function signal = generateSignal( delta )
    dot1 = 200-delta;
    dot2 = 200+delta;

    s = zeros(1, 400);
    s(dot1) = 1;
    s(dot2) = 1;

    s = conv(s, exp(-(-100:100).^2/100), 'same');
    s = s + 0.1;
    s = s/max(s);

    sensorSize = 50;
    ss = conv(s, ones(1,sensorSize)/sensorSize, 'same');
    ss = ss/max(ss);
    
    signal = ss(50:349);
end
