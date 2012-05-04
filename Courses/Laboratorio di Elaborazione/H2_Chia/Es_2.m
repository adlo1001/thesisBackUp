% H2_Chia
% 7.8.0.347 (R2009a)

% Exercise 2

close all
clear all
clc

% Original image
img=imread('h2_PET_image.tif');
img=double(img);
figure (1)
imshow(img,[])
title ('Original image')

% Low-attenuating filter
[rows,cols]=size(img);
center_r=floor(rows/2);
center_c=floor(cols/2);

H=zeros(rows,cols);

A=0.25;
B=2;
C=2;
D0=min(rows,cols)/8;

for u=1:rows
    for v=1:cols
        D=sqrt((u-center_r)^2+(v-center_c)^2);
        H(u,v)=A+(C/(1+(D0/(D))^B));
    end
end
figure(2)
mesh(H)
title('Low attenuating filter')

% Homomorphic filtering
z=log(1+img);
Z=fftshift(fft2(z));
S=Z.*H;
s=ifft2(ifftshift(S));
g=exp(s)-1;
figure (3)
imshow(g,[])
title ('Filtered image')