% Load events

function [ x y p t ] = aerLoad( path, offset )



    [allAddr, t] = loadaerdat2(path);
    [x, y, p] = extractRetinaEventsFromAddr(allAddr);

    if exist('offset') > 0
        x = x(offset:end);
        y = y(offset:end);
        p = p(offset:end);
        t = t(offset:end);
    end
end



