%% Set-up for Texture Classification Demo
% Copyright 2011 The MathWorks, Inc.
%

%% Set up all feature vectors in this file

dirName = 'C:\work\ComputerVision\ImageClassificationDemo\KTH_TIPS';
dirInfo = dir(dirName);
dirIdx = find([dirInfo.isdir]);
imDirs = dirIdx(3:end);

for k = 1:length(imDirs)
    dirIdx = imDirs(k);
    if (dirInfo(dirIdx).isdir)
        dbInfo(k).material = dirInfo(dirIdx).name;
        dbInfo(k).path = fullfile(dirName, dirInfo(dirIdx).name);
        fileInfo = dir(fullfile(dirName, dirInfo(dirIdx).name, '*.png'));
        dbInfo(k).files = {fileInfo(:).name};
    end
end
        
save dbInfo dbInfo