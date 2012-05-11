clear all;

tmax = 1000000;
events = 100;

tau = 10000;

times = unique(sort(floor(rand(1, events)*tmax)));


sync = zeros(1, tmax);
sync(times) = 1;
numeventssync = conv2(sync, ones(1, 10000), 'same');


numeventsasync = zeros(1, tmax);
u = 0;
lastt = 0;
deltaexp = exp(-3/(tau));
for i = 1:tmax
    u = u * deltaexp;
    
    if (ismember(i, times))
        u = u + 1;
        i
        u
    end

    numeventsasync(i) = u;
end