% [input_output_count cellClasses] = dvsMonteCarlo(10:40, 0.01, 0.001, 1000);
function [input_output_count cellClasses] = dvsMonteCarlo( deltas, noiseratio, class_threshold, iterations )
    cellClasses = {};
    
    h = waitbar(0,'Monte Carlo');

    input_output_count = zeros(length(deltas), 3); % delta, cellclassid => occurences

    for d = 1:length(deltas);

        delta = deltas(d);

        ss = generateSignal( delta );

        for iter = 1:iterations
            spikes = pixelSimu(ss, noiseratio, 0.2);

            bestd = 10000;
            bestclass = 1;
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

        waitbar(d/length(deltas), h, strcat('Classes : ', int2str(length(cellClasses))))
        %[deltas(d) length(cellClasses)]
    end
    
    close(h);

end

