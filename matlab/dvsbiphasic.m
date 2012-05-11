clear all;

dot1 = 475;
dot2 = 525;

s = zeros(1, 1000);
s(dot1) = 1;
s(dot2) = 1;

s = conv(s, exp(-(-100:100).^2/100), 'same');
s = s + 0.1;
s = s/max(s);

sensorSize = 10;
ss = conv(s, ones(1,sensorSize), 'same');
ss = ss/max(ss);

%noise = rand(1, length(ss))/10;
noise = normrnd(ones(1,length(ss)), 1)/40;

ss = ss + noise;

t = [];
p = [];

last_event = 1;
ss(1) = 0.05;
temps = zeros(1, length(ss));
for i = 1:length(ss)
   temp = log(ss(i)) - log(ss(last_event));
   temps(i) = temp;
   if (abs(temp) > 0.8)
       t = [t i];
       p = [p sign(temp)];
       last_event = i;
   end
end

events = zeros(1, length(ss));
for i = 1:length(t)
    events(t(i)) = p(i);
end

figure, hold on;
plot(s, 'black');
plot(ss, 'blue');
plot(events, 'red');

plot(temps, 'green');
