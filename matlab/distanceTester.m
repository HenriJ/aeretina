clear all;


iterations = 500;
cells = cell(1, iterations);

for iter = 1:iterations
    ss = generateSignal( floor(rand()*20+10) );
    spikes = pixelSimu(ss, 0.05, 0.2);
    cells{iter} = spikes;
end

% ---

distances = zeros(iterations, iterations);
for i = 1:iterations
    for j = i:iterations
        distances(i, j) = spikesDist(cells{i}, cells{j}, 10);
    end
    i
end

hist(nonzeros(distances(:)), 0:0.001:max(max(distances)));

% ---

ONclasses = {};
ONclasses{1} = ONs{1};

classes = zeros(1, iterations);

thresh = 0.2;
for i = 2:iterations
    ON = ONs{i};
    
    bestd = 10000;
    bestclass = 1;
    for j = 1:length(ONclasses)
        d = spkdexp(ON, ONclasses{j}, 10);
        if (d < bestd)
            bestd = d;
            bestclass = j;
        end
    end
    
    if (bestd < thresh)
        classes(bestclass, i) = 1;
    else
        ONclasses{length(ONclasses)+1} = ON;
    end
end


matrix = zeros(length(ONclasses), 400);
for i = 1:length(ONclasses)
    ON = ONclasses{i};
    sync = zeros(1, 400);
    sync(ON) = 1;
    matrix(i, :) = sync;
end

imagesc(matrix);
