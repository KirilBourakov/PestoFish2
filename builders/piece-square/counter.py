import sys
import csv
import json

translation_map = {
    'P': 'wp',
    'N': 'wk',
    'B': 'wb',
    'R': 'wr',
    'Q': 'wq',
    'K': 'wK',

    'p': 'bp',
    'n': 'bk',
    'b': 'bb',
    'r': 'br',
    'q': 'bq',
    'k': 'bK',
}

def update(locations, evaluation, board):
    wp_count = (board.count("N") + board.count("B") + board.count("R") + board.count("Q"))
    bp_count = (board.count("n") + board.count("b") + board.count("r") + board.count("q"))
    is_endgame = str(wp_count <= 4 and bp_count <= 4)

    x = 0
    y = 0

    for char in board:
        if char == "/":
            y += 1
            x = 0

        elif char.isnumeric():
            x += int(char)
        
        
        elif char in translation_map:
            if abs(evaluation) < 0.5:
                locations[is_endgame][translation_map[char]][y][x] += 1
            elif evaluation > 0 and char.isupper():
                locations[is_endgame][translation_map[char]][y][x] += 1
            elif evaluation < 0 and char.islower():
                locations[is_endgame][translation_map[char]][y][x] += 1
            x += 1
    return

def gen_locations():
    if len(sys.argv) > 2:
        print('loading...')
        return json.load(open('out.json'))
    locations = {}
    for i in ['True', 'False']:
        locations[i] = {}

        for x in translation_map:
            li = []
            for j in range(8):
                li.append([0] * 8)
            locations[i][translation_map[x]] = li

    return locations

def main():
    locations = gen_locations()

    with open(sys.argv[1], newline='') as file:
        reader = csv.DictReader(file)
        for row in reader:
            if 'cp' in row:
                if len(row['cp'].strip()) == 0:
                    continue
                evaluation = float(row['cp'])
                board = row['fen'].split(" ")[0]
            else:
                if row['Evaluation'].count('#') > 0:
                    continue
                evaluation = float(row['Evaluation'])
                board = row['FEN'].split(" ")[0]
            update(locations, evaluation, board)

    with open('out.json', 'w') as out:
        out.write(json.dumps(locations, indent=4))

if __name__ == '__main__':
    main()