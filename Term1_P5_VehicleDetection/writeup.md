
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
4. I trained a linear SVM by using sklearn.svm.LinearSVC. I use small 'C' parameter to make the model more generalized. To decide the parameters to extract feature, I tested many combinations. The high accuracy shouldn't be the only standard to choose parameters, it could be a sign of overfitting.  I use 'YUV' color space to improve the ability to recognize the white car in the project video. My final parameters are like this:
```
colorspace = 'YUV'
spatial_size = (16, 16)
hist_bins=16
hist_range=(0,256)
orient = 11
pix_per_cell = 16
cell_per_block = 2
```

### Sliding Window Search

1. I used sliding window to select sub-region to feed into my trained classifier. I tested different window sizes, different starting and stoping position, and different overlapping, and choose the combination of 3 different sizes. This picture shows the windows that are recognized as vehicle:  
![alt text][image2]

2. Then I calculate heat map based on these windows and use a threshold to filter out false positive window. The output is as this picture:  
![alt text][image3]

3. Finally I draw back modified boxes to the origin image as this picture shows:  
![alt text][image4]

### Video Implementation

1. When apply the same operation to a video, it could draw boxes jump-around between frames. Instead of directly use the heatmap, I also consider the average heatmap of several previous frames. This method makes the boxes move smoothly, and it also help filtering out false positive boxes that only appears in a single frame.  
2. The output_project_video.mp4 is my final output video.

### Discussion

1. It is precious to have complete dataset to train the classifier. Collecting and labeling images is a good topic of future work.  
2. Be aware of overfitting. The correlation between training data and validation data may give you high accuracy, but may generate bad performance in real situation.
2. The calculation is time consuming. I need use HOG sub-sampling window search to speed up the computation.



```python

```
