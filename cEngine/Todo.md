1. UCI Protocol
2. Bitboard move gen
3. Time management.
   - Define an optimal time and maximum time. Stop the search when time used reaches optimal time after completing a depth, and stop the search immediately when reaching the max time.
   - One legal move, move instantly
   - Early stop if forced mate found
   - time_for_move = total_time / (remaining_moves_estimate + safety_margin)
4. Crash Bugs
5. LazySMP improvements
   - Reintroduce- spin up threads once, send new jobs each iteration
   - Voting
   - History combine
6. Tapered Eval
7. NNUE?
