# Citation
V´azquez-Fernandez, E., Coello , C., & Troncoso, F. (n.d.). _Assessing the Positional Values of Chess Pieces by Tuning Neural Networks’ Weights with an Evolutionary Algorithm_.

http://delta.cs.cinvestav.mx/~ccoello/conferences/evazquez-wac2012-final.pdf.gz

# Meta 

# Notes
Uses 6 neural networks that combine
- Each neural net has 4 inputs, nine in a hidden layer, and 1 output
- "The decision to use three layers was based on the demonstration of Hecht-Nielsen \[12], which established that any function can be approximated by a three-layer neural network" (p2)
- "The decision to use nine nodes in the hidden layer was based on Kolgomorov’s theorem \[20] which established that the number of nodes in the hidden layer should be at least (2i + 1), where i denotes the number of nodes in the input layer." (p2)
Neural networks:
King
Inputs:
- Attacking material
- Defending material
- castling
- pawns
Queen positional value
- Queen mobility
- Column type (pawn, one side has pawns, both side have pawns)
- Row
- Column
Rook positional value
- Rook mobility
- Column type 
- Is on the 7th
- 2 rooks on the 7th
Bishop positional value
- Bishop mobility
- Pawns mobility (num pawns hat obstruct movement of bishop)
- ahead (num pawns in front of bishop and obstructing mobility)
- weight (the weight of the pawns obstructing the bishops mobility)
Knight positional value:
- Knight mobility
- Periphery (is the knight on the edges)
- Supported (if the knight is supported by the pawn)
- Operations base (is the knight on an outpost)
Pawn positional value:
- Doubled 
- Isolated
- Central (is the pawn in the center)
- Past