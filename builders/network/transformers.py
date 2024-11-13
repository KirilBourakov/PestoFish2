def transform_rev_1(row: dict[str, str], code_map):
    '''returns the board, + 3 more indexs for active_color, half_move, full_move'''
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

def transform_rev_2(row: dict[str, str], code_map):
    '''returns only the board'''
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

    return final