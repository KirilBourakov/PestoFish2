# Source
https://www.ibm.com/think/topics/loss-function
# Meta
Try MSE, MAE, Huber
# notes
Regression loss function
- mesures errors in continues values.
- Apply to models that estimate quantifiable concepts
Classification loss function
- discrete values (such as t/f)
- split into binary classification and multi-class classification

## Regression loss functions:
### Mean squared error (MSE)
Generally used as the default.
MSE squares the difference between the predicted value and the true value. 
This strongly punsishes outliers 

### Mean squared logarithmic error (MSLE)
Better then MSE for problems with very wide range of outputs, such as those with expenential growth.

### Mean absolute error (MAE)
Like MSE, but does not square the error.
As such, it is more robust to outliers.

### Huber loss
Huber is a middle ground between MSE and MAE
a value n is used as a transition point:
- if abosulte difference is <= n, use quadratic (such as MSE)
- else, use linear (such as MAE)