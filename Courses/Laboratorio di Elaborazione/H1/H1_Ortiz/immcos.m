function A = immcos(amp,N,TETA,FREQ,FI)

    % N = dimensions of A
    % TETA = tilt cosinusoid in respect to x axis in rad
    % FREQ = spatial frequency in cycles/sample (1/FREQ=samples per cycle)
    % FI = phase cosinusoid (e.g. : 0=cos; -pi/2=sin)

    WX=2*pi*cos(TETA)*FREQ; % pulsazione lungo l'asse x
    WY=2*pi*sin(TETA)*FREQ; % pulsazione lungo l'asse y
    for IX=1:N,
        for IY=1:N,
            ICOL=IX; % l'indice di colonna rappresenta la x
            IRIGA=IY; % l'indice di riga rappresenta la y
            A(IRIGA,ICOL)=amp*cos(WX*IX+WY*IY+FI);
        end
    end
end