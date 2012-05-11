clear all;

deltas = 10:40;

ONclasses = {};
ocdvsd = zeros(length(deltas), 10); % delta, responseid => occurences

for d = 1:length(deltas);
    
    delta = deltas(d);

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
    
    ss = ss(50:349);

    for iter = 1:1000
        [ON OFF] = pixelSimu(ss, 0.05, 0.2);

        bestd = 10000;
        bestclass = 1;
        for j = 1:length(ONclasses)
            dist = spkdexp(ON, ONclasses{j}, 10);
            if (dist < bestd)
                bestd = dist;
                bestclass = j;
            end
        end

        if (bestd < 0.05)
            ocdvsd(d, bestclass) = ocdvsd(d, bestclass) + 1;
        else
            bestclass = length(ONclasses)+1
            ONclasses{bestclass} = ON;
            ocdvsd(d, bestclass) = 1;
        end
        
        %iter
    end

    d
end

pdvs = sum(ocdvsd); pdvs = pdvs / sum(pdvs);
pd = sum(ocdvsd, 2); pd = pd / sum(pd);
pdvsd = ocdvsd ./ repmat(sum(ocdvsd, 2), 1, 159);

imagesc(pdvsd);

%--- Best classes

[null order] = sort(sum(pdvsd), 'descend');
figure, hold on;
cols = jet(6);
for i = 1:length(cols)
    class = order(i)
    sync = zeros(1, 300);
    sync(ONclasses{class}) = 1;
    plot(sync+i*2, 'color', cols(i, :));
end;

%--- Inversion

pddvs = zeros(size(ocdvsd));

for i = 1:length(pdvs)
    for j = 1:length(pd)
        pddvs(j, i) = pdvsd(j, i) * pd(j) / pdvs(i);
    end
end
imagesc(pddvs);