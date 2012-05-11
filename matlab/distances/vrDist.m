function d=vrDist(a, b, tau)
%VRIDST Computes the Van Rossum distance between two spike trains
%   a and b must be timestamp vectors
%   tau is a time constant smoothing the distance between nearby spikes

    T = max(max(a), max(b));
    
    syncA = zeros(1, T+10*tau);
    syncB = zeros(1, T+10*tau);
    
    syncA(a) = 1;
    syncB(b) = 1;
    
    G = fspecial('gaussian', [1 10*tau], tau);
    
    d = sum((conv(syncA-syncB, G, 'full')).^2);
    
end