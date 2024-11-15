import numpy as np
import sys
import tensorflow as tf
import csv
import itertools
RESTART_POINT = 10
def main():
    # o_f, o_v = read(sys.argv[1])
    # o_f = np.reshape(o_f, newshape=(-1,8))
    # np.savetxt('10_pos_.txt.gz', o_f)
    # np.savetxt('10_eval_.txt.gz', o_v)

    model = tf.keras.Sequential([
        tf.keras.layers.Conv2D(8, (3,3), activation='relu', input_shape=(8,8,6)),
        tf.keras.layers.Conv2D(16, (3,3), activation='relu'),
        tf.keras.layers.Conv2D(32, (3,3), activation='relu'),
        tf.keras.layers.Conv2D(64, (2,2), activation='relu'),

        tf.keras.layers.Flatten(),
        tf.keras.layers.Dense(64),
        tf.keras.layers.Dense(32),
        tf.keras.layers.Dense(1)
    ])
    model.compile(optimizer='adam', loss='mae', metrics=['mae'])
    model.fit(read('1.csv'), epochs=10, steps_per_epoch=5)

def read(file_path):
    with open(file_path, newline='') as file:
        reader = csv.DictReader(file)
        features = []
        eval = []
        for row in itertools.cycle(reader):
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
            yield(np.array(features, dtype=np.byte), np.array(eval, dtype=np.float16))


def transform(fen_board, color_to_move):
    final_board = []
    for i in range(8):
        final_board.append([])
    for i in range(8):
        for j in range(8):
            final_board[i].append([0]*6)

    index = {
        'k': 0,
        'q': 1,
        'r': 2,
        'b': 3,
        'n': 4,
        'p': 5
    }
    fen_board = fen_board.split('/')
    for y,row in enumerate(fen_board):
        x_fen_index = 0
        x_true_index = 0
        while x_true_index < 8:
            letter = row[x_fen_index]
            if letter.isdigit():
                x_true_index += int(letter)
                x_fen_index += 1
                continue
            
            white = 1 if color_to_move == "w" else -1
            if letter.islower():
                final_board[y][x_true_index][index[letter]] = white * -1
            else:
                final_board[y][x_true_index][index[letter.lower()]] = white
            x_true_index += 1
            x_fen_index += 1
    f = np.array(final_board)
    return f

main()