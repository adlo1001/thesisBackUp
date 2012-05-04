close all; clear all; clc

maxPts = 150;
ptThresh = 1e-3;

%% Step 1. Read Frames from a Movie File
filename = 'taking_off.avi';%'raising_up.avi';

%  Look at source video
hVideoSrc = vision.VideoFileReader(filename, ...
                                      'ImageColorSpace', 'Intensity',...
                                      'VideoOutputDataType', 'double');                                  

%% Step 6. Run on the Full Video

hGTE = vision.GeometricTransformEstimator(...
                        'Transform','Affine',...
                        'InlierOutputPort',true,...
                        'NumRandomSamplings', 1000);
hGT = vision.GeometricTransformer;
hGTPrj = vision.GeometricTransformer;
finfo = info(hVideoSrc);
hvfw = vision.VideoFileWriter('taking_off_output.avi','AudioInputPort',false,'FrameRate', finfo.VideoFrameRate);  

hCD = vision.CornerDetector( ...
    'Method','Local intensity comparison (Rosen & Drummond)', ...
    'MaximumCornerCount', maxPts, ...
    'CornerThreshold', ptThresh, ...
    'NeighborhoodSize', [9 9]);

% Process all frames in the video
movMean = im2single(step(hVideoSrc));
imgB = movMean;
imgBp = imgB;
correctedMean = imgBp;
Hcumulative = eye(3);
%ii = 1;
%nFrames = 300;

while ~isDone(hVideoSrc)% && ii < 10
    % Read in new frame
    imgA = imgB; % z^-1
    imgAp = imgBp; % z^-1
    imgB = im2single(step(hVideoSrc));
    movMean = movMean + imgB;

    % Estimate transform from frame A to frame B, and fit as an s-R-t
    H = cvexEstStabilizationTform(imgA,imgB,hCD,hGT,hGTE);
    HsRt = cvexTformToSRT(H);
    Hcumulative = Hcumulative * HsRt;
    imgBp = step(hGTPrj, imgB, Hcumulative);

    %--------------------------------------------------------    
    % Create AVI file.        
    step(hvfw, cat(3,imgAp,imgBp,imgBp));
    %--------------------------------------------------------            
    correctedMean = correctedMean + imgBp;
    
    %ii = ii+1;
end

release(hVideoSrc);
release(hvfw);