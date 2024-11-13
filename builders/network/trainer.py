import csv
import tensorflow as tf
from sklearn.model_selection import train_test_split
import numpy as np
import sys

code_map = {
    '  ': 1,
    'wr': 2,
    'Wr': 3,
    'wk': 4,
    'wb': 5,
    'wq': 6,
    'wK': 7,
    'WK': 8,
    'we': 9,
    'wp': 10,

    'br': 22,
    'Br': 23,
    'bk': 24,
    'bb': 25,
    'bq': 26,
    'bK': 27,
    'BK': 28,
    'be': 29,
    'bp': 210,
}

def main():
    with open(sys.argv[1], newline='') as file:
        reader = csv.DictReader(file)
        features = []
        eval = []
        for row in reader:
            features.append(transform(row))
            eval.append(int(row['Evaluation'].replace('#', '')))
    eval = np.array(eval)
    features = np.array(features)
    model(eval, features)

def model(eval, features):
    training_features, testing_features, training_eval, testing_eval = train_test_split(features, eval, test_size=0.5)    

    model = tf.keras.models.Sequential()
    model.add(tf.keras.layers.Dense(67, input_shape=(67,)))
    model.add(tf.keras.layers.Dense(12))
    model.add(tf.keras.layers.Dense(1))
    model.compile(optimizer='adam', loss='mae', metrics=['accuracy'])
    model.fit(training_features, training_eval, epochs=10)
    model.evaluate(testing_features, testing_eval)


def transform(row: dict[str, str]):
    fen = row['FEN']
    rows = fen.split('/')
    remaining = ''
    board = [[], [], [], [], [], [], [], []]
    for y,row in enumerate(rows):
        for x, letter in enumerate(row):
            if letter == ' ':
                remaining = row[x:]
                break
            if letter.isdigit():
                for i in range(int(letter)):
                    board[y].append('  ')
            elif letter.islower():
                converted = 'k' if letter == 'n' else letter
                converted = 'K' if converted == 'k' else converted.lower()
                board[y].append(f'b{converted}')
            elif letter.isupper():
                converted = 'k' if letter == 'N' else letter
                converted = 'K' if converted == 'K' else converted.lower()
                board[y].append(f'w{converted}')
    

    remaining = remaining.split()

    # get active color
    active_color = 0 if remaining[0].strip() == 'w' else 1

    # handle castling
    if remaining[1].strip() != "-":
        for letter in remaining[1].strip():
            x = 7 if letter.lower() == 'k' else 0
            y = 7 if letter.isupper() else 0
            board[y][x] = board[y][x][0].upper() + board[y][x][1]
            board[y][4] = board[y][4][0].upper() + board[y][4][1]

    # handle enpassent
    if remaining[2].strip() != "-":
        location = remaining[2].strip()
        xPos = ord(location[0]) - ord('a')

        pawnY = int(location[1])
        color = 'w' if pawnY == 3 else 'b'
        factor = -1 if color == 'b' else 1
        yPos = 7 - pawnY + factor

        board[yPos][xPos] = f"{color}e"
    
    final = []
    for y in range(len(board)):
        for x in range(len(board)):
            final.append(code_map[board[y][x]])

    # half move clock
    half_move = int(remaining[3].strip())

    # full move clock (likly irrelevent, try removing)
    full_move = int(remaining[4].strip())

    final.append(active_color)
    final.append(half_move)
    final.append(full_move)
    return final

    

if __name__ == "__main__":
    main()