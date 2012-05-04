%% H2_Ortiz
%% Matlab Version: R2011b
%--------------------------------------------------------------------------
% Rodrigo Ortiz
% Matricola: 764561
% Date: May 02 - 2012

clear all
close all
clc

%% Exercise 1

h = msgbox('Exercise 1');
uiwait(h)

img = imread('h2_CT_head_corrupted.tif');
[M_mod M_phase] = myfft_log(img);%log( 1 +  fftshift(abs(M)) );    

W = ones(size(M_mod ));
W(217,217) = 0;
W(297,297) = 0;
W(237,257) = 0;
W(277,257) = 0;
W(257,247) = 0;
W(257,267) = 0;
Winv = ones(size(W)) - W;

M_mod_c = M_mod.*W;
Noise = M_mod.*Winv;

m_filtered = ifft2img( M_mod_c, M_phase);
filtered_noise = ifft2img( Noise, M_phase);

figure; 
imshow(img)
title('Original Image')

figure 
subplot(121)
imshow( mat2gray(M_mod) )
title('Module of Fourier transform of Original Image')
subplot(122)
mesh(M_mod)
colormap('Hot')

figure; 
mesh( W )
title('Freq response of Band-reject filter')

figure 
subplot(121)
imshow( mat2gray(M_mod_c) )
subplot(122)
mesh(M_mod_c )
colormap('Hot')
title('Module of Fourier transform of retreived Image')

figure
imshow(mat2gray(m_filtered))
title('Restored image')

figure
mesh( Winv )
title('Freq response of Band-reject filter')

figure
imshow(mat2gray(filtered_noise))
title('Filtered Noise')

%% Exercise 2

clear all;
h = msgbox('Exercise 2');
uiwait(h);

img = imread('h2_PET_image.tif');
[N M] = size(img);
H = zeros(N,M);

c_row = fix(N/2);
c_col = fix(M/2);

A = 0.25;
B = 2;
C = 2;
D0 = min(N,M)/8;

for u=1:N
    for v=1:M
        D = sqrt( (u-c_row)^2 + (v-c_col)^2 );
        H(u,v) = A + C/( 1 + (D0/D)^B);
    end
end

IMG = fftshift( fft2( log(1+double(img)) ));
img_filtered = exp( ifft2(ifftshift( H.*IMG)) )-1;

figure
imshow(img)
title('Original Image')
figure
subplot(121)
imshow(mat2gray(H))
title('Filter Frequency spectro')
subplot(122)
mesh(H)
colormap('Hot')
figure
imshow(img_filtered,[])
title('Enhanced image')

%% Excersise 3

clear all;
h = msgbox('Exercise 3');
uiwait(h);

f = imread('h2_test_pattern.tif');
g = imread('h2_mri_4ch.tif');

% 1
[F_mod F_phase] = myfft_log( double(f) );

figure
imshow(f)
title('f')
figure
imshow( F_mod, [] )
title('Power Spectra of f')
figure
imshow(F_phase,[])
title('Phase Spectra of f')

% 2
figure
imshow( ifft2(exp(1j*F_phase)), [] )
title('Recovered f just with the phase')

% 3
figure
imshow( uint8(ifft2img(F_mod,0)) )
title('Recovered f just with the Margnitud Spectrum')

% 4
[G_mod G_phase] = myfft_log( double(g) );
 
figure
imshow(g)
title('g')
figure
imshow( G_mod, [] )
title('Power Spectra of g')
figure
imshow(F_phase,[])
title('Phase Spectra of g')

figure
imshow( ifft2(exp(1j*G_phase)), [] )
title('Recovered g just with the phase')
 
figure
imshow( uint8(ifft2img(G_mod,0)) )
title('Recovered f just with the Margnitud Spectrum')
%
figure
subplot(121)
imshow( ifft2img(F_mod, G_phase), []  )
title('Recovered image with Magnitud of f and phase of g')
subplot(122)
imshow( ifft2img(G_mod, F_phase), []  )
title('Recovered image with Magnitud of g and phase of f')

%% Exercise 4

clear all;
h = msgbox('Exercise 4: Check out the file Ex4_Ortiz');
uiwait(h);

load h2_noisy_images.mat
figure, imshow(img1,[ ])
figure, imshow(img2,[ ])
figure, imshow(img3,[ ])
figure, imshow(img4,[ ])


%% Exercise 5

clear all;
h = msgbox('Exercise 5');
uiwait(h);

%1
img = imread('h2_mri_4ch.tif');
[IMG_mod IMG_phase] = myfft_log( img );

fil1 = fspecial('average',5);
fil2 = fspecial('average',15);
fil3 = fspecial('gaussian',12,2);
fil4 = fspecial('unsharp');

img1 = imfilter(img, fil1);
img2 = imfilter(img, fil2);
img3 = imfilter(img, fil3);
img4 = imfilter(img, fil4);

%________

figure
imshow(img)
title('Original image')
figure
mesh(IMG_mod)
title('Image Spectrum')

figure
subplot(221)
freqz2(fil1)
title('Filter: Average 5')
subplot(222)
freqz2(fil2)
title('Filter: Average 5')
subplot(223)
freqz2(fil3) 
title('Filter: Gaussian')
subplot(224)
freqz2(fil4)
title('Filter: Unsharp')


%2
figure
subplot(221)
mesh( myfft_log(img1))
title('Image Spectrum: filtered with average 5')
subplot(222)
mesh( myfft_log(img2))
title('Image Spectrum: filtered with average 15')
subplot(223)
mesh( myfft_log(img3))
title('Image Spectrum: filtered with gaussian')
subplot(224)
mesh( myfft_log(img4))
title('Image Spectrum: filtered with unsharp')

%3
figure
subplot(221)
imshow( img1)
title('Filter image with average 5')
subplot(222)
imshow( img2)
title('Filter image with average 15')
subplot(223)
imshow( img3 )
title('Filter image with gaussian')
subplot(224)
imshow( img4 )
title('Filter image with unsharp')