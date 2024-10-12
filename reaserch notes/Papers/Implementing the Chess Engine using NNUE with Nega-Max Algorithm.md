# Citation
Chitale, A. M., Cherian, A. M., Singh, A., P, P., & 2024 IEEE International Conference on Electronics, Computing and Communication Technologies (CONECCT) Bangalore, India 2024 July 12 - 2024 July 14. (2024). Implementing the Chess Engine using NNUE with Nega-Max Algorithm. In 2024 IEEE International Conference on Electronics, Computing and Communication Technologies (CONECCT) (pp. 1–6). https://doi.org/10.1109/CONECCT62155.2024.10677087

https://unb.on.worldcat.org/search/detail/10379208017?queryString=chess%20engine&clusterResults=true&groupVariantRecords=false

# Meta
- Look into NNUE
# Notes
"Stockfish, the strongest chess engine available currently, utilises the Min-Max algorithm with Efficiently Updateable Neural Network (NNUE)"

"Min-Max algorithm computes the feasibility of a particular move only in positive numbers ... Nega-Max algorithm utilises both negative and positive numbers." This makes Nega-max potentially theoretically more efficient. Interestingly, however, stockfish outperformed a version of itself that used nega-max instead of Min-Max.

"This research aims to advance computer chess and artificial intelligence (AI) by integrating Neural Network with Updated Evaluation (NNUE) and the Nega-Max algorithm."

Chess is a game of both tactics and stratagy:
- "Tactical moves involve calculating potential sequences of moves to determine forced advantages or disadvantages." IE, this sacrifice leads to a knight fork.
- "\[S\]trategic decisions involve evaluating positions without extensive calculation." IE, the bishops are worth more then knights in open positions.

"Chess engines typically utilize Alpha-Beta pruning or Monte Carlo search algorithms to generate move sequences while optimizing them through neural networks." Min-max is often used to allow for alpha-beta pruning, but Nega-max is better and more modern.

NNUE dramatically improves the ability of the engine to evaluate positions.

A chess engine generally has the following core functions:
- Board representation
- Move generation
Generates all legal moves
- Evaluation function
Assigns a numerical score to a position
- Search algorithm
Explores possible moves and chooses the best one according to the eval function.
- Transposition table
Stores previous evaluations and searches to avoid redoing them
- Opening book
A book of opening theory to help in the initial stages of the game 
- Endgame table base
A database of endgame positions to let the engine play perfectly in the end game.

Negamax is an algorithm derived from minimax.
It uses a recursive tree search, and assumes all players will make optimal choices.
It assigns scores to the leafs of the tree, based on heuristic functions 
Alpha-beta pruning significantly improves the efficiency of negamax  
It focuses on more promising moves first. 

The NNUE component uses three hidden layers, and divides input into two players and represents the pieces relative to the king. 
Weight sharing = shared weights for mirrored piece-square relations.
"Training involves pairs of positions and scores from existing or self-played games, with iterative reinforcement learning refining evaluation through position re-analysis."