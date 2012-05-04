function img = imyfft_log(IMG) 

    M_abs = ifftshift(exp(IMG) - 1);
    img = ifft2( M_abs );
    
end