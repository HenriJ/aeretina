function d = carnellDist(a, b, tau) 
%CARNELLDIST Computes the Carnell distance between two spike trains
%   a and b must be timestamp vectors
%   tau is a time constant smoothing the distance between nearby spikes

    d = 0;
    
    la = length(a);
    lb = length(b);
    
    for i = 1:la
        for j = 1:la
            d = d + exp(-abs(a(i) - a(j))/tau);
        end
    end
    
    for i = 1:lb
        for j = 1:lb
            d = d + exp(-abs(b(i) - b(j))/tau);
        end
    end
    
    for i = 1:la
        for j = 1:lb
            d = d - 2 * exp(-abs(a(i) - b(j))/tau);
        end
    end
    
    d = max(0, d);% / (la^2 + lb^2);
    
end
    