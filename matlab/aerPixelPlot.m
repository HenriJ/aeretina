% Plot the events of a pixel

clear all;

[x, y, p, t] = aerLoad('200stepsmove_10000speed_p64.dat');

% ------

cols = {'red', 'green', 'blue', 'black', 'yellow'};

posX = 79;
posY = 40; % Faire varier pour avoir toute la ligne
k = 0;

for posX = 61:65
    k = k + 1;
    
    positions = find(((x==posX) & (y==posY)));
    temp = zeros(1, t(end));
    for i = 1:length(positions)
        if p(positions(i)) == 1
            temp(t(positions(i))) = 1;
        end
    end

    plot(temp(2*10^6:3*10^6), cols{k}), hold on;

end