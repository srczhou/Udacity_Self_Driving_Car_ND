import csv
import cv2
import numpy as np

# only keep 1/10 of samples whose angle is close to zero
cnt = 0
lines = []
with open('./data/driving_log.csv') as csvfile:
    reader = csv.reader(csvfile)
    for line in reader:
        if  abs(float(line[3])) < 0.05:
            cnt += 1
            if cnt % 10 != 0:
                continue
        lines.append(line)
del lines[0]

# each image will generate 2 samples: original and flipped
images = []
measurements = []
for line in lines:
    #center
    source_path = line[0]
    filename = source_path.split('/')[-1]
    current_path = './data/IMG/' + filename
    image = cv2.imread(current_path)
    images.append(image)
    measurement = float(line[3])
    measurements.append(measurement)
    image_flipped = np.fliplr(image)
    images.append(image_flipped)
    measurement_flipped = -measurement
    measurements.append(measurement_flipped)
    #left camera's angle plus 0.25
    source_path = line[1]
    filename = source_path.split('/')[-1]
    current_path = './data/IMG/' + filename
    image = cv2.imread(current_path)
    images.append(image)
    measurement = float(line[3]) + 0.25
    measurements.append(measurement)
    image_flipped = np.fliplr(image)
    images.append(image_flipped)
    measurement_flipped = -measurement
    measurements.append(measurement_flipped)
    #right camera's angle minus 0.25
    source_path = line[2]
    filename = source_path.split('/')[-1]
    current_path = './data/IMG/' + filename
    image = cv2.imread(current_path)
    images.append(image)
    measurement = float(line[3]) - 0.25
    measurements.append(measurement)
    image_flipped = np.fliplr(image)
    images.append(image_flipped)
    measurement_flipped = -measurement
    measurements.append(measurement_flipped)
X_train = np.array(images)
y_train = np.array(measurements)


from keras.models import Sequential
from keras.layers import Flatten, Dense, Lambda
from keras.layers.convolutional import Conv2D, Cropping2D
from keras.layers.normalization import BatchNormalization

model = Sequential()
model.add(Cropping2D(cropping=((50,20), (0,0)), input_shape=(160,320,3)))
model.add(Lambda(lambda x: (x / 255.0) - 0.5, input_shape=(90,320,3)))
model.add(Conv2D(24, (5, 5), strides=(2,2), padding='valid', activation='elu'))
model.add(Conv2D(36, (5, 5), strides=(2,2), padding='valid', activation='elu'))
model.add(BatchNormalization())
model.add(Conv2D(48, (5, 5), strides=(2,2), padding='valid', activation='elu'))
model.add(Conv2D(64, (3, 3), strides=(1,1), padding='valid', activation='elu'))
model.add(Conv2D(64, (3, 3), strides=(1,1), padding='valid', activation='elu'))
model.add(Flatten())
model.add(BatchNormalization())
model.add(Dense(100,activation='elu'))
model.add(Dense(50,activation='elu'))
model.add(Dense(10,activation='elu'))
model.add(Dense(1))

model.compile(loss='mse', optimizer='adam', metrics=['accuracy'])
model.fit(X_train, y_train, validation_split=0.2, shuffle=True, epochs=9)

model.save('model.h5')

del model
