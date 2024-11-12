
def letter_to_x_pos(letter: str) -> int:
    assert len(letter) == 1
    return ord(letter) - ord('a')

def get_piece_and_location(move: str, color: str):
    if move == "O-O-O":
        index = 0 if color == "W" else 7
        return [("K", (index, 2)), ("R", (index, 3))]
    if move == "O-O":
        index = 0 if color == "W" else 7
        return [("K", (index, 6)), ("R", (index, 5))]

    if move[-1] == "+" or move[-1] == "#":
        move = move.replace("+", "").replace("#", "")

    # piece
    try:
        if move[0] in ["N", "B", "K", "Q", "R"]:
            return [(move[0], (letter_to_x_pos(move[-2]),int(move[-1])-1))]
        else:
            if move.count("(") > 0 or move.count("=") > 0:
                start = move.index("=")
                return [('P', (letter_to_x_pos(move[start-2]),int(move[start-1])-1))]
            return [('P', (letter_to_x_pos(move[-2]),int(move[-1])-1))]
    except ValueError:
        return [("", (10,10))]
