# Citation
Rothman, D. (2018). Artificial intelligence by example : develop machine intelligence from scratch using real artificial intelligence use cases. Packt Publishing. https://search.ebscohost.com/login.aspx?direct=true&scope=site&db=nlebk&db=nlabk&AN=1823676

# Meta
- Seems to take an neural net approach instead of a minmax approach 
# Notes
Has only one section covering chess, that of evaluating a chess position, using neurons 
Look into
- McCulloch-Pitts neurons
- Activation functions for chess
# Evaluating a position in a chess game
"The evaluation system can easily be represented with two McCulloch-Pitts neurons, one for black and one for white." (55)
"The sum of both neurons requires an activation function that converts the evaluation into 1/100th of a pawn" (55)

McCulloch-Pitts Neurons
- contains inputs, weights, and an activation function (32)
Diagram that shows how they work (53):
![](Pasted%20image%2020241011132151.png)
Basically, inputs are multiplied by weights, which are summed, and then ran through an activation function
Requires an "operator to directly set its weights". perceptron, developed in 1957 and often used in deep learning, don't. (34)
