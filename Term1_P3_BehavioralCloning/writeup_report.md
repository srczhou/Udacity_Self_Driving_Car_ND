
# Behavioral Cloning Project

### Files Submitted & Code Quality

My project includes the following files:
* model.py containing the script to create and train the model
* drive.py for driving the car in autonomous mode
* model.h5 containing a trained convolution neural network
* run1.mp4 recording the autonomous driving vedio
* writeup_report.md summarizing the results

Using the Udacity provided simulator (640x480 resolution and fastest quality) and drive.py file, the car can be driven autonomously around the track by executing
```sh
python drive.py model.h5
```

### Model Architecture and Training Strategy

#### Solution Design Approach

My first step was to use NVIDIA convolution neural network model because it's been tested on both simulator and road. The paper didn't provide every details, so I made the following choice:
1. Use ELU as activation function. It's better than ReLU because its output has closer-to-zero mean; and it takes care of the vanishing gradient problem at the negative regiem.
2. Add batch normalization layers as regularizer. So I didn't use dropout layers.
3. Before normalization the input image, I also cropped the image so it filtered out un-related background.

#### Final Model Architecture

The final model architecture (model.py lines 67-81) consisted of a convolution neural network with the following layers and layer sizes:
1. Input layer 160x320x3
2. Cropping layer, output 90x320x3
3. Input normalization layer, output 90x320x3
4. Convolution layer, 5x5 kernel, 2x2 stride, 24 depth, ELU activation, output 43x158x24
5. Convolution layer, 5x5 kernel, 2x2 stride, 36 depth, ELU activation, output 20x77x36
6. Batch normalization layer, output 20x77x36
7. Convolution layer, 5x5 kernel, 2x2 stride, 48 depth, ELU activation, output 8x37x48
8. Convolution layer, 3x3 kernel, 1x1 stride, 64 depth, ELU activation, output 6x35x64
9. Convolution layer, 3x3 kernel, 1x1 stride, 64 depth, ELU activation, output 4x33x64
10. Flatten Layer, output 8448
11. Batch normalization layer, output 8448
12. Fully connected layer, output 100
13. Fully connected layer, output 50
14. Fully connected layer, output 10
15. Fully connected layer, output 1

#### Creation of the Training Set

The original training data is collected from driving, so most of the samples are with almost zero angle. To get a more balanced data set (a bell-shaped distribution centered at zero), I randomly remove 9/10 samples whose angles are between -0.05 and 0.05.

To get more training data, I also use left and right cameras' data. I adjusted their angles so they can be used as if they are taken by the center camera. Finally, I plus 0.25 to the left camera angle, and minus 0.25 to the right camera angle.

To get even more training data, I flip each sample image, and multiple its angle by -1, to get another sample.

I tried to train my own data, but it only made my model worse. The reason is that the keyboard is not a good control equipment, it cannot generate constent samll control signal. The collected data are either overturn or recovery from overturn, not optimized angle. Finally, I didn't use any of my own training data.

#### Training Process

I use adam optimizer to minimize MSE. I randomly shuffled the data set and put 20% of the data into a validation set.

### Simulation

The car can navigate correctly on track 1. It can stay on road all the time, but won't stay at the center when it's close to a sharp turn. I think that's OK for higher speed, which is how the training data collected.

### Future Improvement

1. Train the data with a joystick, which can provide small constent control signal.
2. Carefully collect recovery data from extreme situation. Maybe it's easier to create data than collect data from the simulator.
3. Image augmentation such as brightness and contrast randomization.
4. Optimize the neural network structure, such as adjust the size of the fully connected layers (our input image is larger than NVIDIA's input); do experiment to find out should the batch normalization layer be put before or after the activation layer.
5. The best steering angle should also consider the car speed.


```python

```
