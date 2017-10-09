
# Vehicle Detection Project

The goals / steps of this project are the following:

* Perform a Histogram of Oriented Gradients (HOG) feature extraction on a labeled training set of images and train a classifier Linear SVM classifier
* Optionally, you can also apply a color transform and append binned color features, as well as histograms of color, to your HOG feature vector.
* Note: for those first two steps don't forget to normalize your features and randomize a selection for training and testing.
* Implement a sliding-window technique and use your trained classifier to search for vehicles in images.
* Run your pipeline on a video stream (start with the test_video.mp4 and later implement on full project_video.mp4) and create a heat map of recurring detections frame by frame to reject outliers and follow detected vehicles.
* Estimate a bounding box for vehicles detected.
---
[//]: # (Image References)
[image1]: ./output_images/HOG_example.png
[image2]: ./output_images/Hot_window.png
[image3]: ./output_images/Heatmap.png
[image4]: ./output_images/Labeled_box.png


### Train a Classifier
1. In the 3rd cell of my Jupyter Notebook file, I defined the function get_hog_features() to extract the Histogram of Oriented Gradients (HOG) features from an image, which used skimage.feature.hog function. A visualized example is like this:
![alt text][image1]
2. In the 4th cell of my Jupyter Notebook file, I also defined function to extract binned color features and color histogram features.  
3. Then I combined all these features as input to my classifier. Because these features are in different scale, I normalized them first.  
4. I trained a linear SVM by using sklearn.svm.LinearSVC. To decide the parameters to extract feature, I tested many combinations, and finally choose the one with the best accuracy, which is higher than 99 percent. My parameter choice is:
```
colorspace = 'HLS'
spatial_size = (16, 16)
hist_bins=16
hist_range=(0,256)
orient = 9
pix_per_cell = 8
cell_per_block = 2
```

### Sliding Window Search

1. I used sliding window to select sub-region to feed into my trained classifier. I tested different window sizes, different starting and stoping position, and different overlapping, and choose the combination with a good results on the test images. This picture shows the windows that are recognized as vehicle:  
![alt text][image2]

2. Then I calculate heat map based on these windows and use a threshold to filter out false positive window. The output is as this picture:  
![alt text][image3]

3. Finally I draw back modified boxes to the origin image as this picture shows:  
![alt text][image4]

### Video Implementation

1. While the previous stratage works well on the test images, it had some major issue when applied on the project video. For a certain period of time, the white car cannot be recognized. So I increase my sliding window type with different window size and with larger overlapping.  
2. Because the classifier had trouble to recognize the white car in that period of time, I need keep even slight positive window, I dare not to raise the threshold to filter out false positives. Actually I use threshold 1.  
3. The output_project_video.mp4 is my final output video.

### Future Improvement

1. Use more complete data set. Apparently, my training data have less white car, so the pipeline works much worse for white car than the black car.  
2. I need use HOG sub-sampling window search to speed up the computation.  
3. I need smooth the video output by averaging with previous frames.



```python

```
