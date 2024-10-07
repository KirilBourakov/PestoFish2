ENG-STACKS Q 335 .R86 2021

**p.147**
A game can be fully defined with these elements:
- S<sub>0</sub>: The initial state (initial board position)
- To-move(s): The player who's turn it is to move in state s
- Actions(s): the set of legal moves in state s.
- Result(s,a): the **transition model**, which is the result of taking action a in state s
- Is-Terminal(s): true when the game is over and false other wise. 
- Utility(s,p): defines the numaeric value to player p when game ends in terminal state s. In chess, outcomes are 1 (win), 0 (loss), 1/2 (draw)

state space graph = graph where vertices are states, the edges are moves, and a state might be reached in several ways.

**p.148**
Minimax search = search the game tree assuming the enemy player will make the optimal moves at all time. 

**p.149**
Example of a minmax tree
![[2024_10_07 3_02 p.m. Office Lens.jpg]]
If the enemy is not playing perfectly it sometimes makes sense to make a risky move that say, gives up a guaranted draw for a good chance at winning. 

**p.150**
Minimax completes depth first search exploration of a game tree. If the max depth of the tree is m and there are b legal moves, the time complexity is O(b<sup>m</sup>). This is very bad.

**p.152**
**Alpha-beta pruning**: don't examine state that has no effect on the game

**p.153**
Example of alpha-beta pruning
![[2024_10_07 3_18 p.m. Office Lens.jpg]]
"The effectiveness of alpha beta pruning is highly dependent on the order in which the states are examined."

**p.155**
"For chess, a fairly simple ordering function (such as trying captures first, then threats, then forward moves, and then backward moves)" gets you to within about a factor of 2 of the best case"

**iterative deepening** "First, search one ply deep and record the ranking of the moves based on their evals. Then, search one ply deeper, using the previous ranking to inform move ordering; and so on."

**transposition table** caches the values of states to be reused.
In chess, these tables are considered very effective.


Claude Shannon proposed two strategies to deal with chess complexity
**Type A Strategy**
"considers all possible moves to a certain depth in the search tree"
"wide but shallow"
**Type B Strategy**
"ignores moves that look bad, and follows promising lines"
"deep but narrow"

Chess engines have been general type A, but recently type B has shown world champion level play in many games, including chess

**p.156**
because of limited compute, we replace the utility function with an eval one, which estimates the value of a position. 
replace terminal test with cutoff test, which returns true for a terminal state, or when the search should be ended

Good eval functions are fast, and strongly correlated with winning.

most eva functions combine different numerical contributions from a feature

**p.157**
Not all features are linear. For example, two bishops may be worth more then just the utility of a bishop \* 2

**p.158**
Using iterative deepening, and when time for the eval runs out, choose from the deepest layer. Use a transposition table to increase the speed of calculation for subsequent rounds.
A standard depth can also be used.

Evaluation function should only be applied to positions that are quiescent - positions with no pending moves that would wildly swing the eval. 

**horizon effect** arises when the program is facing a move that can cause serious damage and cannot be avoided, but can be delayed. This may lead to it sacing material to push the damage 'out of sight'. 

**p.159**
Singular extensions: extend the search tree for moves that are clearly better for all other moves to avoid this.

Forward pruning: prune out moves that are likely bad.
**beam search** consider only a beam of some number of best move. Dangerous as there is no guarantee good moves won't be pruned away.
**ProbCut** (probabilistic cut) uses states gained from prior experience to lessen a good move being cut. 

**p.160**
Late move reduction: reduce the depth on later moves in an ordered list of move. If the move comes back with a value above some constant, we do the search at full depth. 

Table lookup. Use known good moves (such as opening theory and ending tables). For first 10-15 moves lookup can be ok, then need to switch to evals. 

**p.161**
Retrograde minimax search: start with a set of positions. Reverse the rules of chess. "any move by White that, no matter what move Black responds with, ends up in a position marked as a win, must also be a win. Continue this search until all possible positions are resolved as win, loss, or draw, and you have an infallible lookup table for all endgames with those pieces"

Monte Carlo Tree search: value of a state is estimated via an average value of simulations of complete games starting from that state. Seems to be used for go, not chess. Preferred when branching factor is high or difficult to define an eval.

