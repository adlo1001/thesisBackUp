function H = cvexEstStabilizationTform(leftI,rightI,hCD,hGT,hGTE,...
        maxPts,ptThresh,nRansacTrials)
%Get inter-image transform and aligned point features.
%  H = cvexEstStabilizationTform(leftI,rightI,HCD,HGT,HGTE) computes an
%  affine transform between leftI and rightI using the
%  |GeometricTransformEstimator| block, and returns the points used as the
%  2-by-npts matrices, pointsLeft and pointsRight.
%
%  (...,maxPts,ptThresh,nRansacTrials) also accepts arguments for the
%  maximum number of points to match, the threshold to use for the corner
%  detector, the number of outer trials to run, and an optional image
%  region of interest for caculating costs in the outer loop.

% Copyright 2010 The MathWorks, Inc.

% Set default parameters
if nargin < 6 || isempty(maxPts)
    maxPts = 250;
end
if nargin < 7 || isempty(ptThresh)
    ptThresh = 1e-3; %#ok<NASGU>
end
if nargin < 8 || isempty(nRansacTrials)
    nRansacTrials = 10;
end

%% Generate prospective points
pointsA = step(hCD, leftI);
pointsB = step(hCD, rightI);

%% Select point correspondences
blockSize = 9; % Block size.

% Extract features for the corners
[featuresA, pointsA] = extractFeatures(leftI, pointsA, ...
    'BlockSize', blockSize);
[featuresB, pointsB] = extractFeatures(rightI, pointsB, ...
    'BlockSize', blockSize);

% Match features which were computed from the current and the previous
% images
indexPairs = matchFeatures(featuresA, featuresB, 'Metric', 'SSD');
numMatchedPoints = cast(size(indexPairs, 2), 'int32');
pointsA = pointsA(:, indexPairs(1, :));
pointsB = pointsB(:, indexPairs(2, :));

pointsA(:, numMatchedPoints+1:maxPts) = 0;
pointsB(:, numMatchedPoints+1:maxPts) = 0;

%% Perform RANSAC procedure with transform estimator block
% Reselect right points in correspondence with left

% Run multiple RANSAC trials
Ts = cell(1,nRansacTrials);
costs = zeros(1,nRansacTrials);
inliers = cell(1,nRansacTrials);

for j=1:nRansacTrials
    % Estimate affine transform
    [Ts{j},inliers{j}] = step(hGTE, pointsB, pointsA, numMatchedPoints);
    
    % Warp image and compute error metric.
    rightIp = step(hGT, rightI, Ts{j});
    costs(j) = sum(sum(imabsdiff(rightIp, leftI)));
end
% Take best result.
[~,ix] = min(costs);
H = [Ts{ix}; 0 0 1];


   
%cvexShowMatches(leftI, rightI, pointsA(:,inliers{ix}), ...
 %   pointsB(:,inliers{ix}), 'A', 'B');

% indexPairs = matchFeatures(featuresA, featuresB, 'Metric', 'SSD');
% numMatchedPoints = cast(size(indexPairs, 2), 'int32');
% pointsA = pointsA(:, indexPairs(1, :));
% pointsB = pointsB(:, indexPairs(2, :));
% 
% cvexShowMatches(imgA, imgB, pointsA, pointsB, 'A', 'B', 'RC');
