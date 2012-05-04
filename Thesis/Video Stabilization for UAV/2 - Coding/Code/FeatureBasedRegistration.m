%% Feature-Based Registration Demo
close all
clear all
clc

%%  Look at source video
implay('shaky_car.avi');

%% Step 1 - Read Images
imgA = im2single(imread('imageA.tif'));
imgB = im2single(imread('imageB.tif'));

cvexShowImagePair(imgA, imgB, 'Frame A', 'Frame B');

%% Step 2 - Collect Salient Points from Each Image

maxPts = 200;
ptThresh = 1e-3;
hCornerDet = vision.CornerDetector( ...
    'Method','Local intensity comparison (Rosen & Drummond)', ...
    'MaximumCornerCount', maxPts, ...
    'CornerThreshold', ptThresh, ...
    'NeighborhoodSize', [9 9]);
pointsA = step(hCornerDet, imgA);
pointsB = step(hCornerDet, imgB);

cvexShowImagePair(imgA, imgB, 'Corners in A', 'Corners in B', 'SingleColor', pointsA, pointsB);

%% Step 3 - Extract Features around each Point
blockSize = 9;
[featuresA, pointsA] = extractFeatures(imgA, pointsA, 'BlockSize', blockSize);
[featuresB, pointsB] = extractFeatures(imgB, pointsB, 'BlockSize', blockSize);

%% Step 4 - Select Correspondences Between Points
% Match features which were found in the current and the previous frames
indexPairs = matchFeatures(featuresA, featuresB, 'Metric', 'SSD');
numMatchedPoints = cast(size(indexPairs, 2), 'int32');
pointsA = pointsA(:, indexPairs(1, :));
pointsB = pointsB(:, indexPairs(2, :));

cvexShowMatches(imgA, imgB, pointsA, pointsB, 'A', 'B', 'RC');

%% Step 5 - Estimatate Geometric Transformation
% RANSAC offers robust estimation of the transformation parameters by
% randomly sampling from the matched features to identify and remove
% outliers for improved results.
doc 'estimate geometric transformation'

%%
hGTE = vision.GeometricTransformEstimator(...
                        'Transform','Affine',...
                        'InlierOutputPort',true,...
                        'NumRandomSamplings', 1000);
hGT = vision.GeometricTransformer;
hGTPrj = vision.GeometricTransformer;
%
% Run multiple RANSAC trials
nRansacTrials = 3;
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
%
[~,ix] = min(costs);
imgBp = step(hGT, imgB, Ts{ix});
pointsBp = Ts{ix} * [single(pointsB); ones(1,size(pointsB,2))];
H = [Ts{ix}; 0 0 1];
%
cvexShowMatches(imgA, imgB, pointsA(:,inliers{ix}), ...
    pointsB(:,inliers{ix}), 'A', 'B');


