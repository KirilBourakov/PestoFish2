# Citation
Q335 .C483 1985
Charniak, E., & McDermott, D. V. (1985). Introduction to artificial intelligence. Addison-Wesley.

# Meta
This book is interesting, but not very good right now due to age; it was published ~12 years before Deep Blue beat Gary Kasparov. 
Look into SSS*
A hashmap is likely more efficient and definingly simpler then a discrimination tree.
# Notes
"The guiding principle should be to keep track of the most attractive state as states are generated" (260)
Heuristic search is often used to describe a search where you examine the state that is the most appealing at the time, add the generated state to the states, order them, and do so again. (261)

"moves are explored down a few levels, and an evaluation function is applied to judge how 'good' the position is." (282)

SSS* = a type of algorithm that attempts to avoid exploring board positions that are irrelevant. It is like alpha-beta, but uses the values of partial solutions to guide the search. (290)

In games like chess, "brute force tree-search methods have done better then methods oriented around pattern recognition and planning" (291).

For "a realistic system in which state repetition is possible, every generated state must be stored in a table, and every new state must be looked up to make sure it has not been seen before" (295)

The book suggests a discrimination tree to store the state- that is a tree traversed in a direction dictated by a series of questions asked to the state. For example, if our state has 7 pawns, we might traverse to the left, while having more then 7 pawns results in a rightward traversal (295). 


