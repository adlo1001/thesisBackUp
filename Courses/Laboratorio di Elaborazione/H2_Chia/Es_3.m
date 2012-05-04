% H2_Chia
% 7.8.0.347 (R2009a)

% Exercise 3

close all
clear all
clc

%% 1: 
% Original images
f=imread('h2_test_pattern.tif');
f=double(f);
figure(1)
imshow(f,[])
title('f: Original image')
g=imread('h2_mri_4ch.tif');
g=double(g);

% Amplitud and phase spectra
F=fft2(f);
F_mod=fftshift(abs(F));
F_phase=angle(F);
figure(2);
imshow(log(1+F_mod),[])
title('Amplitude spectrum of F')
figure(3)
imshow(F_phase,[])
title('Phase spectrum of F')

%% 2:
f_phase=ifft2(exp(1i*F_phase));
figure(4)
imshow(f_phase,[])
title('Inverse transform of the phase of F')

%% 3:
f_mod=ifft2(ifftshift(F_mod));
figure(5)
imshow(log(1+f_mod),[])
title('Inverse transform of the module of F')

%% 4:
figure(6)
imshow(g,[])
title('g: Original image')
% Amplitud and phase spectra
G=fft2(g);
G_mod=fftshift(abs(G));
G_phase=angle(G);
figure(7);
imshow(log(1+G_mod),[])
title('Amplitude spectrum of G')
figure(8)
imshow(G_phase,[])
title('Phase spectrum of G')

% Inverse transform of phase
g_phase=ifft2(exp(1i*G_phase));
figure(9)
imshow(abs(g_phase),[])
title('Inverse transform of the phase of G')

% Inverse transform of module
g_mod=ifft2(ifftshift(G_mod));
figure(10)
imshow(abs(log(1+g_mod)),[])
title('Inverse transform of the module of G')

%% 5: 
image1=ifft2(ifftshift(F_mod).*exp(1i*G_phase));
figure(11)
imshow(abs(image1),[])
title('Module of F and phase of G')
image2=ifft2(ifftshift(G_mod).*exp(1i*F_phase));
figure(12)
imshow(image2,[])
title('Module of G and phase of F')

% In Fourier analysis, we decompose a signal in his basic components, being
% each one of them a sine at different frequencies. This sine is
% represented with two values: the amplitude and the initial phase of the
% wave. Both of them are important to reconstruct the signal. As we saw, if
% we only have the phase, we are able to see the silhouette of the image,
% created by constructive or destructive interference of waves. But all of
% these waves have the same amplitude.
% On the other hand, if we only reconstruct with the module, we're not able
% to distinguish anything. 
% This also proves how important the phase is, and we can't modify the 
% of a signal when we filter it. Or at least, add only a constant component
% to it. 

