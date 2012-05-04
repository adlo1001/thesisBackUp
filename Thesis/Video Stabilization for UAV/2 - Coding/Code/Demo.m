
%% Step 1. Read Frames from a Movie File

hVideoSrc = vision.BinaryFileReader('shaky_car.bin', ...
    'FourCharacterCode', 'GREY', ...
    'OutputSize', [240 320]);

imgA = im2single(step(hVideoSrc)); % Read first frame into imgA
imgB = im2single(step(hVideoSrc)); % Read second frame into imgB

cvexShowImagePair(imgA, imgB, 'Frame A', 'Frame B');

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

%% Step 3. Select Correspondences Between Points

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

%% Step 4. Estimating Transform from Noisy Correspondences

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

% Reset the video source to the beginning of the file.
reset(hVideoSrc);
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
while ~isDone(hVideoSrc) && ii < 10
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

cvexShowImagePair(movMean, correctedMean, ...
    'Raw input mean', 'Corrected sequence mean');

