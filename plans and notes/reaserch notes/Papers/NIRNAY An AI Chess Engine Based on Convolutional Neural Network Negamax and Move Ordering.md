# Citation
Agarwal, S., Dash, S., Saini, A., Singh, N. K., Kumar, A., Diwakar, M., & 2024 11th International Conference on Reliability, Infocom Technologies and Optimization (Trends and Future Directions) (ICRITO) Noida, India 2024 March 14 - 2024 March 15. (2024). NIRNAY: An AI Chess Engine Based on Convolutional Neural Network, Negamax and Move Ordering. In 2024 11th International Conference on Reliability, Infocom Technologies and Optimization (Trends and Future Directions) (ICRITO) (pp. 1–6). https://doi.org/10.1109/ICRITO61523.2024.10522402

https://unb.on.worldcat.org/search/detail/10242809128?queryString=chess%20engine&clusterResults=true&groupVariantRecords=false
# Meta
The CNN here is not used as an evaluation function
# Notes
The engine works in three phases:
1. "\[T]he engine will discard 50 % of the potential moves possible in each chess position by calculating the probability of whether move is a “good move”"
2. With negamax, the search tree is calculated to a certain depth 
3. Eval function applied
The CNN here uses and SELU activation function, "squared error as loss function and Adam optimizer for optimization"
The CNN here is used to calculate the possibility of a legal move being 'good'.
Evaluation function uses material value, and piece position

Move ordering used to increase alpha beta pruning speed

Estimated elo of 1450-1520