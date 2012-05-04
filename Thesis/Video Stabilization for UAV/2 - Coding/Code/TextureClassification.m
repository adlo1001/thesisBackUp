%% Texture Classification Demo
% Copyright 2011 The MathWorks, Inc.

%% Show example image of each material type
load dbInfo
materials = [];
figure; subplot(2,5,1); 
for k = 1:length(dbInfo)
    filename = fullfile(dbInfo(k).path, dbInfo(k).files{1});
    subplot(2,5,k);
    imshow(imread(filename));     
    materials{k} = regexprep(dbInfo(k).material,'_',' ');
    title (materials{k});

end

%% Using texture as a feature
% Grayscale co-occurence matrix provides a compact way to describe the
% texture in an image.
filename = fullfile(dbInfo(1).path, dbInfo(1).files{1});
I = rgb2gray(imread(filename));
glcm = graycomatrix(I);
figure;
imagesc(glcm);
axis image;
title (['Co-occurrence matrix for ', materials{1}]);
colorbar;

%% Let's look at a grayscale co-occurence matrix from each material
% category

figure; subplot(2,5,1); 
for k = 1:length(dbInfo)
    filename = fullfile(dbInfo(k).path, dbInfo(k).files{1});
    I = rgb2gray(imread(filename));
    glcm = graycomatrix(I);
    subplot(2,5,k);
    imagesc(glcm);
    title (materials{k});
    axis image;
    axis off;
end

%% Calculate texture for each image and load into MATLAB as descriptors
% load TextureFeatureData   % for data stored in a MAT file

textureDescriptors = [];
textureMaterial = [];
for k = 1:length(dbInfo)
    matInfo = dbInfo(k);
    for imId = 1:length(matInfo.files)
        filename = fullfile(matInfo.path, matInfo.files{imId});
        I=rgb2gray(imread(filename));
        glcm = graycomatrix(I);
        textureDescriptors = [textureDescriptors; glcm(:)'];     
    end
    textureMaterial = [textureMaterial; ones(length(matInfo.files),1)*k];
end
%%
figure; plot(textureDescriptors(1,:));
title (regexprep(dbInfo(1).material,'_',' '));

%% Partition Data into training and test sets
c = cvpartition(textureMaterial,'holdout',.5);

X_Train = textureDescriptors(training(c,1),:);
Y_Train = nominal(textureMaterial(training(c,1)));

X_Test = textureDescriptors(test(c,1),:);
Y_Test = nominal(textureMaterial(test(c,1)));

%% Let's try Classification Tree
ctObj = ClassificationTree.fit(X_Train,Y_Train);

predicted = predict(ctObj,X_Train);
missClassified = sum(Y_Train~=predicted);
r1 = 100*(1- missClassified/size(Y_Train,1));
display(['Training accuracy = ',num2str(r1),'%']);

predicted = predict(ctObj,X_Test);
missClassified = sum(Y_Test~=predicted);
r2 = 100*(1- missClassified/size(Y_Test,1));
display(['Test accuracy = ',num2str(r2),'%']);

%% Look at confusion matrix
[conf, classorder] = confusionmat(Y_Test,predicted);
figure
imagesc(conf);
colorbar;
set(gca,'XTick',1:10);
set(gca,'XTickLabel',materials,'FontSize',8);
set(gca,'YTick',1:10);
set(gca,'YTickLabel',materials,'FontSize',8);

%% Let's try K Nearest Neighbor

kdtreeNS = KDTreeSearcher(X_Train);

[IDX, D] = knnsearch(kdtreeNS,X_Test);
results = Y_Train(IDX);
missClassified = sum(Y_Test~=results);
r2 = 100*(1-missClassified/size(Y_Test,1));
display(['Test accuracy = ',num2str(r2),'%']);

%% Try again with larger training set
c = cvpartition(textureMaterial,'holdout',.2);

X_Train = textureDescriptors(training(c,1),:);
Y_Train = nominal(textureMaterial(training(c,1)));

X_Test = textureDescriptors(test(c,1),:);
Y_Test = nominal(textureMaterial(test(c,1)));

kdtreeNS = KDTreeSearcher(X_Train);

[IDX, D] = knnsearch(kdtreeNS,X_Test);
results = Y_Train(IDX);
missClassified = sum(Y_Test~=results);
r2 = 100*(1-missClassified/size(Y_Test,1));
display(['Test accuracy = ',num2str(r2),'%']);

%% Now let's try treebagger
matlabpool                  % treebagger can leverage PCT
%%
tbObj = TreeBagger(150,X_Train,Y_Train);

predicted = tbObj.predict(X_Test);
predicted2 = cellfun(@(x)str2num(x), predicted);
missClassified = sum(Y_Test~=nominal(predicted2));
r2 = 100*(1-missClassified/size(Y_Test,1));
display(['Test accuracy = ',num2str(r2),'%']);

%% Look at confusion matrix again
[conf, classorder] = confusionmat(Y_Test,nominal(predicted2));
figure
imagesc(conf);
colorbar;
set(gca,'XTick',1:10);
set(gca,'XTickLabel',materials,'FontSize',8);
set(gca,'YTick',1:10);
set(gca,'YTickLabel',materials,'FontSize',8);
