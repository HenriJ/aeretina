clear all;
[x, y, p, t] = aerLoad('/home/riton/JAER_DATA/Brep.dat');

maxFreq = 1000;
x = x(4:100000);
y = y(4:100000);
p = p(4:100000);
t = t(4:100000);

t = t(p==1);
x = x(p==1);
y = y(p==1);

t = double(t);
T = max(t);
FFT = zeros(1, maxFreq);

for k = 1:maxFreq
    temp = 0;
    for j = 1:length(t)
        temp = temp + exp(double(-1i * 2 * pi * k) * t(j) / T);
    end
    FFT(k) = temp;
    k
end

plot(abs(FFT(1:end))); hold on;


% ---

sync = zeros(1, T);
sync(t) = 1;
syncFFT = abs(fft(sync));
plot(syncFFT(2:1000), 'red');


% ---

interspikes = zeros(1,T);
for i = 1:length(t)
    for j = i+1:length(t)
        interspikes(t(j) - t(i) + 1) = interspikes(t(j) - t(i) + 1) + 1;
    end
    i
end

cinterspikes = conv(interspikes, 1:100, 'same');
plot(conv(interspikes, 1:100, 'same'));

plot(cinterspikes);

% ---

binSize = 1000;

interspikes = zeros(1, ceil(T/binSize));
bin = 0;
for i = 1:length(t)
    for j = i+1:length(t)
        bin = floor((t(j) - t(i)) / binSize) + 1;
        interspikes(bin) = interspikes(bin) + 1;
    end
    %i
end

plot(interspikes);

%----

interspikes = zeros(1, T);
for lx = 1:128
    for ly = 1:128
        lt = t((x==lx) & (y==ly));
        for i = 1:length(lt)
            for j = i+1:length(lt)
                interspikes(lt(j) - lt(i) + 1) = interspikes(lt(j) - lt(i) + 1) + 1;
            end
        end
    end
    lx
end

plot(conv(interspikes, 1:100, 'same'));