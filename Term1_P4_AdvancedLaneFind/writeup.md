
## Advanced Lane Finding Project
---
The goals / steps of this project are the following:
* Compute the camera calibration matrix and distortion coefficients given a set of chessboard images.
* Apply a distortion correction to raw images.
* Use color transforms, gradients, etc., to create a thresholded binary image.
* Apply a perspective transform to rectify binary image ("birds-eye view").
* Detect lane pixels and fit to find the lane boundary.
* Determine the curvature of the lane and vehicle position with respect to center.
* Warp the detected lane boundaries back onto the original image.
* Output visual display of the lane boundaries and numerical estimation of lane curvature and vehicle position.
---
[//]: # (Image References)

[image1]: ./chessboardDistort.png "ChessboardUndistorted"
[image2]: ./undistort3.jpg "RoadUndistorted"
[image3]: ./threshold3.jpg "BinaryThreshold"
[image4]: ./perspectiveTransform.png "PerspectiveTransform"
[image5]: ./fittingLine.png "FittingLine"
[image6]: ./pipelineOutput.png "PipelineOutput"

### Camera Calibration
This project provided 20 images of a chessboard object taken by the same camera from different angles and distances. From these images, I can calculate the camera matrix and distortion coefficents with cv2.calibrateCamera() function, which reads the "object points" and "image points" of the inner corners of the chessboard.

I applied the distortion correction to an image which wasn't used for the calculation, and get this result:
![alt text][image1]
It works great. This distortion correction can be used later to any images or videos taken by the camera.

---
### Pipeline (test images)
#### 1. Provide an example of a distortion-corrected image.
An example of undistorted image is like this:
![alt text][image2]

#### 2. Describe how (and identify where in your code) you used color transforms, gradients or other methods to create a thresholded binary image. Provide an example of a binary image result.
In file P4.ipynb cell 3, where function threshold() were defined, I combined sobel derivatives on x and y directions, gradient's magnitude and direction, and color transform to create a binary image containing likely lane pixels. An example of the binary image is like this:
![alt text][image3]

#### 3. Describe how (and identify where in your code) you performed a perspective transform and provide an example of a transformed image.
In file P4.ipynb cell 4, I defined a generalized function warper() for perspective transform. And I used a straight lane line image to fine tune the source and destination points for this project. My final choice is
```
src = np.float32([[581,460],[207,720],[1100,720],[700,460]])
dst = np.float32([[320,0],[320,720],[960,720],[960,0]])
```
The following picture shows how it transform the image to a "birds-eye view".
![alt text][image4]

#### 4. Describe how (and identify where in your code) you identified lane-line pixels and fit their positions with a polynomial.
In file P4.ipynb, function find_line() used 12 sliding windows to identify lane-line pixels for an image, and the margin for each window gradually decrease.  
The shrinking-window method comes from when I deal with the first image in challenge_vedio.mp4, where the road separation fence is much stronger than the lane line, so I need a method to exclude the fence. I think it's better than a region-of-interest mask because the region-of-interest changes unexpectedly in a video.  
Then I fit their positions with a polynomial by function numpy.polyfit().  
An example of the fitted lane-line is like this:
![alt text][image5]

#### 5. Describe how (and identify where in your code) you calculated the radius of curvature of the lane and the position of the vehicle with respect to center.
In file P4.ipynb, function find_line() also calculated the radius of lane curvature and vehicle position.

#### 6. Provide an example image of your result plotted back down onto the road such that the lane area is identified clearly.
This picture is the output of the pipeline function image_pipeline() when reading a test image.
![alt text][image6]

---
### Pipeline (video)
When applying the pipeline function to video, we can make good use of the previous calculation. 
1. I don't have to use sliding window every time to identify lane-line. Instead, just search around the previous lane-line can speed up the program.
2. To avoid the lane-line jumps around from frame to frame, every time I calculate the lane-line based on the current frame, I average it with the previous frame. And this average is weighted, the previous frame have much higher weight, so it is almost like take average of the previous several frames.

My output video is output_project_video.mp4.
(I also provide the results video of output_challenge_video.mp4, the result is not perfect, but it provides clue on how to improve my program)

---
### Discussion
After testing my pipeline on the challenge_video.mp4 and harder_challenge_video.mp4, I find there are a lot of improvement needed to make it robust.   

#### Give more constraint to the lane-lines.
- The left-lane must be on the left side of the right-lane.
- The left-lane and right-lane should have the same curvature.
- The distance between left-lane and right-lane in the birds-eye-view should always be the same.  

#### Some optimization method need be more generalized.
- For example, smoothing over the previous frame works good for the project_video.mp4, but is bad for harder_challenge_video.mp4 becuase it has many sharp turn. I should not give the previous frame too much weight.
- I avoided mask the image with hard coded region-of-interest. If any region-of-interest technique is applied, it should be a relative region with respect to the lane-line.


```python

```
