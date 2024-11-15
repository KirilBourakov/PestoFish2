import csv
import numpy as np
from transformer import transform

def read(file_path):
    with open(file_path, newline='') as file:
        reader = csv.DictReader(file)
        features = []
        eval = []

        print('Reading...')
        for row in reader:
            if row['cp'] == '':
                continue

            s = row['fen'].split()
            board, color = s[0], s[1]
            features.append(transform(board, color))

            rating = float(row['cp'])
            if rating < 0:
                eval.append(max(rating, -64.0))
            else: 
                eval.append(min(rating, 64.0))         

    return (np.array(features, dtype=np.byte), np.array(eval, dtype=np.float16))
    