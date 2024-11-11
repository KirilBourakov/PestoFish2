import unittest

from engine.src.engine import engine
from engine.src.helpers.board_analysis import find_king

class EngineGetBestMoveTest(unittest.TestCase):
    def test_get_best_move(self):
        e = engine()
        e.fifty_move_rule_counter = 0
        e.move_counter = 0
        board1 = [['  ', '  ', '  ', '  ', 'BK', '  ', '  ', '  '],
                ['wq', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wq', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', 'WK', '  ', '  ', '  ']]
        e.board = board1
        
        print(e.get_best_move())
        