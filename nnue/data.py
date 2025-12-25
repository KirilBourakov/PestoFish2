import json
from itertools import islice
from typing import Generator

import chess
import numpy as np
import numpy.typing as npt
import torch
from torch.utils.data import IterableDataset

DATA_FILE = "data/lichess_db_eval.jsonl"

ENCODINGS_PER_KING_POS = 640
ENCODING_SIZE = ENCODINGS_PER_KING_POS * 64
MAX_SCORE = 32000

HalfKP = tuple[npt.NDArray[np.int8], npt.NDArray[np.int8]]

#TODO: reuse buffers
class Positions(IterableDataset):
    def __init__(self, limit: int | None = None):
        self.limit = limit

    def __iter__(self) -> Generator[tuple[HalfKP, float]]:
        worker_info = torch.utils.data.get_worker_info()

        with open(DATA_FILE, 'r', encoding='utf-8') as f:
            if worker_info is None:
                iter_start = 0
                iter_step = 1
            else:
                iter_start = worker_info.id
                iter_step = worker_info.num_workers

            for line in islice(f, iter_start, self.limit, iter_step):
                data = json.loads(line)

                fen = data["fen"]
                encoding = self._extract_encoding(fen)

                first_pv = data["evals"][0]["pvs"][0]
                score = self._extract_score_from_pv(first_pv)

                yield encoding, score

    @staticmethod
    def _extract_encoding(fen: str) -> HalfKP:
        def extract_for(board: chess.Board, color: chess.Color) -> npt.NDArray[np.int8]:
            encoding = np.zeros((ENCODING_SIZE, ), dtype=np.int8)

            k_sq = board.king(color)
            if color == chess.BLACK:
                k_sq = chess.square_mirror(k_sq)

            offset = ENCODINGS_PER_KING_POS * k_sq

            SQUARE_COUNT = 64
            for square, piece in board.piece_map().items():
                if piece.piece_type == chess.KING:
                    continue

                p_sq = square if color == chess.WHITE else chess.square_mirror(square)
                piece_type_idx = piece.piece_type - 1  # pawn = 0 ... queen = 4
                color_idx = 0 if piece.color == color else 5

                colored_piece_idx = color_idx + piece_type_idx # enemy pawn becomes 5

                piece_index = (colored_piece_idx * 64) + p_sq # find area of color_piece, and then square in it
                encoding[offset + piece_index] = 1

            return encoding

        converted_fen = chess.Board(fen)
        return extract_for(converted_fen, converted_fen.turn), extract_for(converted_fen, not converted_fen.turn)

    @staticmethod
    def _extract_score_from_pv(first_pv: dict) -> float:
        if "cp" in first_pv:
            position_score = clamp(first_pv["cp"], -MAX_SCORE, MAX_SCORE)
        elif "mate" in first_pv:
            position_score = MAX_SCORE if first_pv["mate"] > 0 else -MAX_SCORE
        else:
            raise ValueError(f"No evaluation or mate in {first_pv}")

        return 2 / (1 + 10 ** (-position_score / 400)) - 1

def clamp(value, minimum, maximum):
    return max(minimum, min(value, maximum))