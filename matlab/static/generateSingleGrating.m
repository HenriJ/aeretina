function [ input ] = generateSingleGrating( portx, porty, width, delta, noise )
    centerx = floor(portx/2);

    input = abs(randn(porty, portx).^2) * noise;

    input(1:porty, (centerx + delta - width):(centerx + delta + width)) = input(1:porty, (centerx + delta - width):(centerx + delta + width)) + 1;

    %input = conv2(input, fspecial('gaussian', 2*8, 2), 'same');
    input = input / max(max(input));
end

