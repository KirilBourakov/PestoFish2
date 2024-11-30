import json
import math
import sys

def subtract_avg(counter, outfile):
    for endgame_state in counter:
        endgame_states = counter[endgame_state]
        for piece_map_key in endgame_states:
            piece_map = endgame_states[piece_map_key]
            avg = 0
            for row in piece_map:
                avg += sum(row)
            avg = math.floor(avg/64)
            
            def mapper(x):
                if x == 0:
                    return 0
                else:
                    return x-avg

            for i, row in enumerate(piece_map):
                piece_map[i] = list(map(mapper, row))

            endgame_states[piece_map_key] = piece_map

    with open(outfile, 'w') as out:
        out.write(json.dumps(counter, indent=4))

if __name__ == "__main__":
    subtract_avg(json.load(open(sys.argv[1])), sys.argv[2])