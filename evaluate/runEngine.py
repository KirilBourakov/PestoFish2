import logging
import os
from collections import defaultdict
from pathlib import Path
from plistlib import InvalidFileException
from typing import Sequence

import chess
import chess.engine
from chess.engine import SimpleEngine
from sqlalchemy import create_engine, select, exists
from sqlalchemy.orm import Session
from tqdm import tqdm

from Models import Engine, Tournament, ChessPosition, Match

ENGINE = r"E:\Programming\Projects\PestoFish2\evaluate\bitboards.exe"
OLD_ENGINE = r"E:\Programming\Projects\PestoFish2\evaluate\normal.exe"


def play_game(fen: str, white: str, black: str) -> str:
    white_engine = chess.engine.SimpleEngine.popen_uci(white)
    black_engine = chess.engine.SimpleEngine.popen_uci(black)

    board = chess.Board(fen=fen)

    while not board.is_game_over():
        if board.turn == chess.WHITE:
            result = white_engine.play(board, chess.engine.Limit(time=0.1))
            move = result.move
        else:
            result = black_engine.play(board, chess.engine.Limit(time=0.1))
            move = result.move

        board.push(move)

    white_engine.quit()
    black_engine.quit()

    return board.result()

def get_inputs() -> tuple[str, int, tuple[str, str], tuple[str, str]]:
    db_name = input("Enter database name [main.db]: ")
    if db_name.strip() == "":
        db_name = "main.db"

    games = input("Enter number of games to play [300]: ")
    if games.strip() == "":
        games = 300
    else:
        games = int(games)

    white = input("Engine for white: ")
    black = input("Engine for black: ")

    return db_name, games, validate_engine(white), validate_engine(black)

def validate_engine(engine: str) -> tuple[str, str]:
    path = Path.cwd() / "engines" / engine
    if os.path.isdir(path):
        exes = []
        for filename in os.listdir(path):
            full_path = path / filename
            if filename.lower().endswith(".exe") and os.path.isfile(full_path):
                exes.append(full_path)
        if len(exes) == 1:
            return engine, exes[0]
        else:
            raise InvalidFileException(f"Invalid engine folder format; {len(exes)} .exe files found.")
    else:
        raise FileNotFoundError(f"{engine} not found.")

def get_engine_model(session: Session, name: str) -> Engine:
    stmt = select(Engine).where(Engine.name == name)
    engine_obj = session.scalar(stmt)

    if engine_obj is None:
        engine_obj = Engine(name=name)
        session.add(engine_obj)
        session.commit()
        session.refresh(engine_obj)

    return engine_obj

def main():
    db_name, games, (white_name, white_engine), (black_name, black_engine) = get_inputs()

    db_engine = create_engine(f"sqlite:///database/{db_name}", echo=True)

    with Session(db_engine) as session:
        white_engine_model = get_engine_model(session, white_name)
        black_engine_model = get_engine_model(session, black_name)

        tournament_model = Tournament(white_engine_id=white_engine_model.id, black_engine_id=black_engine_model.id)
        session.add(tournament_model)
        session.commit()
        session.refresh(tournament_model)

        # Fetch positions
        stmt = select(ChessPosition).limit(games)
        positions: Sequence[ChessPosition] = session.scalars(stmt).all()

        for position in tqdm(positions):
            result = play_game(position.fen, white_engine, black_engine)
            match_model = Match(tournament_id=tournament_model.id, position_id=position.id, result=result)
            session.add(match_model)
            session.commit()

if __name__ == "__main__":
    main()
