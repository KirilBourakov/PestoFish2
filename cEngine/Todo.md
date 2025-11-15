1. UCI Protocol
2. Bitboard move gen
3. Cleanup Engine paramaters
4. Time management.
   - Define an optimal time and maximum time. Stop the search when time used reaches optimal time after completing a depth, and stop the search immediately when reaching the max time.
   - One legal move, move instantly
   - Early stop if forced mate found
   - time_for_move = total_time / (remaining_moves_estimate + safety_margin)
6. Crash Bugs
6. Draw detection
7. LazySMP improvements
   - Voting
   - History combine
8. Tapered Eval
9. NNUE?
