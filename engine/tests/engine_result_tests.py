import unittest

from engine.src.engine import engine

class EngineResultTests(unittest.TestCase):
    def test_result(self):
        e = engine()
        board1 = [['Br', 'bk', 'bb', '  ', 'BK', 'bb', 'bk', 'Br'],
                ['bp', 'bp', 'bp', 'bp', '  ', 'bp', 'bp', 'bp'],
                ['wp', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', 'bp', '  ', '  ', '  '],
                ['  ', '  ', 'bq', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp'],
                ['Wr', 'wk', 'wb', 'wq', 'WK', 'wb', 'wk', 'Wr']]
        r = e.result(board1, {'original': (1, 1), 'new': (0, 2), 'rating': 3, 'promotion': ''})
        self.assertEqual('bp', r[2][0])

        board2 = [['  ', '  ', '  ', '  ', 'BK', '  ', '  ', '  '],
                ['wq', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wq', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', 'WK', '  ', '  ', '  ']]
        board3 = [['  ', 'wq', '  ', '  ', 'BK', '  ', '  ', '  '],
                ['wq', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', 'WK', '  ', '  ', '  ']]
        r = e.result(board2, {'original': (1, 2), 'new': (1, 0), 'rating': 1, 'promotion': ''})
        self.assertEqual(r, board3)

        r = e.result(board2, {'original': (4, 7), 'new': (4, 6), 'rating': 1, 'promotion': ''})
        self.assertEqual(r[6][4], 'wK')

    def test_double_move(self):
        e = engine()
        original = [['Br', 'bk', 'bb', 'bq', 'BK', 'bb', 'bk', 'Br'],
                ['bp', 'bp', 'bp', 'bp', 'bp', 'bp', 'bp', 'bp'],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['wp', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp'],
                ['Wr', 'wk', 'wb', 'wq', 'WK', 'wb', 'wk', 'Wr']]
        result = e.result(original, {'original': (1, 1), 'new': (1, 3), 'rating': 3, 'promotion': ''})
        self.assertEqual('  ', result[1][1])
        self.assertEqual('be', result[2][1])
        self.assertEqual('bp', result[3][1])

        original = [['Br', 'bk', 'bb', 'bq', 'BK', 'bb', 'bk', 'Br'],
                ['bp', '  ', 'bp', 'bp', 'bp', 'bp', 'bp', 'bp'],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['wp', 'bp', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp'],
                ['Wr', 'wk', 'wb', 'wq', 'WK', 'wb', 'wk', 'Wr']]
        result = e.result(original, {'original': (7, 6), 'new': (7, 4), 'rating': 1, 'promotion': ''})
        self.assertEqual('  ', result[6][7])
        self.assertEqual('we', result[5][7])
        self.assertEqual('wp', result[4][7])

    def test_en_passent(self):
        e = engine()
        original = [['Br', 'bk', 'bb', 'bq', 'BK', 'bb', 'bk', 'Br'],
                ['bp', '  ', 'bp', 'bp', 'bp', 'bp', 'bp', 'bp'],
                ['  ', 'be', '  ', '  ', '  ', '  ', '  ', '  '],
                ['wp', 'bp', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp'],
                ['Wr', 'wk', 'wb', 'wq', 'WK', 'wb', 'wk', 'Wr']]
        result = e.result(original, {'original': (0, 3), 'new': (1, 2), 'rating': 1, 'promotion': ''})
        self.assertEqual('  ', result[1][1])
        self.assertEqual('wp', result[2][1])
        self.assertEqual('  ', result[3][1])

        original = [['Br', 'bk', 'bb', 'bq', 'BK', 'bb', 'bk', 'Br'],
                ['bp', '  ', 'bp', 'bp', 'bp', 'bp', '  ', 'bp'],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['wp', 'bp', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', 'bp', 'wp'],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', 'we'],
                ['  ', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp', '  '],
                ['Wr', 'wk', 'wb', 'wq', 'WK', 'wb', 'wk', 'Wr']]
        result = e.result(original, {'original': (6, 4), 'new': (7, 5), 'rating': 1, 'promotion': ''})
        self.assertEqual('  ', result[6][7])
        self.assertEqual('bp', result[5][7])
        self.assertEqual('  ', result[4][7])

    def test_castle_white(self):
        e = engine()
        original = [['Br', 'bk', 'bb', 'bq', 'BK', 'bb', 'bk', 'Br'],
                ['bp', '  ', 'bp', 'bp', 'bp', 'bp', '  ', 'bp'],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['wp', 'bp', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', 'bp', 'wp'],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', 'we'],
                ['  ', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp', '  '],
                ['Wr', '  ', '  ', '  ', 'WK', '  ', '  ', 'Wr']]
        result = e.result(original, {'original': (4, 7), 'new': (6, 7), 'rating': 1, 'promotion': ''})
        self.assertEqual('  ', result[7][7])
        self.assertEqual('wK', result[7][6])
        self.assertEqual('wr', result[7][5])

        result = e.result(original, {'original': (4, 7), 'new': (2, 7), 'rating': 1, 'promotion': ''})
        self.assertEqual('  ', result[7][0])
        self.assertEqual('  ', result[7][1])
        self.assertEqual('wK', result[7][2])
        self.assertEqual('wr', result[7][3])

    def test_castle_black(self):
        e = engine()
        original = [['Br', '  ', '  ', '  ', 'BK', '  ', '  ', 'Br'],
                ['bp', '  ', 'bp', 'bp', 'bp', 'bp', '  ', 'bp'],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['wp', 'bp', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', 'bp', 'wp'],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', 'we'],
                ['  ', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp', '  '],
                ['Wr', '  ', '  ', '  ', 'WK', '  ', '  ', 'Wr']]
        result = e.result(original, {'original': (4, 0), 'new': (6, 0), 'rating': 1, 'promotion': ''})
        self.assertEqual('  ', result[0][7])
        self.assertEqual('bK', result[0][6])
        self.assertEqual('br', result[0][5])

        result = e.result(original, {'original': (4, 0), 'new': (2, 0), 'rating': 1, 'promotion': ''})
        self.assertEqual('  ', result[0][0])
        self.assertEqual('  ', result[0][1])
        self.assertEqual('bK', result[0][2])
        self.assertEqual('br', result[0][3])
