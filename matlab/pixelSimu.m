function spikes = pixelSimu(signal, noiseratio, threshold)
    noise = normrnd(ones(1, length(signal)), noiseratio);

    lnss = log(max(signal + noise, 0.01));

    spikes = zeros(1, 20);

    index  = 1;
    last_event = 1;
    for i = 1:length(lnss)
       temp = lnss(i) - lnss(last_event);
       if (abs(temp) > threshold)
           spikes(index) = i * sqrt(sign(temp));
           index = index + 1;
           last_event = i;
       end
    end
    
    spikes = spikes(1:(index-1));
end



