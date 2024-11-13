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