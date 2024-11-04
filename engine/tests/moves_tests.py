import unittest

from engine.src.generator.moves import Moves

class MovesTests(unittest.TestCase):
    def get_board(self, pieces: list[tuple[str, tuple[int,int]]]):
        board = []
        for i in range(8):
            board.append([' ']*8)

        for piece in pieces:
            board[piece[1][1]][piece[1][0]] = piece[0]
        return board
    
    def test_get_all_moves(self):
        board1 = self.get_board([('wp', (0,3)), ('bp', (0,2))])
        moves = Moves()
        w_moves = moves.get_simple_moves(board1, (0,3))
        b_moves = moves.get_simple_moves(board1, (0,2))

        self.assertEqual(len(w_moves), 0)
        self.assertEqual(len(b_moves), 0)

    def test_get_simple_moves(self):
        moves = Moves()
        board1 =    [['Br', 'bk', 'bb', '  ', 'BK', 'bb', 'bk', 'Br'],
                    ['bp', 'bp', 'bp', 'bp', '  ', 'bp', 'bp', 'bp'],
                    ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                    ['  ', '  ', '  ', '  ', 'bp', '  ', '  ', '  '],
                    ['wp', '  ', '  ', '  ', '  ', '  ', '  ', 'bq'],
                    ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                    ['  ', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp'],
                    ['Wr', 'wk', 'wb', 'wq', 'WK', 'wb', 'wk', 'Wr']]
        moves = moves.get_simple_moves(board1, (4,7))
        self.assertEqual(len(moves), 15)
        print(moves)