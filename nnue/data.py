import json
from itertools import islice
from typing import Generator, Literal

import chess
import numpy as np
import numpy.typing as npt
import torch
from torch.utils.data import IterableDataset

from parse import extract_halfkp_encoding, DATA_FILE, extract_score_from_pv, soft_max_normalize, HALF_KP_ENCODING_SIZE, \
    SIMPLE_FEATURES


#TODO: reuse buffers
class LichessPositions(IterableDataset):
    def __init__(self, train_limit: int, validation_limit: int, style: Literal["train", "validation"], encoding: Literal['halfkp', 'simple']) -> None:
        self.train_limit = train_limit
        self.validation_limit = validation_limit
        self.style = style
        self.start_offset = 0
        self.encoding = encoding

    @staticmethod
    def create(train_limit: int, validation_limit: int, encoding: Literal['halfkp', 'simple']) -> tuple["LichessPositions", "LichessPositions"]:
        return LichessPositions(train_limit, validation_limit, "train", encoding), LichessPositions(train_limit, validation_limit, "validation", encoding)

    def seek(self, offset: int) -> None:
        print(f"Skipping forward {offset} positions")
        self.start_offset = offset

    def __iter__(self) -> Generator[tuple[tuple[npt.NDArray[np.int8], npt.NDArray[np.int8]], float]]:
        worker_info = torch.utils.data.get_worker_info()
        entry_size = 130

        if self.style == "validation":
            abs_start = self.train_limit * entry_size
            abs_end = abs_start + (self.validation_limit * entry_size)
        else:
            abs_start = 0
            abs_end = self.train_limit * entry_size

        with open(DATA_FILE, 'rb') as f:
            if worker_info is None:
                worker_id = 0
                num_workers = 1
            else:
                worker_id = worker_info.id
                num_workers = worker_info.num_workers
            base_worker_start = abs_start + (worker_id * entry_size)
            stride_size = num_workers * entry_size

            skip_steps = self.start_offset // num_workers
            current_pos = base_worker_start + (skip_steps * stride_size)

            f.seek(current_pos)

            while True:
                chunk = f.read(entry_size)
                if not chunk or len(chunk) < entry_size:
                    break

                our_idx = np.frombuffer(chunk, dtype=np.uint16, count=32)
                if self.encoding == 'halfkp':
                    our = np.zeros((HALF_KP_ENCODING_SIZE,), dtype=np.int8)
                else:
                    our = np.zeros((SIMPLE_FEATURES,), dtype=np.int8)
                our[our_idx[our_idx != np.iinfo(np.uint16).max]] = 1

                their_idx = np.frombuffer(chunk[64:], dtype=np.uint16, count=32)
                if self.encoding == 'halfkp':
                    their = np.zeros((HALF_KP_ENCODING_SIZE,), dtype=np.int8)
                else:
                    their = np.zeros((SIMPLE_FEATURES,), dtype=np.int8)
                their[their_idx[their_idx != np.iinfo(np.uint16).max]] = 1

                score = np.frombuffer(chunk[128:], dtype=np.int16, count=1)[0]

                yield (our, their), soft_max_normalize(score)

                if stride_size-entry_size > 0:
                    f.seek(stride_size-entry_size, 1)
                if f.tell() >= abs_end:
                    break

