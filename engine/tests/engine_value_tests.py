import unittest

from engine.src.engine import engine

class EngineValueTests(unittest.TestCase):
    def test_result(self):
        e = engine()
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
        val = e.value(board, 'b')
        self.assertTrue(val < 0)


        board = [['Br', 'bk', 'bb', 'bq', 'BK', 'bb', 'bk', 'Br'],
                ['bp', '  ', 'bp', '  ', '  ', '  ', 'bp', 'bp'],
                ['wr', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', 'bp', 'bp', 'bp', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp'],
                ['  ', 'wk', 'wb', 'wq', 'WK', 'wb', 'wk', 'Wr']]
        val = e.value(board, 'w')
        self.assertTrue(val < 0)

        board = [['Br', 'bk', 'bb', 'bq', 'BK', 'bb', 'bk', 'Br'],
                ['bp', '  ', 'bp', '  ', '  ', '  ', 'bp', 'bp'],
                ['bp', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', 'bp', 'bp', 'bp', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp'],
                ['wr', 'wk', 'wb', 'wq', 'WK', 'wb', 'wk', 'Wr']]
        val = e.value(board, 'b')
        self.assertTrue(val < 0)
