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