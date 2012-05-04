%% H1_Ortiz
%% Matlab Version: R2011a
%--------------------------------------------------------------------------
% Rodrigo Ortiz
% Matricola: 764561
% Date: April 11 - 2012

clear all;
close all;
clc;
currentFolder = pwd;

%% Exercise 1 
% The archive CT.rar, available with the homework, contains a series of 
% cardiac CT images stored in the DICOM format. 
% After unzipping the archive, the student should write a script able to: 
% *************************************************************************
h = msgbox( 'Exercise 1');
uiwait( h );

%% a)
% let the user choose the directory containing the CT slices, 
% using uigetdir(), and load the images in the workspace; 

h = msgbox( 'Choose the path where the cardiac CT images reside');
uiwait( h );

% get the path
path = uigetdir();

% change directory
cd( path );

% list files in the directory
files = dir;

% index for put the image in the struct//a name and load the image in the workspace
index = 1;
structOfCTs = struct('image', {});

% loop for getting all files and try to load them in the workspace
for i=3:length(files)
    try        
        eval(['[X map] = dicomread(''' files(i).name ''');']);        
        structOfCTs(index).image = X;
        index = index + 1;
    catch err
        rethrow(err)
    end
end
eval(['info = dicominfo(''' files(3).name ''');']);     

if index==1 
    errordlg('There was not found any DICOM.', 'Error');
else
    h = msgbox( 'Found DICOMs are loaded in the workspace in the struct structOfCTs.');
    uiwait( h );
end

%% b)
% visualize the CT slices, in group of 16, in 3 separate figures using montage();        

h = msgbox( 'Coming up, we visualize 3 goups of 16 images.');
uiwait( h );

names1 = { files(3:3+16-1).name }';
names2 = { files(19:19+16-1).name }';
names3 = { files(19+16:end).name }';

figure(1)
montage(names1,'DisplayRange', []);

figure(2)
montage(names2,'DisplayRange', []);

figure(3)
montage(names3,'DisplayRange', []);

%% c) 
% visualize the sequence of 48 images in the same figure, with a 
% temporal lag of 0.5 sec between two subsequent slices, using pause(); 

h = msgbox( 'Coming up, the sequence of images');
uiwait( h );

figure(4)
for j=1:index-1
    try
        imshow( structOfCTs(j).image, map );
        pause(0.5);
    catch err
        rethrow(err)
    end
end

%% d) 
% save all the slices in a multilayer TIFF file, giving the user the 
% opportunity to choose the name and the location of the output file. 

eval('cd ..');

h = msgbox( 'All slices will be saved on a .TIFF file');
uiwait( h );

[name path] = uiputfile();
pathNameExt = strcat(path,name,'.TIFF');

if name~=0  
    % obtain RescaleSlope and RescaleIntercept from DICOM info.              
    slope = info.RescaleSlope;
    intercept = info.RescaleIntercept;

    for k=1:length( structOfCTs )
        % calculate imHU values
        imHU = structOfCTs(1).image .* slope + intercept;
        imGray = mat2gray( imHU );  
        imwrite( imGray, pathNameExt,'WriteMode','append');
    end    
end

%% Exercise 2
% Load the images coro.bmp and coro_mod.bmp, available with the homework.  
% The latter image has been created by transforming the histogram of 
% coro.bmp using the  command  imadjust()  with  gamma=1.  
% The student is expected to: 
% *************************************************************************

cd(currentFolder);
h = msgbox( 'Exercise 2');
uiwait( h );

%% a)   
% find an estimate (the best possible!) of the parameters utilized in 
% imadjust(); 

% J = imadjust(I,[low_in; high_in],[low_out; high_out],gamma) maps the 
% values in I to new values in J, where gamma specifies the shape of the 
% curve describing the relationship between the values in I and J.  
% If you omit the argument, gamma defaults to 1 (linear mapping).

coro = imread('coro.bmp', 'bmp');
coro_mod = imread('coro_mod.bmp', 'bmp');

figure
subplot(2,2,1)
imshow(coro)
subplot(2,2,2)
imshow(coro_mod)

h1 = imhist(coro);
h2 = imhist(coro_mod);

subplot(2,2,3)
imhist(coro)
subplot(2,2,4)
imhist(coro_mod)

lowin = 64;
highin = 255;
lowout = 0;
highout = 230;
k = (highout-lowout)/(highin-lowin);

f = @( pto ) pto.*k - lowin*k + lowout*k;
finv = @(pto)((pto + lowin*k - lowout*k)./k);
coro_est = finv( double(coro_mod) );

coroD = double( coro );
error = mean( mean( sqrt((coroD - coro_est).^2) ));

h = msgbox(...
    strcat('Estimated error is (standard deviation): ', num2str(fix(error)), ' pixels' ));
uiwait( h );

figure;
subplot(1,2,1)
imshow( uint8( coro_est) );
title('Estimated image')
subplot(1,2,2)
plot( f(0:255) );
title('Transfer function')

%% Exercise 3
% Load the RGB image peppers.png, available in Matlab. The student should 
% write the code able to: 
% *************************************************************************
h = msgbox( 'Exercise 3');
uiwait( h );

%% a) - b)
% visualize the loaded image; 
% convert the RGB colorspace into the HSV colorspace, visualizing the three 
% components H, S and V in three separate figures using the adequate colormaps

peppers = imread('peppers.png');
[H S V] = rgb2hsv(peppers);
mapHSV = rgb2hsv(peppers);

figure;
subplot(2,2,1)
imshow(peppers)
title('Image in RGB')
subplot(2,2,2)
imshow(H)
title('H')
subplot(2,2,3)
imshow(S)
title('S')
subplot(2,2,4)
imshow(V)
title('V')


%% c)   
% normalize the RGB space and visualize the obtained image. In order to 
% normalize a RGB image, each channel of each pixel has to be normalized 
% by the intensity amplitude of the pixel itself. 

peppersD = double( peppers );

R = peppersD(:,:,1);
G = peppersD(:,:,2);
B = peppersD(:,:,3);
rat = sqrt(R(:,:).^2 + G(:,:).^2 + B(:,:).^2);

nR(:,:) = R(:,:)./rat;
nG(:,:) = G(:,:)./rat;
nB(:,:) = B(:,:)./rat;

peppersN(:,:,1) = nR;
peppersN(:,:,2) = nG;
peppersN(:,:,3) = nB;
figure
imshow(peppersN)
title('Normalized RGB image')

% With RGB normalization we get free of distortions caused by lights and 
% shadows in an image by means of taking the intensity of each component of
% color (R, G and B) to be compere with the others intensities at the 
% same pixel.

%% Exercise 4
% Given the file es4_300312.pdf, please match each image (1-12) with its 
% amplitude spectrum (A-L). 

% *************************************************************************
h = msgbox( 'Exercise 4');
uiwait( h );

im1 = zeros(101,101);
im1(36:66,36:66) = 1;
im2 = zeros(101,101);
im2(50:52,50:52) = 1;
im3 = zeros(101,101);
im3(50:52,36:66) = 1;
im4 = zeros(101,101);
im4(36:66,50:52) = 1;
im5 = immcos(1, 100, 0, 2*pi, 0);
im6 = immcos(1, 100, -pi/3, 1/5, 0);
im7 = immcos(1, 100, -pi/3, 1/15, 0);
im8 = circleInsideSquare(100,50);
im9 = circleInsideSquare(101,16);
im10 = circleInsideSquare(101,5);
im11 = blurrySqr(100, 3, 5);
im12 = blurrySqr(100, 3, 10);

IM1 = myfft_log(im1);
IM2 = myfft_log(im2);
IM3 = myfft_log(im3);
IM4 = myfft_log(im4);
IM5 = myfft(im5);
IM6 = myfft(im6);
IM7 = myfft(im7);
IM8 = myfft_log(im8);
IM9 = myfft_log(im9);
IM10 = myfft_log(im10);
IM11 = myfft(im11);
IM12 = myfft(im12);

% Ploting time!!! -------------

figure
subplot(3,4,1)
imshow(im1)
subplot(3,4,2)
imshow(im2)
subplot(3,4,3)
imshow(im3)
subplot(3,4,4)
imshow(im4)
subplot(3,4,5)
imshow( im5 );
subplot(3,4,6)
imshow(im6);
subplot(3,4,7)
imshow(im7);
subplot(3,4,8)
imshow(im8);
subplot(3,4,9)
imshow(im9);
subplot(3,4,10)
imshow(im10);
subplot(3,4,11)
imshow(im11);
subplot(3,4,12)
imshow(im12);

figure
subplot(3,4,1)
pcolor(IM1)
axis off
shading interp
set(gca,'DataAspectRatio',[1 1 1], 'PlotBoxAspectRatio',[1 1 1])
title('1 -> F')

subplot(3,4,2)
pcolor(IM2)
axis off
shading interp
set(gca,'DataAspectRatio',[1 1 1], 'PlotBoxAspectRatio',[1 1 1])
title('2 -> B')

subplot(3,4,3)
pcolor(IM3)
axis off
shading interp
set(gca,'DataAspectRatio',[1 1 1], 'PlotBoxAspectRatio',[1 1 1])
title('3 -> G')

subplot(3,4,4)
pcolor(IM4)
axis off
shading interp
set(gca,'DataAspectRatio',[1 1 1], 'PlotBoxAspectRatio',[1 1 1])
title('4 -> J')

subplot(3,4,5)
pcolor( IM5 );
axis off
shading interp
set(gca,'DataAspectRatio',[1 1 1], 'PlotBoxAspectRatio',[1 1 1])
title('5 -> L')

subplot(3,4,6)
pcolor(IM6);
axis off
shading interp
set(gca,'DataAspectRatio',[1 1 1], 'PlotBoxAspectRatio',[1 1 1])
title('6 -> A')

subplot(3,4,7)
pcolor(IM7);
axis off
shading interp
set(gca,'DataAspectRatio',[1 1 1], 'PlotBoxAspectRatio',[1 1 1])
title('7 -> K')

subplot(3,4,8)
pcolor(IM8);
axis off
shading interp
set(gca,'DataAspectRatio',[1 1 1], 'PlotBoxAspectRatio',[1 1 1])
title('8 -> C')

subplot(3,4,9)
pcolor(IM9);
axis off
shading interp
set(gca,'DataAspectRatio',[1 1 1], 'PlotBoxAspectRatio',[1 1 1])
title('9 -> E')

subplot(3,4,10)
pcolor(IM10);
axis off
shading interp
set(gca,'DataAspectRatio',[1 1 1], 'PlotBoxAspectRatio',[1 1 1])
title('10 -> I')

subplot(3,4,11)
pcolor(IM11);
axis off
shading interp
set(gca,'DataAspectRatio',[1 1 1], 'PlotBoxAspectRatio',[1 1 1])
title('11- > H')

subplot(3,4,12)
pcolor(IM12);
axis off
shading interp
set(gca,'DataAspectRatio',[1 1 1], 'PlotBoxAspectRatio',[1 1 1])
title('12 -> D')