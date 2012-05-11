function [ON OFF] = pixelSimu(signal, noiseratio, threshold)
    noise = normrnd(ones(1, length(signal)), noiseratio);

    lnss = log(max(signal + noise, 0.01));

    ON = zeros(1, 20);
    OFF = zeros(1, 20);

    indexON  = 1;
    indexOFF = 1;
    last_event = 1;
    for i = 1:length(lnss)
       temp = lnss(i) - lnss(last_event);
       if (abs(temp) > threshold)
           if sign(temp) == 1
               ON(1, indexON) = i;
               indexON = indexON + 1;
           else
               OFF(1, indexOFF) = i;
               indexOFF = indexOFF + 1;
           end
           last_event = i;
       end
    end
    
    ON  =  ON(1:(indexON-1));
    OFF = OFF(1:(indexOFF-1));
end



