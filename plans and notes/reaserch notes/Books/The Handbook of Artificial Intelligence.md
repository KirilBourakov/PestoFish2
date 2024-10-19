# Citation
Q335.H36 v.1

Barr, A., Feigenbaum, E. A., & Cohen, P. R. (1981). The Handbook of artificial intelligence. HeurisTech Press.
# Meta
Looked at only vol 1 thus far (the book is split into 3 volumes)
# Notes
## Minimax, Negamax
Minimax ignores the possibility that the opponent will make sub-optimal moves (85)

Negamax allows the algorithm to return moves that are optimal for both players. In negamax, the "value given to each node is it's value to the player whose turn it would be to move at that node" For example, if a node has a value x, the value to the other player would be -x. (86)

Negamax and minimax are equivalent (86)
# Static Evaluation Function
If a game is to complex to generate all at once, generate a reasonable portion of the game tree, and use that to choose your move (87). When you have a partial tree, you can use a *static evaluation function* to estimate the value of the position. If any of the nodes are terminal, generally a win is measured as infinity, a loss as -infinity, and a draw as 0. All others have some finite number as their eval. The minimax procedure is then applied. (87)

It is assumed that backed-up evaluations are more accurate then applying a static eval function and not looking into the future. (87)

# Alpha Beta Pruning
Alpha-Beta pruning allows for the removal of known bad moves (88). It uses two parameters, alpha and beta. Alpha's are the bottom values. For example, if MAX discovers a node with a value of 15 say on the right most branch, and of 10 on the left most branch, it can stop exploring the left most branch because of the alpha cutoff. A beta cutoff is the exact opposite, where a node that is over the cap is cut. Beta cuts are done for MIN, while alpha cuts are done for MAX (87).

Because of how cutoffs are done, the degree to which alpha-beta procedures represent an improvement in efficiency depends on what order the nodes are evaluated (90). There are several ways to do this:
**Fixed Ordering**:
Apply the static evaluation function to the states, and sort them by it (this assumes that the static eval of a position is an approximation of it's 'true' eval) (90).

## Alternatives to Search
"book moves" in the opening of a chess game can be used instead of search (95)


# Shannon's two types of chess engine
Shannon proposed two types of chess engines:
Type A:
- Generated to a fixed depth
- Use of static Eva function on leaf nodes to evaluate their value
Type B:
- Explore a line to a reasonable stopping point, instead of cutting it off at arbitrary depth
- Provide some selectivity among lines explored 
(96)
# Static Eval
Estimates the value of a board position without looking at any successors (96).
It's value represents features in the positions, such as material, king safety, mobility, center control, pawn structure (97).

Samuel, a researcher who made a checkers playing program, provided his program a large amount of features and let the program determine, through play, what features to use and what their weight should be. In later versions, there was an emphasis placed on interactions between these features, improving the checkers program (97).


# Backed-up Evaluation
When static evaluation is applied to minimax, it looses it's original purpose of finding theoretically correct moves, and becomes more of a heuristic itself. Therefore, several alternatives and modifications to it have been tried:
- taking into account not only the best successor, but how many good successors there
- using a very simple evaluation function and breaking ties between moves based on other heuristics not part of the eval function
- setting a value the move is expected to achieve, and taking that move when it is found. If no move is good enough, standered minimax is used.

# Depth of search
**Fixed depth search with extensions for quiscence (p.99)**
- Look forward a certain depth
- depth extended for certain unstable branches until "all tip nodes \[are] relatively stable or perhaps until some absolute depth-bound \[was] reached." (99)
A position is nonquiscence (non dead) if according to Shannon if "any piece is attacked by a piece of lower value, or by more pieces then defenses or if any check exists on a square controlled by the opponent." (99)
Basically, checks and captures are examined below the normal limit.

**The horizon effect. (p.100)**
Negative horizon effect:
- The program manipulates the timing of moves to push a undesirable effect out of view, resulting in it thinking that it has avoided it.
Positive horizon effect:
- The program incorrectly believes that a consequence it wants is achievable, or it fails to realize that the same consequence can be achieved later with more effect. 
They come because the definition of quiscence is over simplified.

Secondary search was proposed to deal with this, by looking at the best move so far, and then looking 2 ply deeper (plus quiscence). However, this only pushes the horizon back further.

**Iterative Deepening**
Used instead of a fixed depth, it does the search several times, sending back a result each time.
"A complete search, investigating all legal moves (subject to alpha-beta pruning), is done to depth 2, returning a move. The search is redone to depth 3, again to depth 4, and so on, until a preset time limit is exceeded. For efficiency, information from earlier iterations is saved for use in later ones." (100-101)

# Ordering of search
Fixed ordering method (101)
Refutation moves (102):
- For each move that is not the best move, strong replies should be considered first to show why the move is bad
- Typical implementations look at captures, and then killer moves:
	- *Killer move heuristic* "If a move has served as refutation in some previously examined position that is similar to the current one, it is likely a refutation in the current position, too." (102)

**Dynamic ordering:**
- if a good move turns out bad, it may be worth it to "go back, and reorder the move, and start with a different estimated best move" (102).
- It is disputed if this works, with some experiencing slowdowns, and others slight increases in speed

***Best first search**:
- Dynamic ordering expanded to re-ordering every time a node is expanded.
- The move considered next, therefore, is the most promising in the whole search tree.

# Width of search
*Full-width searching* = searching all legal moves (103)
**Forward Pruning (p.104)**
"\[generate] all legal moves at a position, use a fixed-ordering scheme to sort them according to their apparent goodness, or *plausibility* and disregard all but the best few moves." (104)
*Tapered forward pruning* = modification on Forward Pruning, where the number of moves retained scaled with the depth where they were generated.
- Greenblatt's program kept 15 moves from top 2 levels, 9 moves from next two, and 7 moves after
- Could be increased in special cases, such as to make sure several piece moves were considered

Nodes can also be pruned if they are outside the alpha beta value according to the static eval without further investigation, though this could remove good moves

**Goal-directed move generation (p.105)**
Generate moves only relevant to some goal
For example, goals could be:
- Material
- Center control
- Piece development
The goals were ordered based on importance, and then moves were generated to act on them.
# Knowledge-based programs
p.106
Moves generated depended on the current goal state, such as:
- Aggressive
- Preventative defense
- Nominal Defense
- Dynamic Defense
- Strategy
These states were mutually exclusive
State transitions took place dynamically as the tree was searched

Used to great effect for endgames.  The objective was to provide the engine enough knowledge to solve specific chess problems, such as King and Rook vs King and Knight.
The knowledge is encoded in a advice table,  within which "patterns occurring on the board may be described" (108). Each pattern has an "associated list of goals" (108). These can drastically reduce the calculation needed. For example, just giving the advice to avoid checkmate may result in a 85 ply calculation, while the advice to not lose the knight and to keep the king and knight together result in search to 10 ply being enough (108).