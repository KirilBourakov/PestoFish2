
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
    if move[0] in ["N", "B", "K", "Q", "R"]:
        print(move)
        return [(move[0], (letter_to_x_pos(move[-2]),int(move[-1])-1))]
    else:
        print(move)
        return [('P', (letter_to_x_pos(move[-2]),int(move[-1])-1))]
