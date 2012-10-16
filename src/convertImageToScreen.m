% Write out test images for hack emulator screen display 

%clear everything
close('all')
clear('all')
figure(1)
%get an image
I1 = imread('cameraman.tif');
%double it to be the correct width for our screen (512x256 wxh)
I1 = cat(2,I1,~I1);
I1 = im2bw(I1);
imshow(I1)
size(I1)

%repeat for image 2
figure(2)
I2 = imread('mandi.tif');
size(I2)
I2 = imresize(I2,0.35);
size(I2)
I2 = I2(1+256:256+256,1+512:512+512);
size(I2)
I2 = im2bw(I2,0.35);
imshow(I2)

figure(3)
I3 = imread('testIm.bmp');
I3 = im2bw(I3);

%stack both images vertically so they appear as separate screens
%largeIm = cat(1,I1,I2,I3,I1,I2,I3);

largeIm = cat(1,I1,I2,I1);


imshow(largeIm);
%11100011
%Now take the binary numbers and pack them into bytes so we can store as 
%ASCI integers from 0-255
ii = 1;

%Declare output that we will eventually write to file
outArray = ones(numel(largeIm)/8,1);
for i=1:size(largeIm,1)
   for j=1:8:size(largeIm,2) 
        numStr = num2str((largeIm(i,j:j+7)));
        %numStr = fliplr(numStr);
        outArray(ii) = bin2dec(numStr);
        ii = ii + 1;
   end
end

size(largeIm)
size(outArray)
%Now write data
fid = fopen('C:\Users\Chris2.chris-HP-Select\Documents\GitHub\hack_cpu_emulator\screenTestGolden.txt','w');

for k=1:numel(outArray)
    fprintf(fid,'%d\n',outArray(k));
end
%m5 = bin2dec(m5);
fclose(fid);
