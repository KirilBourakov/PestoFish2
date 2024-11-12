import sys
import math
import string
from default_map import heat_hash
from helpers import get_piece_and_location

CAP = sys.argv[3]

def contains(input: str, target: str) -> bool:
    return input.count(target) > 0

def format_for_file(hash):
    for gamestate in hash:
        for color in hash[gamestate]:
            for piece in hash[gamestate][color]:
                li = hash[gamestate][color][piece]
                largest = 0
                for row in li:
                    largest = max(largest, max(row))
                
                for y, row in enumerate(li):
                    for x, square in enumerate(row):
                        if largest != 0:
                            li[y][x] = math.ceil((square/largest)*40)
    depth = 0
    final = '{\n'
    for gamestate in hash:
        depth += 1
        final += str(gamestate) + get_white_space(depth) + ":{\n"
        for color in hash[gamestate]:
            depth += 1
            final += color + get_white_space(depth) + ":{\n"
            for piece in hash[gamestate][color]:
                depth += 1
                final += get_white_space(depth) + piece + ":[\n"
                li = hash[gamestate][color][piece]
                for row in li:
                    final += get_white_space(depth) + str(row) + ",\n"
                
                final += get_white_space(depth) + "]\n"
                depth -= 1
            
            final += get_white_space(depth) + "}\n"
            depth -= 1
        
        final += get_white_space(depth) + "}\n"
        depth -= 1
    
    final += get_white_space(depth) + "}\n"  
    depth -= 1
    return final
                
def get_white_space(depth):
    return ' ' * depth

def contains_number(input: str):
    for char in input:
        if char.isnumeric():
            return True
    return False

def contains_punc(input: str):
    for char in input:
        if char in string.punctuation:
            return True
    return False

def handle_game(game: str) -> None:
    # if the line has an unexpected value, just ignore it
    moves: list[str] = game.split()

    move_count = 1
    white_move = True
    for move in moves:
        if len(move) == 0 or len(move) > 5 or not (contains_number(move)) or contains_punc(move):
            continue
        # move number indicator
        if move[0].isnumeric():
            move_count += 1
            white_move = True
            continue
        if move[0].isalpha() and move[-1] != '.':
            # handle the move
            try:
                index = "W" if white_move else "B"
                end_game = move_count > 42
                converted = get_piece_and_location(move, index)
                for c in converted:
                    piece, pos = c 
                    posx, posy = pos
                    heat_hash[end_game][index][piece][posy][posx] += 1
            except:
                pass
        else:
            break

def main() -> None:
    read_file = open(sys.argv[1], "r", encoding="cp437")
    write_file = open(sys.argv[2], "a")
    game: int = 1
    game_moves: str = ""
    reading_game: bool = False
    for line in read_file:
        if game == CAP:
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

    write_file.write(format_for_file(heat_hash))
    read_file.close()
    write_file.close()



if __name__ == "__main__":
    main()