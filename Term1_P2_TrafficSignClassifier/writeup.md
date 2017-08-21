
# Traffic Sign Recognition Project

The goals / steps of this project are the following:
* Load the data set (see below for links to the project data set)
* Explore, summarize and visualize the data set
* Design, train and test a model architecture
* Use the model to make predictions on new images
* Analyze the softmax probabilities of the new images
* Summarize the results with a written report

---
### For detailed code and images, please review Traffic_Sign_Classifier.ipynb

### Dataset Exploration
- I first checked the training, validation, and testing data sizes are reasonable.
- After checking the image shape is (32, 32, 3) and the category number is 43, I drew one image for each category. I found some image are pretty dark, seems taken at night time.
- Preprocessing the image includes two steps: first, changed the image to grayscale; second, normalize the image to zero-centerd and small enough so that they are between -1 and 1.
- Then I checked the category's distribution in the training, validation, and testing data. In training data, category numbers vary between 180 and 2010. To avoid this imbalance, I randomly oversampled data for each catogary to make sure there are 2010 training examples for each category. This actually increased my training data from 34799 to 86430.
- I tried data augmentation by writing a function which can randomly rotate, translate, and shear the image. Unexpectedly, my trainging with the data augmentation actually get worse accuracy. Finally I didn't use this augmentation, but I need spend some time to find out the reason.

### Design and Test a Model Architecture
- Start with leNet-5, I changed the layer structure by increasing feature numbers, and I also use dropout for each layer.
- My final model consisted of the following layers:

| Layer         		|     Description	        					| 
|:----------------------|:----------------------------------------------| 
| Input         		| 32x32x1 grayscale image    			    	| 
| Convolution 5x5     	| 1x1 stride, valid padding, outputs 28x28x32 	|
| RELU					|												|
| Max pooling	      	| 2x2 stride,  outputs 14x14x32 				|
| Dropout               |                                               |
| Convolution 5x5	    | 1x1 stride, valid padding, outputs 10x10x64	|
| RELU					|												|
| Max pooling	      	| 2x2 stride,  outputs 5x5x64    				|
| Dropout               |                                               |
| Fully connected		| outputs 128        							|
| Dropout               |                                               |
| Fully connected		| outputs 64        							|
| Dropout               |                                               |
| Fully connected		| outputs 43        							|
| Softmax				| etc.        									|

- To train the model, I used Adam optimizer; set batch size to 128; learning rate to 0.001. The dropout keep probability was tested from 0.5 to 0.9, and finally choose 0.9 which has the best result.
- My final model results were:
    * training set accuracy of 0.999
    * validation set accuracy of 0.963
    * test set accuracy of 0.949

### Test a Model on New Images
- The test set accuracy is 0.949
- I use the first 5 test data to look into details. All of them are recognized correctly, and the correct category is with very high probability (1.00000000e+00), which means it had no difficulty to distinguish it with other signs.

### Future work
- The oversample took very long time, I need improve the efficiency of the code.
- The data augmentation didn't make the result better. Is it because I over-augmented the image?
- How to choose the network structure is still not very clear, random search then fine-tune is what I am using. I think if I borrow more advanced network structures such as googleNet, resNet, I should be able to get a better results.


```python

```
