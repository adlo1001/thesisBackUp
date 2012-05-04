close all
clear all
clc

%% Video Stabilization Using Point Feature Matching
% ( feature based registration)

% Stabilizing a video that was captured from a jittery or moving platform
% is an important application in computer vision. One way to stabilize a
% video is to track a salient feature in the image and use this as an
% anchor point to cancel out all perturbations relative to it. This
% procedure, however, must be bootstrapped with knowledge of where such a
% salient feature lies in the first video frame. In this demo, we explore a
% method of video stabilization that works without any such _a priori_
% knowledge. It instead automatically searches for the "background plane"
% in a video sequence, and uses its observed distortion to correct for
% camera motion.
%
% This stabilization algorithm involves two steps. First, we determine the
% affine image transformations between all neighboring frames of a video
% sequence using a Random Sampling and Consensus (RANSAC) [1] procedure
% applied to point correspondences between two images. Second, we warp the
% video frames to achieve a stabilized video. We will use System objects in
% the Computer Vision System Toolbox(TM), both for the algorithm and for
% display.
%
% This demo is similar to the <videostabilize.html Video Stabilization Demo>. 
% The main difference is that the Video Stabilization Demo is given a
% region to track while this demo is given no such knowledge. Both demos
% use the same example video.

%% Step 1. Read Frames from a Movie File
% Here we read in the first two frames of a video sequence using the
% |BinaryFileReader| System object. We read them as intensity images
% since color is not necessary for the stabilization algorithm, and because
% using grayscale images improves speed. Below we show both frames side by
% side, and we produce a red-cyan color composite to demonstrate the
% pixel-wise difference between them. There is obviously a large vertical
% and horizontal offset between the two frames.

filename = 'raising_up.avi';%rotelleri672x512.avi';
%implay( filename );

%  Look at source video
hVideoSrc = vision.VideoFileReader(filename, ...
                                      'ImageColorSpace', 'Intensity',...
                                      'VideoOutputDataType', 'double');
                                  
% hVideoSrc = vision.BinaryFileReader('shaky_car.bin', ...
%     'FourCharacterCode', 'GREY', ...
%     'OutputSize', [240 320]);

%for s=1:35
    imgA = im2single(step(hVideoSrc)); % Read first frame into imgA
%end
imgB = im2single(step(hVideoSrc)); % Read second frame into imgB

cvexShowImagePair(imgA, imgB, 'Frame A', 'Frame B');

%%
figure; imshow(cat(3,imgA,imgB,imgB));
title('Color composite (frame A = red, frame B = cyan)');

%% Step 2. Collect Salient Points from Each Frame
% Our goal is to determine a transformation that will correct for the
% distortion between the two frames. We can use the
% |GeometricTransformEstimator| System object for this, which will return
% an affine transform. As input we must provide this object with a set of
% point correspondences between the two frames. To generate these
% correspondences, we first collect points of interest from both frames,
% then select likely correspondences between them.
%
% In this step we produce these candidate points for each frame. To have
% the best chance that these points will have corresponding points in the
% other frame, we want points around salient image features such as
% corners. For this we use the |CornerDetector| System object. The FAST
% corner detector algorithm, which we have selected below is one of the
% fastest options.
%
% The detected points from both frames are shown in the figure below.
% Observe how many of them cover the same image features, such as points
% along the tree line, the corners of the large road sign, and the corners
% of the cars.

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
% Next we pick correspondences between the points derived above. For each
% point, we extract a 9-by-9 block centered around it. The matching cost we
% use between points is the sum of squared differences (SSD) between their
% respective image regions. Points in frame A and frame B are matched
% putatively. Note that there is no uniqueness constraint, so points from
% frame B can correspond to multiple points in frame A.

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

% The image below shows the same color composite given above, but added are
% the points from frame A in red, and the points from frame B in green.
% Yellow lines are drawn between points to show the correspondences
% selected by the above procedure. Many of these correspondences are
% correct, but there is also a significant number of outliers.

cvexShowMatches(imgA, imgB, pointsA, pointsB, 'A', 'B', 'RC');

