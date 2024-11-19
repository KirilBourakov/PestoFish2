import tensorflow as tf
import numpy as np

index = {
        'K': 0,
        'q': 1,
        'r': 2,
        'b': 3,
        'k': 4,
        'p': 5
    }
def get_color(piece: str) -> str:
    '''Give a piece, get the char that represents its color (will always be lowercase)'''
    return piece[0].lower()

def get_type(piece: str) -> str:
    '''Give a piece, get the char that represents its type'''
    return piece[1]

def is_empty(square: str)-> bool:
    '''returns if a square is empty. En passent is considered a empty square'''
    return len(square.strip()) == 0 or get_type(square) == 'e'

def transform(board, move_color):
    final_board = []
    for i in range(6):
        final_board.append([])
    for i in range(6):
        for j in range(8):
            final_board[i].append([0]*8)
    white = 1 if move_color == 'w' else -1
    for y, row in enumerate(board):
        for x, square in enumerate(row):
            if not is_empty(square):
                if get_color(square) == 'w':
                    final_board[index[get_type(square)]][y][x] = white
                else:
                    final_board[index[get_type(square)]][y][x] = white * -1 
    return np.array(final_board, dtype=np.float32).reshape(-1, 6, 8, 8)

board1 = [['  ', '  ', '  ', '  ', 'BK', '  ', '  ', '  '],
                ['wq', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wq', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', 'WK', '  ', '  ', '  ']]

board2 = [['Br', 'bk', 'bb', 'bq', 'BK', 'bb', 'bk', 'Br'],
                ['bp', 'bp', 'bp', 'bp', 'bp', 'bp', 'bp', 'bp'],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['wp', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp'],
                ['Wr', 'wk', 'wb', 'wq', 'WK', 'wb', 'wk', 'Wr']]

model = tf.keras.models.load_model("m.keras")

print(model(transform(board1, 'w')))