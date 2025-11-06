1. Tapered Eval
2. Quiescence search
3. Time limit based searching, not depth limit
2. Killer moves
2. Iterative Deepening
3. Multithreading
   - Shared SMP seems to be the way to go.
   - Move history needs to be copied per thread, then recombined. Also, regularized
2. Possible capture bug? (black Knight captured white pawn, but pawn was still there. When pawn captured, it became a black knight)
3. Crash on victory
