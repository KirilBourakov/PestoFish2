import unittest

from engine.src.engine import Engine

class EngineValueTests(unittest.TestCase):
    def test_result(self):
        e = Engine()
        e.fifty_move_rule_counter = 0
        e.move_counter = 0
        board = [['Br', 'bk', '  ', 'bq', 'BK', 'bb', 'bk', 'Br'],
                ['bp', '  ', 'bp', '  ', '  ', '  ', 'bp', 'bp'],
                ['bb', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', 'bp', 'bp', 'bp', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp'],
                ['  ', 'wk', 'wb', 'wq', 'WK', 'wb', 'wk', 'Wr']]
        val = e.value(board, 'b')[0]
        self.assertTrue(val < 0)


        board = [['Br', 'bk', 'bb', 'bq', 'BK', 'bb', 'bk', 'Br'],
                ['bp', '  ', 'bp', '  ', '  ', '  ', 'bp', 'bp'],
                ['wr', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', 'bp', 'bp', 'bp', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp'],
                ['  ', 'wk', 'wb', 'wq', 'WK', 'wb', 'wk', 'Wr']]
        val = e.value(board, 'w')[0]
        self.assertTrue(val < 0)

        board = [['Br', 'bk', 'bb', 'bq', 'BK', 'bb', 'bk', 'Br'],
                ['bp', '  ', 'bp', '  ', '  ', '  ', 'bp', 'bp'],
                ['bp', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', 'bp', 'bp', 'bp', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp'],
                ['wr', 'wk', 'wb', 'wq', 'WK', 'wb', 'wk', 'Wr']]
        val = e.value(board, 'b')[0]
        self.assertTrue(val < 0)

        board = [['  ', 'wq', '  ', '  ', 'BK', '  ', '  ', '  '],
                ['wq', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', 'WK', '  ', '  ', '  ']]
        val = e.value(board, 'w')[0]
        self.assertEqual(val, float('inf'))

        board = [['  ', '  ', '  ', '  ', 'BK', '  ', '  ', '  '],
                ['wq', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wq', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', 'WK', '  ', '  ', '  ']]
        val = e.value(board, 'w', max_depth=1)[0]
        self.assertNotEqual(val, float('inf'))