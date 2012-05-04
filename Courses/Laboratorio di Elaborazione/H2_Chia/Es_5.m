% H2_Chia
% 7.8.0.347 (R2009a)

% Exercise 5

close all
clear all
clc

%% 1: 
% Original image
img=imread('h2_mri_4ch.tif');
img=double(img);
img_fft=fftshift(fft2(img));
figure(1)
imshow(img,[])
title('Original image')
figure(2)
mesh(log(1+abs(img_fft)))
colormap(jet)
title('Spectrum of the original image')

% Average filter of 5 pixels
average5=fspecial('average',5);
% Average filter of 15 pixels
average15=fspecial('average',15);
% Gaussian filter
gaussian=fspecial('gaussian',6*2,2);
% Unsharp filter
unsharp=fspecial('unsharp');

figure(3)
subplot(221)
freqz2(average5)
title('Average 5 pixels')
subplot(222)
freqz2(average15)
title('Average 15 pixels')
subplot(223)
freqz2(gaussian)
title('Gaussian')
subplot(224)
freqz2(unsharp)
title('Unsharp')

%% 2:
img1=imfilter(img,average5,'same');
img1_fft=fftshift(fft2(img1));
img2=imfilter(img,average15,'same');
img2_fft=fftshift(fft2(img2));
img3=imfilter(img,gaussian,'same');
img3_fft=fftshift(fft2(img3));
img4=imfilter(img,unsharp,'same');
img4_fft=fftshift(fft2(img4));

figure('Position', get(0,'Screensize'));
subplot(221)
mesh(log(1+abs(img1_fft)))
title('Spectrum of image filtered with average of 5 pixels')
subplot(222)
mesh(log(1+abs(img2_fft)))
title('Spectrum of image filtered with average of 15 pixels')
subplot(223)
mesh(log(1+abs(img3_fft)))
title('Spectrum of image filtered with gaussian')
subplot(224)
mesh(log(1+abs(img4_fft)))
title('Spectrum of image filtered with unsharp')
colormap(jet)

%% 3:
figure(5)
subplot(221)
imshow(img1,[])
title('Image filtered with average of 5 pixels')
subplot(222)
imshow(img2,[])
title('Image filtered with average of 15 pixels')
subplot(223)
imshow(img3,[])
title('Image filtered with gaussian')
subplot(224)
imshow(img4,[])
title('Image filtered with unsharp')