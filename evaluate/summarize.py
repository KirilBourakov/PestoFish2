from sqlalchemy import create_engine, select, or_, and_
from sqlalchemy.orm import Session

from Models import Tournament
from utils import validate_engine, get_engine_model


def main():
    db_name = input("Enter database name [main.db]: ")
    if db_name.strip() == "":
        db_name = "main.db"
    db_engine = create_engine(f"sqlite:///database/{db_name}", echo=True)

    engine1 = input("Engine 1: ")
    engine2 = input("Engine 2: ")

    engine1_name, engine1_path = validate_engine(engine1)
    engine2_name, engine2_path = validate_engine(engine2)

    results = {
        engine1_name: 0,
        engine2_name: 0,
        "draw": 0
    }
    tournament_count = 0
    with Session(db_engine) as session:
        engine1_obj = get_engine_model(session, engine1_name)
        engine2_obj = get_engine_model(session, engine2_name)

        stmt = select(Tournament).where(
            or_(
                and_(
                    Tournament.white_engine_id == engine1_obj.id,
                    Tournament.black_engine_id == engine2_obj.id
                ),
                and_(
                    Tournament.white_engine_id == engine2_obj.id,
                    Tournament.black_engine_id == engine1_obj.id
                )
            )
        )

        tournaments = session.execute(stmt).scalars().all()
        for tournament in tournaments:
            if tournament.white_engine_id == engine1_obj.id:
                white_engine = engine1_obj
                black_engine = engine2_obj
            else:
                white_engine = engine2_obj
                black_engine = engine1_obj

            tournament_count += 1
            for match in tournament.matches:
                if match.result == "1-0":
                    results[white_engine.name] += 1
                elif match.result == "0-1":
                    results[black_engine.name] += 1
                else:
                    results['draw'] += 1

    print(f"Found {tournaments} tournaments")
    print(f"Results: {results}")

if __name__ == "__main__":
    main()