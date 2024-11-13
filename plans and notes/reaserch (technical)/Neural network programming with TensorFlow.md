# Citation 
Ghotra, M. S., & Dua, R. (2017). Neural network programming with TensorFlow : unleash the power of TensorFlow to train efficient neural networks. Packt Publishing. https://search.ebscohost.com/login.aspx?direct=true&scope=site&db=nlebk&db=nlabk&AN=1630664

https://unb.on.worldcat.org/search/detail/1015687249?queryString=neural%20network%20optimizers&clusterResults=true&groupVariantRecords=false

# Meta

# Notes
## Cha 3
p.65
optimization = process to find maxima or minima (65)
First order optimization
- Uses first order derivetive

p.66
Second order optimization (also called Hessian)
- Use second derivative
- More costly then the first
- Does not ignore the curvature of a surface
- Stepwise preformance is better
- Can work better then first order because it "will never get stuck around paths of slow convergence"

p.67
Gradiant descent is the most "most successful optimization algoritim"

p.68
standard graident descent = calculates gradiant of whole dataset
Stochastic gradient descent = "performs parameter updates on each training example"
- frequet updates complicates convergence 
- mini-batch gradiant descent helps, by having a bunch of small batches of training examples ran, instead of having each example ran at a time

p.69
momentum = accelerates SGD by "by navigating along the appropriate direction and softening the oscillations in irrelevant direction"
- Adds some fraction (generally 9/10ths) of the last step to the current vector

**Nesterov accelerated gradient** 
- when we reach minima, momentum is high causing us to miss it
- So:
1. We make a "long jump based on the previous momentum"
2. We calculate the gradiant and make a correction, updating the parameters
- This results in it being responsive to change, and not missing the minima

**Adagrad**
- Perfroms large updates for "infrequent parameters and small updates for frequent param"
- Well suited for dealing with sparse data
- Learning rate is in decay, fixed in AdaDelta

**AdaDelta**
- Solves the decayed learning issue in adagrad, by stopping the denomenator from always decreasing 

**Adaptive Moment Estimation (Adam)**
- Computes adaptive learning rates for parameters
- Similar to AdaDelta (stores decaying average of past squared gradiants)
- Also stores "momentum change for each parameter"
- One of the most used methods today

p.71
If data is sparse or we want fast convergance, adaptive learning rate is the best choice. Most of the time, Adam is a good choice.