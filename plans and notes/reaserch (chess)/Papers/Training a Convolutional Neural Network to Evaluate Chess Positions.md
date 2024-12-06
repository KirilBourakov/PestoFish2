# Citation
https://www.diva-portal.org/smash/get/diva2:1366229/FULLTEXT01.pdf
# Meta
You can likely train on both black to move and white to move with the 1 = friendly piece, -1 = enemy piece idea. 
# Notes
p.5 
A common way to represent a category is with a number. However, this does not work, as you end up with nonsese results. For example, if Sweden = 1, Japan = 2, USA = 3, the average of USA and Sweden is Japan.
Instead, one hot encoding is used:
category is replaced by a binary value, one for each category. The category of the input is set to one, and all others are set to 0.

p.7 
To one-hot the board, each of the 6 pieces is considered a value. 1 = white piece, 0 = no piece, -1 = black piece.
The chessboard becomes "3-dimensional with dimensions (8,8,6)." (8x8) for the size of the board, and *6 again as there are 6 pieces.

They only trained on positions that were white to move. The board then can be flipped to eval a position from black's perspective. 

This then makes 1 = friendly piece, -1 = enemy piece
They ignore ending's other then checkmate

p.8
They set a checkmate to be valued at +/-255, and a forced checkmate to be +/-127. All others were capped to +/-63

model implemented using tf.keras
- used 4 2d convolutional layer
- first 3 had kernal size 3x3, and last one 2x2
- All used ReLU
- 8, 16, 32 and 64 filters used

p.14
model is good at "counting piece"
bad at tactics 
A more complex model is needed for better results