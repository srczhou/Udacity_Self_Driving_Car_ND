
# **Finding Lane Lines on the Road** 

The goals / steps of this project are the following:
* Make a pipeline that finds lane lines on the road
* Reflect on your work in a written report


### Reflection

### 1. Describe your pipeline. As part of the description, explain how you modified the draw_lines() function.

My pipeline consisted of 5 steps.
1. Converte the images to grayscale.
2. Use canny edge extraction to get all the edges. Of course blur the grayscale before that for a good results. Because the lane lines in the images are all clear, I set the low_threshold high (150), this will drop some edges caused by uneven road surface.
3. Choose an area of interests, mask out edges outside of this area. I show all the 6 images together to make sure the area is good for all of them. Also I use percentage of the image size, not absolute number as coordinates.
4. Use hough line transform to get lane lines. Because most lane lines are relatively long, I choose a larger min_lan_length.
5. Annotate the deteced lane lines back into the original image.

In order to draw a single line on the left and right lanes, I modified the draw_lines() function by dividing all the lines into 2 groups: left (if their slope is negative) and right (if their slope is positive), then I use their average slope and intercept to draw a single line. Later I found a weighted average (longer line gets more weight) leads to better results. This method works good for all the images and videos... until I met the challenge.mp4.

I wrote an optimized draw_lines() function specially for the challenge video. I made the following 4 optimization, and it finally get a good result.
1. Besides give longer lines more weight, I also give the line closer to the car more weight. This is based on the observation that the lines far away from the car becomes curve.
2. Based on the same observation, I draw the line shorter, so it ends before the begining of curve.
3. During calculating the weighted average slope, I ignore all the lines with small absolute slope value (-0.2<slope<0.2). These lines are almost horizontal, and apparently not what I want, but influence the final slope a lot.
4. To make the final line change smoothly, not jump around. I introduced several global variables to store the line location of the previous image. If the calculated line is far away from the previous image, I will restric the new line stay close (within 5 pixel) to the previous location. This optimization is very useful to generate a smooth video.

The output images are test_images/*.png files, and the output videos are test_videos_output/*.mp4.

### 2. Identify potential shortcomings with your current pipeline

The draw_lines() function I used for the challenge video can get a good results. But it has a problem. If the very first image is a bad image, I get a rediculous line. Later, it will take a while to go back to normal line, because I forbid large change between two continous images.


### 3. Suggest possible improvements to your pipeline

For the problem I mentioned above, one solution is to use adaptive restriction. For example, when the line's slope is close to zero (apparently abnormal), I will allow big change from the previous image (say, 20 pixel); when the line's slope is around 0.8 (expected slope), I only allow small change from the previous image (say, 5 pixel).

I didn't apply this to my code, because too early optimization is not very good. I'll keep this in mind and come back after I learn how to draw curve lines.

