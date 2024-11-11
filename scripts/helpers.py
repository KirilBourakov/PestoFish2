
def letter_to_x_pos(letter: str) -> int:
    assert len(letter) == 1
    return ord(letter) - ord('a')

def get_piece_and_location(move: str):
    if move == "0-0-0":
        pass
    if move == "0-0":
        pass

    if move[-1] == "+" or move[-1] == "#":
        move = move.replace("+", "").replace("#", "")

    # piece
    if move[0] in ["N", "B", "K", "Q", "R"]:
        return (move[0], (letter_to_x_pos(move[-2]),int(move[-1])-1))
    else:
        return ('P', (letter_to_x_pos(move[0]),int(move[1])-1))

print(get_piece_and_location("Kb4"))