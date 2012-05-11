
function times = aerInterspike(x, y, t, p)

    times = [];
    
    tpos1 = t(p==1);
    xpos1 = x(p==1);
    ypos1 = y(p==1);

    for posX = 1:128
        tpos1posx = tpos1(xpos1==posX);
        ypos1posx = ypos1(xpos1==posX);
        
        for posY = 1:128
            temp = tpos1posx(ypos1posx==posY);
            times = [times; diff(temp)];
        end
    end

end



% function times = aerInterspike(x, y, t, p)
% 
%     times = [];
% 
%     for posX = 1:128
%         for posY = 1:128
%             temp = t((x==posX) & (y==posY) & (p==1));
%             times = [times; diff(temp)];
%         end
%         posX
%     end
% 
% end