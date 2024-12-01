import unittest

from engine.src.generator.generator import Generator
from engine.src.constants.static import SHORT_CASTLE, LONG_CASTLE

class GeneratorTests(unittest.TestCase):
    def test_is_legal_move(self):
        generator = Generator()
        board1 = [['Br', 'bk', 'bb', '  ', 'BK', 'bb', 'bk', 'Br'],
                ['bp', 'bp', 'bp', 'bp', '  ', 'bp', 'bp', 'bp'],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', 'bp', '  ', '  ', '  '],
                ['wp', '  ', '  ', '  ', '  ', '  ', '  ', 'bq'],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp'],
                ['Wr', 'wk', 'wb', 'wq', 'WK', 'wb', 'wk', 'Wr']]
        
        s = generator.is_legal_move(board1, (4,7), (5,6), (5,4))
        self.assertFalse(s)


        board2 = [['Br', '  ', 'bb', 'bq', 'BK', 'bb', 'bk', 'Br'],
                ['bp', 'bp', 'bp', 'bp', 'bp', 'bp', 'bp', 'bp'],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['wp', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wp', 'bk', 'wp', 'wp', 'wp', 'wp', 'wp'],
                ['Wr', 'wk', 'wb', 'wq', 'WK', 'wb', 'wk', 'Wr']]
        s = generator.is_legal_move(board2, (4,7), (0,4), (0,3))
        self.assertFalse(s)

        board3 = [['Br', '  ', '  ', 'bq', 'BK', 'bb', 'bk', 'Br'],
                ['bp', 'bp', 'bp', 'bp', '  ', 'bp', 'bp', 'bp'],
                ['bk', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wb', '  ', '  ', 'bp', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', 'bb', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wp', 'wp', 'wp', '  ', 'wp', 'wp', 'wp'],
                ['Wr', 'wk', 'wb', 'wq', 'WK', '  ', 'wk', 'Wr']]
        s = generator.is_legal_move(board3, (4,0), (3,1), (3,3))
        self.assertFalse(s)
        s = generator.is_legal_move(board3, (4,0), (3,1), (3,2))
        self.assertFalse(s)

        board4 = [['Br', '  ', '  ', 'bq', 'BK', 'bb', 'bk', 'Br'],
                ['bp', 'bp', 'bp', '  ', '  ', 'bp', 'bp', 'bp'],
                ['bk', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wb', '  ', 'bp', 'bp', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', 'bb', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wp', 'wp', 'wp', '  ', 'wp', 'wp', 'wp'],
                ['Wr', 'wk', 'wb', 'wq', 'WK', '  ', 'wk', 'Wr']]
        s = generator.is_legal_move(board4, (4,0), (4,0), (3,1))
        self.assertFalse(s)

        board3 = [['Br', 'bk', 'bb', 'bq', 'BK', 'bb', 'bk', 'Br'],
                ['bp', '  ', 'bp', 'bp', 'bp', 'bp', '  ', 'bp'],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['wp', 'bp', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', 'br', '  ', 'bp', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wp', '  ', '  ', '  ', '  ', '  ', '  '],
                ['Wr', '  ', '  ', '  ', 'WK', '  ', '  ', 'Wr']]
        s = generator.is_legal_move(board2, (4,7), (4,7), (6,7, SHORT_CASTLE))
        self.assertFalse(s)

        board4 = [['Br', '  ', '  ', '  ', 'BK', 'bb', 'bk', 'Br'],
                ['bp', '  ', 'bp', '  ', 'bp', 'bp', '  ', 'bp'],
                ['  ', '  ', 'wq', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', 'WK', '  ', '  ', '  ']]
        s = generator.is_legal_move(board4, (4,0), (4,0), (2,0, LONG_CASTLE))
        self.assertFalse(s)

        board5 =   [['Br', '  ', '  ', '  ', 'BK', 'bb', 'bk', 'Br'],
                    ['bp', 'bp', 'bp', 'bp', '  ', 'bp', 'bp', 'bp'],
                    ['  ', 'wp', '  ', '  ', '  ', '  ', '  ', '  '],
                    ['  ', '  ', '  ', '  ', 'bp', '  ', 'bq', '  '],
                    ['  ', '  ', '  ', '  ', '  ', 'wp', '  ', '  '],
                    ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                    ['wp', '  ', 'wp', 'wp', 'wp', '  ', 'wp', 'wp'],
                    ['Wr', '  ', '  ', '  ', 'WK', '  ', '  ', 'Wr']]
        s = generator.is_legal_move(board5, (4,7), (4,7), (6,7, SHORT_CASTLE))
        self.assertTrue(s)

        board6 = [['Br', 'bk', '  ', 'bq', 'BK', '  ', 'bk', 'Br'],
                ['bp', 'bp', '  ', '  ', '  ', 'bp', 'bp', 'bp'],
                ['  ', '  ', 'bp', 'bb', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', 'bp', 'bp', '  ', 'wk', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', 'bb', '  '],
                ['  ', '  ', 'wk', 'wp', '  ', '  ', '  ', '  '],
                ['wp', 'wp', 'wp', '  ', 'wp', 'wp', 'wp', 'wp'],
                ['Wr', '  ', 'wb', 'wq', 'WK', 'wb', '  ', 'Wr']]
        s = generator.is_legal_move(board6, (4,0), (3,0), (6,3,''))

    def test_get_moves(self):
        generator = Generator()
        board1 = [['Br', '  ', 'bb', 'bq', 'BK', 'bb', 'bk', 'Br'],
                ['bp', 'bp', 'bp', 'bp', 'bp', 'bp', 'bp', 'bp'],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['wp', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wp', 'bk', 'wp', 'wp', 'wp', 'wp', 'wp'],
                ['Wr', 'wk', 'wb', 'wq', 'WK', 'wb', 'wk', 'Wr']]
        s = generator.get_moves(board1, (4,7))
        self.assertEqual(len(s), 1)

        board2 = [['Br', '  ', 'bb', 'bq', 'BK', 'bb', 'bk', 'Br'],
                ['bp', 'bp', 'bp', 'bp', 'bp', 'bp', 'bp', 'bp'],
                ['wp', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wp', 'bk', 'wp', 'wp', 'wp', 'wp', 'wp'],
                ['Wr', 'wk', 'wb', 'wq', 'WK', 'wb', 'wk', 'Wr']]
        s = generator.get_moves(board2, (4,0))
        self.assertTrue({'original': (1, 1), 'new': (0, 2), 'rating': 3, 'promotion': ''} in s)

        board3 = [['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', 'bp', '  ', '  '],
                ['  ', 'bp', '  ', '  ', '  ', '  ', '  ', '  '],
                ['bp', '  ', '  ', 'br', 'bK', '  ', '  ', 'bp'],
                ['bp', '  ', 'wp', '  ', 'bp', 'wp', '  ', 'wp'],
                ['wp', '  ', '  ', '  ', '  ', 'we', '  ', '  '],
                ['  ', 'wp', 'wK', '  ', '  ', '  ', 'wp', '  '],
                ['  ', '  ', '  ', '  ', '  ', 'wb', '  ', 'wr']]
        s = generator.get_moves(board3, (4,3))
        self.assertEqual(len(s), 6)
        