%% Step 5. Estimating Transform from Noisy Correspondences
% Many of the point correspondences obtained in the previous step are
% incorrect. But we can still derive a robust estimate of the geometric
% transform between the two images using the Random Sample Consensus
% (RANSAC) algorithm [1], which is implemented in the
% |GeometricTransformEstimator| System object. This object, when given a
% set of point correspondences, will search for the valid inlier
% correspondences. From these it will then derive the affine transform that
% makes the inliers from the first set of points match most closely with
% the inliers from the second set. This affine transform will be a 3-by-3
% matrix of the form:
%
%  [a_1 a_3 t_r;
%   a_2 a_4 t_c;
%     0   0   1]
%
% The parameters $a$ define scale, rotation, and sheering effects of the
% transform, while the parameters $t$ are translation parameters. This
% transform can be used to warp the images such that their corresponding
% features will be moved to the same image location.
%
% A limitation of the affine transform is that it can only alter the
% imaging plane. Thus it is ill-suited to finding the general distortion
% between two frames taken of a 3-D scene, such as with this video taken
% from a moving car. But it does work under certain conditions that we
% shall describe shortly.
%
% We implement this procedure below. For added robustness, we run the 
% |GeometricTransformEstimator| System object multiple times and calculate
% a cost for each result. This cost is obtained by projecting frame B onto
% frame A according to the derived transform, and taking the sum of
% absolute difference (SAD) between the two images. We take the best
% transform as the one that minimizes this cost.

% Step 5 - Estimatate Geometric Transformation
% RANSAC offers robust estimation of the transformation parameters by
% randomly sampling from the matched features to identify and remove
% outliers for improved results.

%doc 'estimate geometric transformation'

hGTE = vision.GeometricTransformEstimator(...
                        'Transform','Affine',...
                        'InlierOutputPort',true,...
                        'NumRandomSamplings', 1000);

hGT = vision.GeometricTransformer;
hGTPrj = vision.GeometricTransformer;
% hGT.ROIShape --> Rectangle ROI

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

%%
% Below is a color composite showing frame A overlaid with the reprojected
% frame B, along with the reprojected point correspondences. The results
% are excellent, with the inlier correspondences nearly exactly coincident.
% The cores of the images are both well aligned, such that the red-cyan
% color composite becomes almost purely black-and-white in that region.
%
% Note how the inlier correspondences are all in the background of the
% image, not in the foreground, which itself is not aligned. This is
% because the background features are distant enough that they behave as if
% they were on an infinitely distant plane. Thus, even though the affine
% transform is limited to altering only the imaging plane, here that is
% sufficient to align the background planes of both images. Furthermore, if
% we assume that the background plane has not moved or changed
% significantly between frames, then this transform is actually capturing
% the camera motion. Therefore correcting for this will stabilize the
% video. This condition will hold as long as the motion of the camera
% between frames is small enough, or, conversely, if the sample time of the
% video is high enough.

cvexShowMatches(imgA, imgBp, pointsA(:,inliers{ix}), ...
    pointsBp(:,inliers{ix}), 'A', 'B');

%% Step 5. Transform Approximation and Smoothing
% Given a set of video frames $T_{i}, \quad i=0,1,2 \ldots$, we can now use
% the above procedure to estimate the distortion between all frames $T_i$
% and $T_{i+1}$ as affine transforms, $H_i$. Thus the cumulative distortion
% of a frame $i$ relative to the first frame will be the product of all the
% preceding inter-frame transforms, or
%
% $H_{cumulative,i} = \prod_{j=0}^{i-1} H_i$
%
% We could use all the six parameters of the affine transform above, but,
% for numerical simplicity and stability, we choose to re-fit the matrix as
% a simpler scale-rotation-translation transform. This has only four free
% parameters compared to the full affine transform's six: one scale factor,
% one angle, and two translations. This new transform matrix is of the
% form:
%
%  [s*cos(ang) s*-sin(ang) t_x;
%   s*sin(ang)  s*cos(ang) t_y;
%            0           0   1]
%
% We demonstrate this conversion procedure below by fitting the
% above-obtained transform $H$ with a scale-rotation-translation
% equivalent, $H_{sRt}$. To show that the error of converting the transform
% is minimal, we reproject frame B with both transforms and show the two
% images below as a red-cyan color composite. As the image appears black
% and white, obviously the pixel-wise difference between the different
% reprojections is negligible.

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
while ~isDone(hVideoSrc) && ii < 50
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
