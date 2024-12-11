import numpy as np
import csv
def read_lichess(file_path):
    with open(file_path, newline='') as file:
        reader = csv.DictReader(file)

        data = []
        labels = []

        for row in reader:
            if len(row['cp'].strip()) == 0:
                continue

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

def read_lichess_double(file_path):
    with open(file_path, newline='') as file:
        reader = csv.DictReader(file)

        data = []
        colors = []
        labels = []

        for row in reader:
            if len(row['cp'].strip()) == 0:
                continue

            s = row['fen'].split()
            board, color = s[0], s[1]

            to_mv = 1 if color == 'w' else -1
            colors.append(to_mv)
        
            rating = float(row['cp'])
            if rating < 0:
                rating = max(int(rating), -32767)
            else: 
                rating = min(int(rating), 32767)
            labels.append(rating)
            data.append(transform_new(board))

    return (np.array(data, dtype=np.byte), np.array(colors, dtype=np.byte), np.array(labels, dtype=np.short))

def read_other_double(file_path):
    with open(file_path, newline='') as file:
        reader = csv.DictReader(file)

        data = []
        colors = []
        labels = []

        for row in reader:
            if row['Evaluation'].count('#') > 0:
                continue

            s = row['FEN'].split()
            board, color = s[0], s[1]
            to_mv = 1 if color == 'w' else -1
            colors.append(to_mv)
        
            rating = float(row['Evaluation'])
            if rating < 0:
                rating = max(int(rating), -32767)
            else: 
                rating = min(int(rating), 32767)
            labels.append(rating)
            data.append(transform_new(board))

    return (np.array(data, dtype=np.byte), np.array(colors, dtype=np.byte), np.array(labels, dtype=np.short))             

def read_other(file_path):
    with open(file_path, newline='') as file:
        reader = csv.DictReader(file)

        data = []
        labels = []

        for row in reader:
            if row['Evaluation'].count('#') > 0:
                continue

            s = row['FEN'].split()
            board, color = s[0], s[1]
        
            rating = float(row['Evaluation'])
            if rating < 0:
                rating = max(int(rating), -32767)
            else: 
                rating = min(int(rating), 32767)
            labels.append(rating)
            data.append(transform(board, color))

    return (np.array(data, dtype=np.byte), np.array(labels, dtype=np.short))             

def transform_new(fen_board):
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
            
            if letter.islower():
                final_board[index[letter]][y][x_true_index] = -1
            else:
                final_board[index[letter.lower()]][y][x_true_index] = 1
            x_true_index += 1
            x_fen_index += 1
    f = final_board
    return f

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
