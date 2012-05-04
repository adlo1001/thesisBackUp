function [M_mod M_phase] = myfft_log(m) 
    M = fft2(m);      
    M_mod = log( 1 +  fftshift(abs(M)) );    
    M_phase = angle(M);
end