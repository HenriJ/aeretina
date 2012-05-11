clear all;

dot1 = 475;
dot2 = 525;

s = zeros(1, 1000);
s(dot1) = 1;
s(dot2) = 1;

s = conv(s, exp(-(-100:100).^2/100), 'same');
%s = conv(s, ones(1, 10));
s = s + 0.1;
s = s/max(s);



% -- test sensorSizes

sensorSizes = [10 20 30 40 50 75 100];
cols = jet(length(sensorSizes));


plot(log(s), 'black'), hold on;
for i = 1:length(sensorSizes)
    sensorSize = sensorSizes(i);
    ss = conv(s, ones(1, sensorSize)/sum(sensorSizes(i)), 'same');
    %ss = ss/max(ss);

    plot(log(ss), 'color', cols(i, :));
end

% ---


sensorSizes = [100];
cols = jet(length(sensorSizes));

figure, hold on;
for j = 1:length(sensorSizes)

    sensorSize = sensorSizes(j);
    ss = conv(s, ones(1,sensorSize), 'same');
    ss = ss/max(ss);

    noise = rand(1, length(ss))/10;

    ss = ss + noise;

    t = [];
    p = [];

    last_event = 1;
    for i = 1:length(ss)
       temp = log(ss(i)) - log(ss(last_event));
       if (abs(temp)>0.4)
           t = [t i];
           p = [p sign(temp)];
           last_event = i;
       end
    end

    figure, hold on;
    plot(s, 'black');
    plot(ss, 'blue');
    events = zeros(1, length(ss));
    for i = 1:length(t)
        events(t(i)) = p(i);
    end
    plot(events, 'color', cols(j, :));
end