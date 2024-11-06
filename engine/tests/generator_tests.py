import unittest

from engine.src.generator.generator import Generator

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
        s = generator.is_legal_move(board3, (4,0), (4,0), (3,1))
        self.assertFalse(s)

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