1. Tapered Eval
2. Quiescence search
3. Time limit based searching, not depth limit
3. Killer moves
4. Multithreading
   - Desync
   - Move history needs to be copied per thread, then recombined. Also, regularized
5. Possible capture bug? (black Knight captured white pawn, but pawn was still there. When pawn captured, it became a black knight)
6. Crash on victory
