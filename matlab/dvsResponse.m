clear all;

deltas = 10:40;
noiseratio = 0.01;

%--- Distances distribution

iterations = 500;
cells = cell(1, iterations);

for iter = 1:iterations
    signal = generateSignal( deltas(ceil(rand()*length(deltas))) );
    spikes = pixelSimu(signal, noiseratio, 0.2);
    cells{iter} = spikes;
end

h = waitbar(0, 'Distances computation');
distances = zeros(iterations, iterations);
for i = 1:iterations
    for j = (i+1):iterations
        distances(i, j) = spikesDist(cells{i}, cells{j}, 10);
    end
    waitbar(i/iterations, h, 'Distances computation')
end
close(h);

clear 'cells';

% imagesc(distances);

hist(nonzeros(distances(:)), 0:0.001:max(max(distances)));



%--- Monte Carlo

iterations = 1000;
distanceThreshold = 2; % Distance threshold between class's representants

cellClasses = {};

p_d = zeros(1, length(deltas));
p_spikes_d = zeros(length(deltas), 1);


tic

for i = 1:iterations
    d_index = ceil(rand()*length(deltas));
    d = deltas(d_index);
    
    signal = generateSignal(d);
    
    spikes = pixelSimu(signal, noiseratio, 0.2);
    
    bestd = +Inf;
    bestclass = 0;
    for j = 1:length(cellClasses)
        dist = spikesDist(spikes, cellClasses{j}, 10);
        if (dist < bestd)
            bestd = dist;
            bestclass = j;
        end
    end

    if (bestd < class_threshold)
        input_output_count(d, bestclass) = input_output_count(d, bestclass) + 1;
    else
        bestclass = length(cellClasses)+1;
        cellClasses{bestclass} = spikes;
        input_output_count(d, bestclass) = 1;
    end
end


toc

pdvs = sum(input_output_count); pdvs = pdvs / sum(pdvs);
pd = sum(input_output_count, 2); pd = pd / sum(pd);
pdvsd = input_output_count ./ repmat(sum(input_output_count, 2), 1, size(input_output_count, 2));

imagesc(pdvsd);

%--- Best classes

sumpdvsd = log(sum(pdvsd));
sumpdvsd = sumpdvsd - min(sumpdvsd);
sumpdvsd = sumpdvsd/max(sumpdvsd);
[null order] = sort(sumpdvsd, 'descend');
figure, hold on;
cols = jet(30);
for i = 1:length(cols)
    classIndex = order(i)
    class = cellClasses{classIndex};
    sync = zeros(1, 300);
    sync(nonzeros(real(class))) = 1;
    sync(nonzeros(imag(class))) = -1;
    plot(sync+i*2, 'color', cols(i, :), 'LineWidth', max(0.5, sumpdvsd(classIndex)*2));
end;

%--- Inversion

pddvs = zeros(size(input_output_count));

for i = 1:length(pdvs)
    for j = 1:length(pd)
        pddvs(j, i) = pdvsd(j, i) * pd(j) / pdvs(i);
    end
end
imagesc(pddvs);


%--- Test

[a b] = sort(pdvs, 'descend');
plot(cumsum(a));

keeponly = b(1:8000);


iterations = 10;
acceptable = 1;

successes = zeros(1, length(deltas));

figure;
for d = 1:length(deltas)
    delta = deltas(d);
    signal = generateSignal(delta);

    success = 0;
    parfor i = 1:iterations
        spikes = pixelSimu(signal, noiseratio, 0.2);

        bestd = 10000;
        bestclass = 1;
        for j = 1:length(cellClasses)
            if (ismember(keeponly, j))
                dist = spikesDist(spikes, cellClasses{j}, 10);
                if (dist < bestd)
                    bestd = dist;
                    bestclass = j;
                end
            end
        end

        [null detected_delta] = max(pddvs(:, bestclass));
        detected_delta = deltas(detected_delta);

        if (abs(detected_delta - delta) <= acceptable)
            success = success + 1;
        end
        %i
    end

    successes(d) = success;    
    plot(successes);
    drawnow();
    [d success]
end

mean(successes)