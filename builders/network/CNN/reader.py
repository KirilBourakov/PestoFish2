import csv
import numpy as np
from transformer import transform

def read(file_path):
    with open(file_path, newline='') as file:
        reader = csv.DictReader(file)
        features = []
        eval = []

        print('Reading...')
        x = 0
        for row in reader:
            if x % 100 == 0:
                print(f"At row {x}")
            s = row['FEN'].split()
            board, color = s[0], s[1]
            features.append(transform(board, color))

            x += 1
            rating = row['Evaluation']
            if rating[0] == "#":
                eval.append(128)
            else:
                rating = int(rating)
                if rating < 0:
                    eval.append(max(rating, -64))
                else: 
                    eval.append(min(rating, 64))
    return (np.array(features), np.array(eval))
    