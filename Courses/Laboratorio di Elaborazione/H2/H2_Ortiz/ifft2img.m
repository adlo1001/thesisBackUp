function M_filtered = ifft2img(M_mod, M_phase)
    
    M_mod_filtered = ifftshift( exp(M_mod) -1 );
    M_filtered = M_mod_filtered.*exp(1i.*M_phase);   
    M_filtered = abs(ifft2(M_filtered)) ;%mat2gray( abs(ifft2(M_filtered)) );
    
end