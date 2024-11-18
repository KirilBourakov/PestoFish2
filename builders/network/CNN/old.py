import sys
import tensorflow as tf
import time
from sklearn.model_selection import train_test_split
import numpy as np
import csv

def main():
    model = tf.keras.Sequential([
        tf.keras.layers.Conv2D(64, (3,3), input_shape=(6,8,8), activation='relu', padding="same"),
        
        tf.keras.layers.Flatten(),

        tf.keras.layers.Dense(2048, activation='softmax'),
        tf.keras.layers.Dense(2048, activation='softmax'),

        tf.keras.layers.Dense(1)
    ])

    model_checkpoint_callback = tf.keras.callbacks.ModelCheckpoint(
        filepath="models/6x8x8_full.weights.h5",
        monitor='loss',
        mode='min',
        save_best_only=True,
        save_weights_only=True,
    )
    # model.load_weights("models/6x8x8_full.weights.h5")
    model.compile(optimizer='adam', loss='mse', metrics=['mae'])

    for j in range(3):
        print('epoch:', j)
        for i in range(140):
            print('part:', i)
            features, evals = read(f"{sys.argv[1]}/{i+1}.csv")
            model.fit(features, evals, epochs=1, callbacks=[model_checkpoint_callback])

    features, evals = read(f"{sys.argv[1]}/{7}.csv")
    model.evaluate(features, evals)

    model.save(f"model_{time.ctime(time.time())}.keras".replace(":", "."))

def read(file_path):
    with open(file_path, newline='') as file:
        reader = csv.DictReader(file)

        data = []
        labels = []

        for row in reader:
            if len(row['cp'].strip()) == 0:
                continue
            # if row['Evaluation'].count('#') > 0:
            #     continue

            s = row['fen'].split()
            board, color = s[0], s[1]
        
            rating = float(row['cp'])
            if rating < 0:
                rating = max(int(rating), -32767)
            else: 
                rating = min(int(rating), 32767)
            labels.append(rating)
            data.append(transform(board, color))

    return (np.array(data, dtype=np.byte), np.array(labels, dtype=np.short))
                

def transform(fen_board, color_to_move):
    final_board = []
    for i in range(6):
        final_board.append([])
    for i in range(6):
        for j in range(8):
            final_board[i].append([0]*8)

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
                final_board[index[letter]][y][x_true_index] = white * -1
            else:
                final_board[index[letter.lower()]][y][x_true_index] = white
            x_true_index += 1
            x_fen_index += 1
    f = final_board
    return f


if __name__ == '__main__':
    main()