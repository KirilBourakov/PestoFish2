Engine Cleanup todo:
- Merge negamax and root
  - Pass ply (or depth from root).
  - Move Generation: If ply == 0, use the pre-sorted rootMoves. If ply > 0, generate moves from the state.
  - Return: Standardize on returning the score. For the root node (ply == 0), update an out-parameter Move* rootBestMove or a class member bestRootMove whenever a new best score is found.
  - TT Cutoffs: Skip "return exact score" TT cutoffs at the root (ply == 0) to ensure the search completes and returns a valid move, or handle it carefully to extract the move from the TT.
- MovePicker Class
  - Replaces std::sort
- Improve search config encapsulation 
- Centralize constants
- Move LMR into helper
- Create should stop method
- Switch to one stop flag
- Move to dedicated search management?

5. Look into search issues.
2. Explore finding better magics.
3. Improve UCI Protocol
4. Improve Time Management
5. Crash Bugs
6. LazySMP improvements
   - Voting
   - History combine
