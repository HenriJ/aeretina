function [T,spks,excitation,inhibition,spiking]=squareAnalysis2D(events,tauOnEx,tauOffEx,polarity, rc, rs,alphaIn,alphaOnOffIn, tauOnIn,tauOffIn, delayOnIn, delayOffIn,ra,rgc) 
    events=double(events);
    tic
    %events=events(:,1000:end-2);
    events=events(:,1000:end); % Premiers evenements à retirer
    events(4,:)=events(4,:)-events(4,1); % Décale le vecteur de temps pour mettre le premier à 0
    % Increment temporel entre deux evenements
    
    
    timeStep=2000; % en us, reoslution temporelle finale
    
    events(3,:)=2*double(events(3,:))-1; % (0, 1) => (-1, 1)
    
    threshold=1; % seuil de spike
    binSize=1000; % 
    
    n=floor((events(4,end)-events(4,1))/timeStep); % Taille finale des vecteurs de sortie
    T=double([0:n-1]*timeStep);%vecteur des temps en us
    
    
    Th=T(T<4*max([tauOnEx,tauOffEx, tauOnIn,tauOffIn])); %vecteur utilisé pour le filtre temporel.
    
    % H = filtre temporel
    HonEx=Th/tauOnEx^2.*exp(-Th/tauOnEx);
    HoffEx=Th/tauOffEx^2.*exp(-Th/tauOffEx);
    HonIn=Th/tauOnIn^2.*exp(-Th/tauOnIn);
    HoffIn=Th/tauOffIn^2.*exp(-Th/tauOffIn);   
       
    
    z=meshgrid(0:2*ra);
    u=meshgrid(0:2*rgc);
    
    amacrine=exp(-(z-ra).^2/ra^2);
    gc=exp(-(u-rgc).^2/rgc^2);
    excitation=zeros(128,128,n+length(Th)-1);
    inhibition=zeros(128,128,n+length(Th)-1);
    
    for x=1:128;
        waitbar(x/128);
        for y=1:128;
        cRange=((events(1,:)-x)<=rc)&((x-events(1,:))<=rc)&((events(2,:)-y)<=rc)&((y-events(2,:))<=rc);
        sRange=((events(1,:)-x)<=rs)&((x-events(1,:))<=rs)&((events(2,:)-y)<=rs)&((y-events(2,:))<=rs);
        
        cTemp=events(:,cRange);
        sTemp=events(:,sRange);
        
        cResponse=zeros(1,n);
        sResponse=zeros(1,n);
                
        
        % events
        % (x, y, p, t)
        
        for i=1:n   
            cResponse(i)=(sum(cTemp(3,(cTemp(4,:)>(i-1)*timeStep)&(cTemp(4,:)<(i-1)*timeStep+binSize))));
            sResponse(i)=(sum(sTemp(3,(sTemp(4,:)>(i-1)*timeStep)&(sTemp(4,:)<(i-1)*timeStep+binSize)))); 
        end
            
        CSresponse=(cResponse-((2*rc+1)/(2*rs+1))^2*sResponse);
        % parties positive et négative du signal pour les cellules bipolaires on et
        % off
        on=max(CSresponse,0);
        off=max(-CSresponse,0);
       
        onInResponse=conv(on,HonIn);
        offInResponse=conv(off,HoffIn);
  
        onInResponse((delayOnIn)/timeStep+1:n)=onInResponse(1:n-(delayOnIn)/timeStep);
        onInResponse(1:(delayOnIn)/timeStep)=0;
        offInResponse((delayOffIn)/timeStep+1:n)=offInResponse(1:n-(delayOffIn)/timeStep);
        offInResponse(1:(delayOffIn)/timeStep)=0;       
        
        CSex=(cResponse-((2*rc+1)/(2*rs+1))^2*sResponse);
        inh=onInResponse+alphaOnOffIn*offInResponse;
        
        if polarity==0,
            exc=conv(on,HonEx)+conv(off,HoffEx);
        elseif polarity==1
            CSex=conv(CSresponse,HonEx);
            exc=max(polarity*CSex,0);
        elseif polarity==-1
            CSex=conv(CSresponse,HoffEx);
            exc=max(polarity*CSex,0);          
        end 
        excitation(x,y,:)=exc;
        inhibition(x,y,:)=inh;
        end
    end
    excitation=convn(excitation,gc','same');
    excitation=excitation/(max(max(max(excitation))));
    inhibition=convn(inhibition,amacrine','same');
    inhibition=alphaIn*inhibition/max(max(max(inhibition)));
    spiking=max(excitation-(rgc/ra)^2*inhibition,0);
    spks=(diff(floor(spiking/threshold),1,3))==1;
    cnt=0;
%     while (length(find(spks))<(200*n*timeStep*128*128/1000000)&&(cnt<30))
%         cnt=cnt+1;
%         threshold=threshold/2;
%         spks=(diff(floor(spiking/threshold),1,3))==1;
%     end
%     threshold
%     [X1,Y1]=find(spks);
%     P1=ones(length(X1),1);
%     Y=rem(Y1,128);
%     T1=floor((Y1-1)/128)*timeStep;
%     outEv=[X1,Y,P1,T1]';
%     
    
    toc
%     figure,    
% %     subplot(1,3,1), imagesc(excitation(1,:,:)), axis off
% %     subplot(1,3,2), imagesc(inhibition(1,:,:)), axis off
% %     subplot(1,3,3), imagesc(spiking(1,:,:)), axis off
%     whitebg('w');










