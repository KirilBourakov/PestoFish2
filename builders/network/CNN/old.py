import sys
import tensorflow as tf
import time
from sklearn.model_selection import train_test_split
import numpy as np
import csv

def main():
    model = tf.keras.Sequential([
        tf.keras.layers.Conv2D(8, (3,3), activation='relu', input_shape=(8,8,6)),
        tf.keras.layers.Conv2D(16, (3,3), activation='relu'),
        tf.keras.layers.Conv2D(32, (3,3), activation='relu'),
        tf.keras.layers.Conv2D(64, (2,2), activation='relu'),

        tf.keras.layers.Flatten(),

        tf.keras.layers.Dense(64),
        tf.keras.layers.Dense(1)
    ])

    model_checkpoint_callback = tf.keras.callbacks.ModelCheckpoint(
        filepath="models/small_adam.weights.h5",
        monitor='loss',
        mode='min',
        save_best_only=True,
        save_weights_only=True,
    )

    model.compile(optimizer='adam', loss='mae', metrics=['mae'])
    for i in range(6):
        features, evals = read(f"{sys.argv[1]}/{i+1}.csv")
        model.fit(features, evals, epochs=5, callbacks=[model_checkpoint_callback])
    features, evals = read(f"{sys.argv[1]}/{7}.csv")
    model.evaluate(features, evals)

    model.save(f"model_{time.ctime(time.time())}.keras".replace(":", "."))

def read(file_path):
    with open(file_path, newline='') as file:
        reader = csv.DictReader(file)

        data = []
        labels = []

        for row in reader:
            if row['Evaluation'].count('#'):
                continue

            s = row['FEN'].split()
            board, color = s[0], s[1]
        
            rating = float(row['Evaluation'])
            if rating < 0:
                rating = max(int(rating), -127)
            else: 
                rating = min(int(rating), 127)
            labels.append(rating)
            data.append(transform(board, color))

    return (np.array(data, dtype=np.byte), np.array(labels, dtype=np.byte))
                

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
    f = final_board
    return f


if __name__ == '__main__':
    main()