import json
from io import BufferedWriter
from pathlib import Path

import chess
import numpy as np
import numpy.typing as npt
from tqdm import tqdm

SQUARES = 64
PIECES = 6
COLORS = 2
HALF_FEATURES = SQUARES * PIECES
SIMPLE_FEATURES = SQUARES * PIECES * COLORS

HALF_KP_ENCODINGS_PER_KING_POS = 640
HALF_KP_ENCODING_SIZE = HALF_KP_ENCODINGS_PER_KING_POS * 64

MAX_SCORE = 32000


def extract_simple_encoding(fen: str) -> tuple[npt.NDArray[np.int8], npt.NDArray[np.int8]]:
    """
    Converts a fen string to a simple encoding (768 neurons of binary values, each representing one possible combination of piece, square, and color)
    """
    board = chess.Board(fen)

    stm_encoding = np.zeros(SIMPLE_FEATURES, dtype=np.int8)
    nstm_encoding = np.zeros(SIMPLE_FEATURES, dtype=np.int8)

    turn = board.turn
    for square, piece in board.piece_map().items():
        p_idx = piece.piece_type - 1

        is_opponent_stm = 0 if piece.color == turn else 1
        sq_stm = square if turn == chess.WHITE else (square ^ 56)
        idx_stm = (is_opponent_stm * HALF_FEATURES) + (p_idx * SQUARES) + sq_stm
        stm_encoding[idx_stm] = 1

        is_opponent_nstm = 0 if piece.color != turn else 1
        sq_nstm = square if turn == chess.BLACK else (square ^ 56)
        idx_nstm = (is_opponent_nstm * HALF_FEATURES) + (p_idx * SQUARES) + sq_nstm
        nstm_encoding[idx_nstm] = 1

    return stm_encoding, nstm_encoding

def extract_halfkp_encoding(fen: str) -> tuple[npt.NDArray[np.int8], npt.NDArray[np.int8]]:
    """
    Converts a fen string to a half KP encoding array
    """
    def extract_for(board: chess.Board, color: chess.Color) -> npt.NDArray[np.int8]:
        encoding = np.zeros((HALF_KP_ENCODING_SIZE,), dtype=np.int8)

        k_sq = board.king(color)
        if color == chess.BLACK:
            k_sq = chess.square_mirror(k_sq)

        offset = HALF_KP_ENCODINGS_PER_KING_POS * k_sq

        for square, piece in board.piece_map().items():
            if piece.piece_type == chess.KING:
                continue

            p_sq = square if color == chess.WHITE else chess.square_mirror(square)
            piece_type_idx = piece.piece_type - 1  # pawn = 0 ... queen = 4
            color_idx = 0 if piece.color == color else 5

            colored_piece_idx = color_idx + piece_type_idx  # enemy pawn becomes 5

            piece_index = (colored_piece_idx * 64) + p_sq  # find area of color_piece, and then square in it
            encoding[offset + piece_index] = 1

        return encoding

    converted_fen = chess.Board(fen)
    return extract_for(converted_fen, converted_fen.turn), extract_for(converted_fen, not converted_fen.turn)

def extract_score_from_pv(first_pv: dict) -> float:
    """
    Extracts the score from the first pv in lichess evaluation database
    """
    if "cp" in first_pv:
        position_score = clamp(first_pv["cp"], -MAX_SCORE, MAX_SCORE)
    elif "mate" in first_pv:
        position_score = MAX_SCORE if first_pv["mate"] > 0 else -MAX_SCORE
    else:
        raise ValueError(f"No evaluation or mate in {first_pv}")
    return position_score

def soft_max_normalize(position_score: float) -> float:
    """Soft max between -1 and 1"""
    return 2 / (1 + 10 ** (-position_score / 400)) - 1

def clamp(value, minimum, maximum):
    return max(minimum, min(value, maximum))

def parse_all_lichess(input_file: str | Path, output_bin: str | Path, extract_halfkp=False):
    """
    Parses evaluated lichess positions into bin. Each entry in the bin is 130 bytes.
    The first 64 bytes is indexes that are active from our perspective, the next 64 are active from the enemy perspective.
    np.iinfo(np.uint16).max is used for padding an indicates an empty value.
    The last 2 are the position evaluation.
    """
    dropped = 0
    parsed = 0
    with open(input_file, 'r', encoding='utf-8') as f_in, open(output_bin, 'wb') as f_out:
        for line in tqdm(f_in):
            data = json.loads(line)

            fen = data["fen"]
            if extract_halfkp:
                our_encoding, their_encoding = extract_halfkp_encoding(fen)
            else:
                our_encoding, their_encoding = extract_simple_encoding(fen)

            first_pv = data["evals"][0]["pvs"][0]
            score = np.array(extract_score_from_pv(first_pv), dtype=np.int16)


            full_size = 64
            side_size = full_size // 2
            active = np.full(full_size, np.iinfo(np.uint16).max, dtype=np.uint16)
            our_active_idx = np.argwhere(our_encoding == 1).flatten()
            their_active_idx = np.argwhere(their_encoding == 1).flatten()

            # Drop positions that have
            if len(their_active_idx) <= side_size and len(our_active_idx) <= side_size:
                active[:len(our_active_idx)] = our_active_idx
                active[side_size:side_size+len(their_active_idx)] = their_active_idx
                f_out.write(active.tobytes())
                f_out.write(score.tobytes())
                parsed += 1
            else:
                dropped += 1

    print(f"Completed parsing of {parsed} positions. Dropped {dropped} positions.")

if __name__ == "__main__":
    parse_all_lichess("data/lichess_db_eval.jsonl", "data/simple-329082547.bin")