import sys
from default_map import heat_hash
from helpers import get_piece_and_location

def contains(input: str, target: str) -> bool:
    return input.count(target) > 0

def handle_game(game: str) -> None:
    # if the line has an unexpected value, just ignore it
    moves: list[str] = game.split()

    move_count = 1
    white_move = True
    for move in moves:
        if len(move) == 0 or len(move) > 5:
            continue
        # move number indicator
        if move[0].isnumeric():
            move_count += 1
            white_move = True
            continue
        if move[0].isalpha():
            # handle the move
            index = "W" if white_move else "B"
            end_game = move_count > 42
            converted = get_piece_and_location(move, index)
            for c in converted:
                piece, pos = c 
                posx, posy = pos
                heat_hash[end_game][index][piece][posy][posx] += 1
        else:
            break

def main() -> None:
    read_file = open(sys.argv[1], "r", encoding="cp437")
    write_file = open('output.txt', "a")
    game: int = 1
    game_moves: str = ""
    reading_game: bool = False
    for line in read_file:
        if game == 3:
            break
        line: str = line.strip()
        if line.count("[") > 0:
            continue
        
        has_resoluton: bool = contains(line, "0-1") or contains(line, "1-0") or  contains(line, "1/2-1/2")
        if reading_game and has_resoluton:
            game += 1
            game_moves += line
            handle_game(game_moves)
            game_moves = ""
            
        if not reading_game and len(line) != 0 and line[0] == "1":
            reading_game = True
        if reading_game:
            game_moves += line

    write_file.write(str(heat_hash))
    read_file.close()
    write_file.close()

if __name__ == "__main__":
    main()