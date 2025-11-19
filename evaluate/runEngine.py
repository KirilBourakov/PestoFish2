import logging
from collections import defaultdict

import chess
import chess.engine
from tqdm import tqdm

ENGINE = r"D:\Core\Desktop\Projects\personal\Pestofish\cEngine\build\Debug\uci.exe"
OLD_ENGINE = r"D:\Core\Desktop\Projects\personal\Pestofish\evaluate\uci.exe"


def play_game(fen):
    engine1 = chess.engine.SimpleEngine.popen_uci(ENGINE)
    engine2 = chess.engine.SimpleEngine.popen_uci(OLD_ENGINE)
    board = chess.Board(fen=fen)

    while not board.is_game_over():
        if board.turn == chess.WHITE:
            result = engine2.play(board, chess.engine.Limit(time=0.1))
            move = result.move
        else:
            result = engine1.play(board, chess.engine.Limit(time=0.1))
            move = result.move

        board.push(move)

    engine1.quit()
    engine2.quit()

    return board.result()


# defaultdict(<class 'int'>, {'1/2-1/2': 246, '0-1': 1, '1-0': 3}) Original vs Original

# defaultdict(<class 'int'>, {'1/2-1/2': 247, '0-1': 3}) New V original

# defaultdict(<class 'int'>, {'1/2-1/2': 244, '0-1': 3, '1-0': 3}) Original v new

if __name__ == "__main__":
    games = 250
    results = defaultdict(int)
    with open("out.txt", "r") as file:
        for i in tqdm(range(games)):
            line = file.readline()
            results[play_game(line)] += 1

    print(results)
