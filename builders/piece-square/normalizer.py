import json
import math
import sys

def normalize(counter, outfile, cap):
    for endgame_state in counter:
        endgame_states = counter[endgame_state]
        for piece_map_key in endgame_states:
            piece_map = endgame_states[piece_map_key]

            highest = 0
            for row in piece_map:
                highest = max(row + [highest])
                

            for i, row in enumerate(piece_map):
                piece_map[i] = list(map(lambda x : x/highest*cap, row))

            endgame_states[piece_map_key] = piece_map

    with open(outfile, 'w') as out:
        out.write(json.dumps(counter, indent=4))

if __name__ == "__main__":
    file, out, cap = open(sys.argv[1]), sys.argv[2], int(sys.argv[3])

    normalize(json.load(file), out, cap)