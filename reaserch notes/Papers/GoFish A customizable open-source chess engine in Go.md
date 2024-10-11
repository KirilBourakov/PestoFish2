# Citation
Jubair, F., Abu Salem, A., Assaf, Y., & Darabkh, K. A. (2024). GoFish: A customizable open-source chess engine in Go. Expert Systems with Applications, 250. https://doi.org/10.1016/j.eswa.2024.123855

https://unb.on.worldcat.org/search/detail/10246212991?queryString=chess%20engine&clusterResults=true&groupVariantRecords=false
# Meta
Look into the following:
- Bitboards, magic bitboards
- Zobrist Hashing 
- UCI
- Negamax
- Quiescence
- Everything in the list at the end
# Notes
"Stockfish employs iterative deepening search and alpha–beta pruning techniques"

"Komodo (Komodo Chess Engine, 2023) employs Monte Carlo tree search and alpha–beta pruning techniques to identify strong moves."

AlphaZero uses deep learning instead of predefined rules.

"chess engines developed in Java are less likely to attain higher ELO ratings compared to engines implemented in compiled programming languages such as C and C++"

"Chess engines must also incorporate the Universal Chess Interface (UCI) (UCI protocol, 2023), a standardized communication protocol facilitating interaction between chess engines and graphical user interfaces (GUIs)"

# Representing pieces under the hood

"Bitboards are a compact and efficient data structure commonly used in chess engines to represent the positions of chess pieces on a chessboard. With the chessboard comprising 64 squares, the fundamental concept of bitboards involves utilizing a set of 64 bits, with each bit corresponding to a square on the chessboard. The presence or absence of a bit at a specific position indicates the presence or absence of a chess piece on the corresponding square."

"GoFish utilizes a distinct bitboard for each piece type on each player's side."

Bitboard are time and memory efficient, and allow for "move generation through fast and parallel bitwise manipulation". However, they add complexity, and have limitations "as they cannot provide answers to queries like identifying which piece exists on a specific given square."

Another approach is representing piece locations in 2x2 array with numerical encodings for each piece. It is simpler, easy to tell what piece is where, but is slower for move generation.

Gofish uses both bitboards and 2x2 array

# Handling Castling and en passent
Because of how much information about the board we need to hold, Gofish uses Zobrist Hashing "a technique used in chess programming to efficiently generate a unique hash key for a given position on the game board. The basic idea behind Zobrist hashing is to pre-compute a set of random values (bitstrings) and associate each possible piece (of each type) on each square with a unique random value. These random values are stored in tables known as Zobrist Tables."

# Move encoding
"to optimize performance, additional information can be encoded within a move." This is because this information can be used "for move ordering during the search phase, enhancing the pruning of the game tree for more efficient exploration"

Gofish uses a 32-bit int to encode a move. 6 bits are used for source square, "six bits for the destination square, two bits for the move type, and an additional two bits for _meta_-information about the move" (is it enpassent, what type of promotion is it, etc)

# Move generation
"GoFish uses a pseudo legal move generator which does not guarantee that the king is not left in check after the move." Instead, this check is made after the move is made to streamline move generation.

Instead of generating moves during runtime, Gofish precomputes and stores "stores all possible moves". This approach was inspired by stockfish.

Gofish splits pieces into sliding pieces and non sliding pieces. Further, it goes shift operations on the bitboard to generate possible moves.

For sliding pieces, it uses a hash function designed to produce collisions "a process referred to as magic bitboards (Kannan, 2007)"

# Universal chess interface integration (UCI)
This is the standard for chess engine communication with a GUI.
Look further into this.

# Heuristic
Gofish uses the following heuristic's
Material count - a variable amount based on the stage of the game
King safety - a 2 square ring around the king that looks at where threats are within that ring
Piece mobility - the number of legal moves a piece can move to, scaled based on the stage in the game
Piece placement - A 8x8 table for each piece that indicates how advantageous it is to have a piece there, like the vector map proposed in vecma. Different tables depending on stage in the game.
Pawn structure - the structure of the pawns
Outpost - is there a piece on an outpost
Open files - is there an open file we have control over

# Minmax
Uses Negamax
Uses Quiescence search - conduct deeper exploration into branches where captures occur. "GoFish extends the search at leaf nodes until a quiet position is reached. A position is deemed quiet when there is no immediate threat—meaning the king is not in check—and no valid tactical opportunities such as captures or promotion moves exist."
Uses a transposition table to remove redundant calculation 

Gofish uses alpha-beta pruning, who's effectiveness is controlled by ordering. In addition, it uses the following optimizations:
- Killer move. "Killer moves refer to non-capture moves that led to an early cutoff in a preceding node during a search, indicating their strength."
- History heuristic. Order based on the effectiveness of moves in the past.
- Counter moves. "Counter moves are non-capture moves that resulted in a search cutoff following specific preceding moves, making them good response moves."
- Progressive deepening. "Progressive deepening uses an iterative deepening exploration strategy where the search begins with a shallow search, exploring the game tree to a relatively low depth."
- MVV/LVA heuristic "This approach involves organizing capturing moves based on the values of both the captured and capturing pieces"
- Aspiration windows. Moves the alpha and beta values of alpha-beta pruning dynamically
- Check extensions - extend search in positions where check is given
- Null move pruning - give the opponent an extra move. If you stay winning, you can stop the search.
- Razoring - remove moves with bad static eval.
- Futility pruning - "For a given current position, each possible move is evaluated using the static evaluation function. If the evaluation of a move, when added to the current evaluation, is still below the alpha value, the move is deemed unnecessary and is consequently pruned from the search."
- Singular Move extensions - when there is a single move that is clearly better then all the others.
- Late Move reductions - prioritize moves based on their order. If the moves are well ordered, this can be a substantial speed increase.  
