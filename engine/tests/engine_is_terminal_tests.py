import unittest

from engine.src.Engine import Engine

class EngineIsTerminalTest(unittest.TestCase):
    def test_get_best_move(self):
        e = Engine()
        e.fifty_move_rule_counter = 0
        e.move_counter = 0
        board1 = [['  ', 'wq', '  ', '  ', 'BK', '  ', '  ', '  '],
                ['wq', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', 'WK', '  ', '  ', '  ']]
        self.assertEqual(e.is_termainal(board1), True)

        board1 = [['  ', '  ', '  ', '  ', 'BK', '  ', '  ', '  '],
                ['wq', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wq', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', 'WK', '  ', '  ', '  ']]
        self.assertEqual(e.is_termainal(board1), False)