% Plot the events of a pixel

clear all;

[x, y, p, t] = aerLoad('p64_d1.dat');

% ------

posX = 77;
posY = 43; % Faire varier pour avoir toute la ligne


for posY = 10:118
    positions = find(((x==posX) & (y==posY)));
    temp = zeros(1, t(end));
    for i = 1:length(positions)
        if p(positions(i)) == 1
            temp(t(positions(i))) = 1;
        end
    end
    posY
end

plot(temp)