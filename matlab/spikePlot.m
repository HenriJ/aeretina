function sync = spikePlot( spikes )
    
    sync = zeros(1, max(abs(spikes)));
    sync(nonzeros(real(spikes))) = 1;
    sync(nonzeros(imag(spikes))) = -1;

    plot(sync);
end

