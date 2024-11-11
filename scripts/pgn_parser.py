import sys
from default_map import heat_hash

def contains(input: str, target: str):
    return input.count(target) > 0

def main():
    read_file = open(sys.argv[1], "r")
    write_file = open('output.txt', "a")

    reading_game = False
    game = []
    for line in read_file:
        line = line.strip()
        if line.count("[") > 0:
            continue
        
        has_resoluton = contains(line, "0-1") or contains(line, "1-0") or  contains(line, "1/2-1/2")
        if reading_game and has_resoluton:
            game.append(line)
            break
        if not reading_game and len(line) != 0 and line[0] == "1":
            reading_game = True
        if reading_game:
            game.append(line)

    read_file.close()
    write_file.close()

if __name__ == "__main__":
    main()