import logging

import chess
import chess.engine


def play_game(engine1_path, engine2_path):
    engine1 = chess.engine.SimpleEngine.popen_uci(engine1_path)
    engine2 = chess.engine.SimpleEngine.popen_uci(engine2_path)
    board = chess.Board()

    while not board.is_game_over():
        if board.turn == chess.WHITE:
            result = engine1.play(board, chess.engine.Limit(time=2))
            move = result.move
        else:
            result = engine2.play(board, chess.engine.Limit(time=0.1))
            move = result.move

        print("---------------")
        print(board)
        board.push(move)

    print(board.fen())
    print("Result:", board.result())

    engine1.quit()
    engine2.quit()


if __name__ == "__main__":
    play_game(
        r"D:\Core\Desktop\Projects\personal\Pestofish\cEngine\build\Debug\uci.exe",
        r"D:\Core\Desktop\Projects\personal\Pestofish\cEngine\build\Debug\uci.exe",
    )
