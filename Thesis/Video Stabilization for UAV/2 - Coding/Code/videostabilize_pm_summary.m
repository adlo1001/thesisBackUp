% Video Stabilization Using Point Feature Matching
close all
clear all
clc

%% Step 1. Read Frames from a Movie File

filename = 'rotelleri672x512.avi';
implay( filename );

%  Look at source video
hVideoSrc = vision.VideoFileReader(filename, ...
                                      'ImageColorSpace', 'Intensity',...
                                      'VideoOutputDataType', 'double');
                                  
for s=1:35
    imgA = im2single(step(hVideoSrc)); % Read first frame into imgA
end

imgB = im2single(step(hVideoSrc)); % Read second frame into imgB

figure; imshow(cat(3,imgA,imgB,imgB));
title('Color composite (frame A = red, frame B = cyan)');

%% Step 2. Collect Salient Points from Each Frame

maxPts = 150;
ptThresh = 1e-3;
hCD = vision.CornerDetector( ...
    'Method','Local intensity comparison (Rosen & Drummond)', ...
    'MaximumCornerCount', maxPts, ...
    'CornerThreshold', ptThresh, ...
    'NeighborhoodSize', [9 9]);
pointsA = step(hCD, imgA);
pointsB = step(hCD, imgB);

cvexShowImagePair(imgA, imgB, 'Corners in A', 'Corners in B', ...
    'SingleColor', pointsA, pointsB);

%% Step 3 - Extract Features around each Point
%% Step 4. Select Correspondences Between Points

% Extract features for the corners
blockSize = 9; % Block size.
[featuresA, pointsA] = extractFeatures(imgA, pointsA, ...
    'BlockSize', blockSize);
[featuresB, pointsB] = extractFeatures(imgB, pointsB, ...
    'BlockSize', blockSize);

% Match features which were found in the current and the previous frames
indexPairs = matchFeatures(featuresA, featuresB, 'Metric', 'SSD');
numMatchedPoints = cast(size(indexPairs, 2), 'int32');
pointsA = pointsA(:, indexPairs(1, :));
pointsB = pointsB(:, indexPairs(2, :));

cvexShowMatches(imgA, imgB, pointsA, pointsB, 'A', 'B', 'RC');

%% Step 5. Estimating Transform from Noisy Correspondences

hGTE = vision.GeometricTransformEstimator(...
                        'Transform','Affine',...
                        'InlierOutputPort',true,...
                        'NumRandomSamplings', 1000);
hGT = vision.GeometricTransformer;
hGTPrj = vision.GeometricTransformer;

% Run multiple RANSAC trials
nRansacTrials = 1;
Ts = cell(1,nRansacTrials);
costs = zeros(1,nRansacTrials);
nPts = int32(size(pointsA,2));
inliers = cell(1,nRansacTrials);

for j=1:nRansacTrials
    % Estimate affine transform
    [Ts{j},inliers{j}] = step(hGTE, pointsB, pointsA, nPts);
    
    % Warp image and compute error metric.
    imgBp = step(hGT, imgB, Ts{j});
    costs(j) = sum(sum(imabsdiff(imgBp, imgA)));
end
% Take best result.
[~,ix] = min(costs);
imgBp = step(hGT, imgB, Ts{ix});
pointsBp = Ts{ix} * [single(pointsB); ones(1,size(pointsB,2))];
H = [Ts{ix}; 0 0 1];

cvexShowMatches(imgA, imgBp, pointsA(:,inliers{ix}), ...
    pointsBp(:,inliers{ix}), 'A', 'B');

%% Step 5. Transform Approximation and Smoothing

% Extract scale and rotation part sub-matrix.
R = H(1:2,1:2);
% Compute theta from mean of two possible arctangents
theta = mean([atan2(R(2),R(1)) atan2(-R(3),R(4))]);
% Compute scale from mean of two stable mean calculations
scale = mean(R([1 4])/cos(theta));
% Translation remains the same:
translation = H(1:2,3);
% Reconstitute new s-R-t transform:
HsRt = [scale*[cos(theta) -sin(theta); sin(theta) cos(theta)] translation;
        0 0 1];

imgBold = step(hGTPrj, imgB, H);
imgBsRt = step(hGTPrj, imgB, HsRt);
figure(2), clf;
imshow(cat(3,imgBold,imgBsRt,imgBsRt)), axis image;
title('Color composite of affine and s-R-t transform outputs');

%% Step 6. Run on the Full Video
% Now we apply the above steps to smooth a video sequence. For readability,
% the above procedure of estimating the transform between two images has
% been placed in the MATLAB(R) function
% <matlab:edit(fullfile(matlabroot,'toolbox','vision','visiondemos','cvexEstStabilizationTform.m')) |cvexEstStabilizationTform|>.
% The function
% <matlab:edit(fullfile(matlabroot,'toolbox','vision','visiondemos','cvexTformToSRT.m')) |cvexTformToSRT|>
% also converts a general affine transform into a
% scale-rotation-translation transform.
%
% At each step we calculate the transform $H$ between the present frames.
% We fit this as an s-R-t transform, $H_{sRt}$. Then we combine this the
% cumulative transform, $H_{cumulative}$, which describes all camera motion
% since the first frame. The last two frames of the smoothed video are
% shown in a Video Player as a red-cyan composite.
%
% With this code, you can also take out the early exit condition to make
% the loop process the entire video.

% Reset the video source to the beginning of the file.
%reset(hVideoSrc);
hGTE = vision.GeometricTransformEstimator(...
                        'Transform','Affine',...
                        'InlierOutputPort',true,...
                        'NumRandomSamplings', 1000);
hGT = vision.GeometricTransformer;
hGTPrj = vision.GeometricTransformer;
                      
hVPlayer = vision.VideoPlayer; % Create video viewer

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
ii = 2;
Hcumulative = eye(3);
while ~isDone(hVideoSrc) && ii < 20
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

    % Display as color composite with last corrected frame
    step(hVPlayer, cat(3,imgAp,imgBp,imgBp));
    correctedMean = correctedMean + imgBp;
    
    ii = ii+1;
end
correctedMean = correctedMean/(ii-2);
movMean = movMean/(ii-2);

% Here you call the release method on the objects to close any open files
% and release memory.
release(hVideoSrc);
release(hVPlayer);

%%
% During computation, we computed the mean of the raw video frames and of
% the corrected frames. These mean values are shown side-by-side below. The
% left image shows the mean of the raw input frames, proving that there was
% a great deal of distortion in the original video. The mean of the
% corrected frames on the right, however, shows the image core with almost
% no distortion. While foreground details have been blurred (as a necessary
% result of the car's forward motion), this shows the efficacy of the
% stabilization algorithm.

cvexShowImagePair(movMean, correctedMean, ...
    'Raw input mean', 'Corrected sequence mean');

%% References
% [1] Tordoff, B; Murray, DW. "Guided sampling and consensus for motion
% estimation." European Conference n Computer Vision, 2002.
%
% [2] Lee, KY; Chuang, YY; Chen, BY; Ouhyoung, M. "Video Stabilization
% using Robust Feature Trajectories." National Taiwan University, 2009.
% 
% [3] Litvin, A; Konrad, J; Karl, WC. "Probabilistic video stabilization
% using Kalman filtering and mosaicking." IS&T/SPIE Symposium on Electronic
% Imaging, Image and Video Communications and Proc., 2003.
%
% [4] Matsushita, Y; Ofek, E; Tang, X; Shum, HY. "Full-frame Video
% Stabilization." Microsoft(R) Research Asia. CVPR 2005.

displayEndOfDemoMessage(mfilename)