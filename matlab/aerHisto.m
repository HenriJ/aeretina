% Count number of events for each pixels

function histo = aerHisto(x, y)

    histo = zeros(128,128);

    for e = 1:length(x)
        i = x(e) + 1;
        j = y(e) + 1;
        histo(i, j) = histo(i, j) + 1;
    end
    clear e i j;

    % ------

    imagesc(histo);

end
