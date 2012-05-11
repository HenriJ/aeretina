clear all;

deltas = 10:40;
noiseratio = 0.01;

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


% imagesc(distances);

hist(nonzeros(distances(:)), 0:0.001:max(max(distances)));