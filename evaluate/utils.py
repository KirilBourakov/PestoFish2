import os
from pathlib import Path
from plistlib import InvalidFileException

from sqlalchemy import select
from sqlalchemy.orm import Session

from Models import Engine


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