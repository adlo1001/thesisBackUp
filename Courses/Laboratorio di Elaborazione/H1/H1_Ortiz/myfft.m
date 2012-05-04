function M = myfft(M) 
    M = fftshift( abs(fft2(M)) );
end