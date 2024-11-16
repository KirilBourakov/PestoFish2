import csv, itertools
import numpy as np
import tensorflow as tf

def read(file_path, size):
    with open(file_path, newline='') as file:
        reader = csv.DictReader(file)

        data = []
        labels = []

        for row in itertools.cycle(reader):
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
            data.append(transform_13(board, color))

            if len(labels) >= size:
                yield (np.array(data, dtype=np.byte), np.array(labels, dtype=np.byte))
                labels = []
                data = []

def transform_13(fen_board, color_to_move):
    final_board = []
    for i in range(8):
        final_board.append([])
    for i in range(8):
        for j in range(8):
            final_board[i].append([0]*13)

    index = {
        'k': 0,
        'q': 1,
        'r': 2,
        'b': 3,
        'n': 4,
        'p': 5,

        'K': 6,
        'Q': 7,
        'R': 8,
        'B': 9,
        'N': 10,
        'P': 11,

        ' ': 12
    }

    fen_board = fen_board.split('/')
    for y,row in enumerate(fen_board):
        x_fen_index = 0
        x_true_index = 0
        while x_true_index < 8:
            letter = row[x_fen_index]
            if letter.isdigit():
                for i in range(int(letter)):
                    final_board[y][x_true_index][index[' ']] = 1
                    x_true_index += 1
            else:
                final_board[y][x_true_index][index[letter]] = 1
                x_true_index += 1
            x_fen_index += 1
    return np.array(final_board)

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
