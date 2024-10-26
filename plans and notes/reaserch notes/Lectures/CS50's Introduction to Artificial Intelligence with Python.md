

# Citation 
https://www.youtube.com/watch?v=WbzNRTTrX0g  Search - Lecture 0 - CS50's Introduction to Artificial Intelligence with Python 2020

# Adversarial Search
- A search situation where there is someone/something trying to make intelligent decisions and someone/something else is trying to make the first party fail. Example: Tic tac toe
![[Pasted image 20241013170325.png]]
X is trying to get 3 in a row and O is trying to stop it.

# Minimax Algorithm
- These kinds of algorithms work best in situations where there is at least 2 people where both are trying to win, meaning one is trying to win while the other is trying to make the other player lose.
- In the algorithm one player is assigned the max meaning the player is trying to maximize the score and the other is assigned the min meaning the player is trying to minimize the score.
- The logic behind minimax is considering all of the possible actions I can take and then to consider what the opponent will do and then consider what I would do to that opponents move. This loop will continue until the game is over and then that branch will stop growing and other branches will be looked at. Once every branch is done the minimum value player will choose the minimum value branch and the maximum value player will choose the maximum one. 
![](Pasted image 20241013171504.png)
- Every level of the tree corresponds to one move.



# Minimax pseudocode
![](Pasted image 20241013172439.png)

![](Pasted image 20241013172821.png)
- The function MAX-VALUE grabs a state of the board and first checks if the game is over. If the game is over then a utility function tells the program what the state of the board is (Basically who won).
- If not then we set a value the smallest it can be which is negative infinity. This is done because if the value is the smallest, then there is a value that is bigger (better) then that value.
- A loop starts which equals the value to look into the best move of the opponent and compare it to my best move then choose the maximum value.

![](Pasted image 20241013172831.png)
- The MIN-VALUE function does the exact opposite.
- Checks the best move of my moves, checks the opponents best move, choose the minimum of the two.

# Optimization
![](Pasted image 20241013174244.png)
- There is a lot of computing necessary to use the minimax function. To optimize we use **alpha-beta pruning**. 
- After checking one state and getting the best value of the opponent, 4 in this case, start checking the other one and stop when we get something less then 4. After seeing something less then 4, we don't need to check more of the options because we know we will not choose that branch.


!!! Even though the alpha-beta pruning method helps a lot for the minimax algorithm, it is not enough. Even a basic game like tic tac toe has around 255k possible games while something like chess has more then 10 to the power of 29000. No computer can go through that many situations in a reasonable amount of time so we use something called **Depth-Limited Minimax**.


- Depth Limited Minimax unlike normal Minimax stops after a certain amount of moves and doesn't consider more in favor of computational speed.
- For situations where the game isn't over we need an additional evaluation function that estimates the expected state of a board.
- ==**The better the evaluation function the better the ai will play.**==



