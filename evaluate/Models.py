from sqlalchemy.orm import DeclarativeBase, Mapped, mapped_column, relationship
from sqlalchemy import String, Integer, Column, ForeignKey, Enum

class Base(DeclarativeBase):
    pass

class ChessPosition(Base):
    __tablename__ = "ChessPosition"

    id: Mapped[int] = mapped_column(Integer, primary_key=True)
    fen: Mapped[str] = mapped_column(String)
    rating: Mapped[int] = mapped_column(Integer)

    matches: Mapped[list["Match"]] = relationship(back_populates="position")

class Engine(Base):
    __tablename__ = "Engine"

    id: Mapped[int] = mapped_column(Integer, primary_key=True)
    name: Mapped[str] = mapped_column(String)

    white_matches: Mapped[list["Tournament"]] = relationship(
        back_populates="white_engine",
        foreign_keys="Tournament.white_engine_id",
    )
    black_matches: Mapped[list["Tournament"]] = relationship(
        back_populates="black_engine",
        foreign_keys="Tournament.black_engine_id",
    )

class Tournament(Base):
    __tablename__ = "Tournament"

    id: Mapped[int] = mapped_column(Integer, primary_key=True)
    white_engine_id: Mapped[int] = mapped_column(ForeignKey("engine.id"))
    black_engine_id: Mapped[int] = mapped_column(ForeignKey("engine.id"))

    white_engine: Mapped["Engine"] = relationship(
        foreign_keys=[white_engine_id],
        back_populates="white_matches",
    )
    black_engine: Mapped["Engine"] = relationship(
        foreign_keys=[black_engine_id],
        back_populates="black_matches",
    )

    matches: Mapped[list["Match"]] = relationship(back_populates="tournament")

class Match(Base):
    __tablename__ = "Match"

    id: Mapped[int] = mapped_column(Integer, primary_key=True)
    tournament_id: Mapped[int] = mapped_column(ForeignKey("Tournament.id"))
    position_id: Mapped[int] = mapped_column(ForeignKey("ChessPosition.id"))
    result: Mapped[str] = mapped_column(String)

    tournament: Mapped["Tournament"] = relationship(back_populates="matches")
    position: Mapped["ChessPosition"] = relationship(back_populates="matches")