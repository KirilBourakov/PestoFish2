# Citation
https://www.youtube.com/watch?v=mFZazxxCKbw
# Meta
# Notes

# Activation functions
**step function**
g(x) = 1 if x >= 0, else 0
**logistic sigmoid**
e^x / (e^x+1)
s shaped graph, useful for probability 
**Relu**
g(x) = max(0,x)

# Tenserflow
sklearn has a function to split train and test data

steps:
1. read data in
2. divide data into training and testing set
3. Create the neural network

## Creation
.keras api provides easy api to create a neural network
.models.Sequential() to create a sequential model
- One layer after another
.add(\[layer]) add a layer to the model
layers have different types
- Dense = layers are all connected to next ones

Input_shape = the shape of the input
activation= string, with the name of the activation func

.compile() create the neural network, with metrics, loss function, and optimizers passed in as str

.fit() takes in training data and labels.
epochs = to set epochs (go through data n times)

.evaluate() tests the model against testing data

.save([filename]) save a model

## Computer vision
Just have each pixel be a input. But, that is alot of input. You also loose structure. It may be challanging to learn.

## image convolution
Filtering an image, by applying a filter
A kernal matrix is applied to the pixels to create new, smaller 'images'.
For example, a kernal of:
[-1,-1,-1]
[-1, 8,-1]
[-1,-1,-1]
will extract information about the edge

pooling:
reduce the quality of the image to reduce it's size
## CNN

1. take image
2. apply convolution (often several times)
3. images produced pooled
4. flatten the result
5. put it into a conventional neural network

.Sequential can take a list of layers instead of calling add every single time

.Conv2D(num_kernals: int, size: tuple[int], activation: str, input_shape: tuple[size, size, channels])

.layers <- pooling, such MaxPooling2d

.flatten <- flatten the CNN to be fed into dense layers

.Dropout() <- declare some amount of dropout to prevent over fitting
