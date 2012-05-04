function [M_mod M_phase] = myfft(m) 
    M = fft2(m);
    M_mod = fftshift( abs(M) );
    M_phase = angle(M);
end