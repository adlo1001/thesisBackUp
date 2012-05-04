% H2_Chia
% 7.8.0.347 (R2009a)

% Exercise 1

close all
clear all
clc

%% a: 
% Original image
img=imread('h2_CT_head_corrupted.tif');
img=double(img);
figure(1)
imshow(img,[])
title('Original image')
img_fft=fft2(img);
img_fft_mod=fftshift(abs(img_fft));
figure (2)
subplot(121)
imshow(log(1+img_fft_mod),[]);
title('Fourier transform of the original image')
subplot(122)
mesh(log(1+img_fft_mod))
colormap(jet)
title('3D mesh')

% Band-reject filter
band_reject=ones(size(img));

f=linspace(-1,1,9); % First, we create a model of a low-pass filter. 
% In this case, the noise bandwith is really thin, only one-pixel wide.
% If the sine wasn't that ideal (or if it's windowed), we should have
% created a gaussian with more bandwith.
y=gaussmf(f,[0.15 0]);
Hd = ones(9).*repmat(y,9,1).*repmat(y',1,9);

matrix=zeros(size(img_fft)); % Now we create a matrix with 1s at the center of the sinuisoidal noise
matrix(217,217)=1;
matrix(297,297)=1;
matrix(257,247)=1;
matrix(257,267)=1;
matrix(237,257)=1;
matrix(277,257)=1;

% band_pass=conv2(matrix,Hd,'same'); % And we convolve this matrix with the low-pass filter
band_pass=matrix;
band_reject=1-band_pass;
figure(3)
mesh(1+band_reject)
title('Band reject filter')

% Filtering
img_rest_fft_mod=img_fft_mod.*band_reject;
figure(4)
subplot(121)
imshow(log(1+img_rest_fft_mod),[]);
title('Fourier transform of the restored image')
subplot(122)
mesh(log(1+img_rest_fft_mod));
title('3D mesh')
colormap(jet)

% Restored image
img_rest_fft=ifftshift(img_rest_fft_mod).*exp(1i*angle(img_fft));
img_rest=ifft2(img_rest_fft);
figure (5)
imshow(img_rest,[])
title ('Restored image')

%% b: 
% Band-pass filter
figure (6)
mesh(band_pass)
title ('Band-pass filter')

% Noise
noise_fft_mod=img_fft_mod.*band_pass;
noise_fft=ifftshift(noise_fft_mod).*exp(1i*angle(img_fft));
noise=ifft2(noise_fft);
figure (7)
imshow(noise,[])
title ('Noise')