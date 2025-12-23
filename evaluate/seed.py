"""
This script seeds the database with sets of runs. Takes in csv with a 'fen' and a 'score' column
"""
import csv

from sqlalchemy import create_engine
from sqlalchemy.orm import Session

from Models import Base, ChessPosition


def run_seed(db_name: str, in_file: str, tolerance: int | None, min_move: int | None, limit: int | None) -> None:
    engine = create_engine(f"sqlite:///database/{db_name}", echo=True)
    Base.metadata.create_all(engine)

    data = []
    with open(in_file, mode="r", newline="") as file:
        reader = csv.DictReader(file)
        i = 0
        for row in reader:
            has_score = row["score"] != ""
            if has_score:
                fullmove = int(row["fen"].split()[-1])

                score_in_tolerance = True if tolerance is None else abs(int(row["score"])) <= abs(tolerance)
                after_min_move = True if min_move is None else fullmove >= min_move
                if has_score and score_in_tolerance and after_min_move:
                    data.append(ChessPosition(fen=row["fen"], evaluation=row["score"]))

                    i += 1
                    if limit is not None and i >= limit:
                        break

    with Session(engine) as session:
        session.add_all(data)
        session.commit()

def main():
    db_name = input("Enter database name [main.db]: ")
    if db_name.strip() == "":
        db_name = "main.db"

    in_file = input("Enter file name [positions.csv]: ")
    if in_file.strip() == "":
        in_file = "positions.csv"

    tolerance = input("Enter max evaluation advantage [None]: ")
    if tolerance.strip() == "":
        tolerance = None
    else:
        tolerance = int(tolerance)

    min_move = input("Enter min move played [None]: ")
    if min_move.strip() == "":
        min_move = None
    else:
        min_move = int(min_move)

    limit = input("Positions to take [None]: ")
    if limit.strip() == "":
        limit = None
    else:
        limit = int(limit)

    run_seed(db_name, in_file, tolerance, min_move, limit)


if __name__ == "__main__":
    main()


