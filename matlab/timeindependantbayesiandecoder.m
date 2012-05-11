clear all;

delta = 25;

dot1 = 200-delta;
dot2 = 200+delta;

s = zeros(1, 400);
s(dot1) = 1;
s(dot2) = 1;

s = conv(s, exp(-(-100:100).^2/100), 'same');
s = s + 0.1;
s = s/max(s);

sensorSize = 10;
ss = conv(s, ones(1,sensorSize), 'same');
ss = ss/max(ss);

cumulON  = zeros(1, 400);
cumulOFF = zeros(1, 400);

figure, hold on;
iterations = 10000;
cols = jet(iterations);

for iter = 1:iterations
    noise = normrnd(ones(1,length(ss)), 1)/50;

    lnss = log(ss + noise);

    last_event = 1;
    for i = 1:length(ss)
       temp = lnss(i) - lnss(last_event);
       if (abs(temp) > 0.8)

           if (sign(temp) == 1)
               cumulON(i) = cumulON(i)+1;
           else
               cumulOFF(i) = cumulOFF(i)+1;
           end

           last_event = i;
       end
    end

    plot(sync(1, :), 'color', cols(iter, :));
    drawnow();

    iter
end