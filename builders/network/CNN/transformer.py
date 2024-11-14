
def transform(fen_board, color_to_move):
    '''Turns data from fen, to board representaion, to bitboard (8x8x6):
        
    Row
        [
           Col
           [
                Piece
                [-1, 0, 0, 0, 0 ... 0],
                [0, 0, 0, 0, 1 ... 0],
                ...
                [0, 0, 0, 0, 1 ... 0],
           ],
           [
                [-1, 0, 0, 0, 0 ... 0],
                [-1, 0, 0, 0, 0 ... 0],
                ...
                [-1, 0, 0, 0, 0 ... 0],
           ]
        ]
    '''
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

    return final_board