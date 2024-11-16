import unittest

from engine.src.generator.moves import Moves

class MovesTests(unittest.TestCase):
    def get_board(self, pieces: list[tuple[str, tuple[int,int]]]):
        board = []
        for i in range(8):
            board.append(['  ']*8)

        for piece in pieces:
            board[piece[1][1]][piece[1][0]] = piece[0]
        return board
    
    def test_get_all_moves(self):
        board1 = self.get_board([('wp', (0,3)), ('bp', (0,2))])
        move_gen = Moves()
        w_moves = move_gen.get_all_moves(board1, (0,3))
        b_moves = move_gen.get_all_moves(board1, (0,2))

        self.assertEqual(len(w_moves), 0)
        self.assertEqual(len(b_moves), 0)

        board2 =    [['Br', 'bk', 'bb', '  ', 'BK', 'bb', 'bk', 'Br'],
                    ['bp', 'bp', 'bp', 'bp', '  ', 'bp', 'bp', 'bp'],
                    ['wp', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                    ['  ', '  ', '  ', '  ', 'bp', '  ', '  ', '  '],
                    ['wp', '  ', '  ', '  ', '  ', '  ', '  ', 'bq'],
                    ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                    ['  ', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp'],
                    ['Wr', 'wk', 'wb', 'wq', 'WK', 'wb', 'wk', 'Wr']]
        
        # queen
        moves = move_gen.get_all_moves(board2, (7,4))
        self.assertEqual(len(moves), 17)

        # pawns
        moves = move_gen.get_all_moves(board2, (7,6))
        self.assertEqual(len(moves), 1)

        moves = move_gen.get_all_moves(board2, (1,0))
        self.assertEqual(len(moves), 2)

        moves = move_gen.get_all_moves(board2, (1,1))
        self.assertEqual(len(moves), 3)

        # bishop, knight
        moves = move_gen.get_all_moves(board2, (0,7))
        self.assertEqual(len(moves), 2)

        moves = move_gen.get_all_moves(board2, (0,1))
        self.assertEqual(len(moves), 0)

        # king
        moves = move_gen.get_all_moves(board2, (4,0))
        self.assertEqual(len(moves), 2)

        board2 = [['Br', '  ', '  ', 'bq', 'BK', 'bb', 'bk', 'Br'],
                ['bp', 'bp', 'bp', '  ', '  ', 'bp', 'bp', 'bp'],
                ['bk', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', 'bp', 'bp', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', 'bb', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wp', 'wp', 'wp', '  ', 'wp', 'wp', 'wp'],
                ['Wr', 'wk', 'wb', 'wq', 'WK', 'wb', 'wk', 'Wr']]
        moves = move_gen.get_simple_moves(board2, (5,7))
        self.assertEqual(len(moves), 5)

    def test_get_simple_moves(self):
        moves_gen = Moves()
        board1 =    [['Br', 'bk', 'bb', '  ', 'BK', 'bb', 'bk', 'Br'],
                    ['bp', 'bp', 'bp', 'bp', '  ', 'bp', 'bp', 'bp'],
                    ['wp', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                    ['  ', '  ', '  ', '  ', 'bp', '  ', '  ', '  '],
                    ['wp', '  ', '  ', '  ', '  ', '  ', '  ', 'bq'],
                    ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                    ['  ', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp', 'wp'],
                    ['Wr', 'wk', 'wb', 'wq', 'WK', 'wb', 'wk', 'Wr']]
        moves = moves_gen.get_simple_moves(board1, (2,7))
        self.assertEqual(len(moves), 0)

        moves = moves_gen.get_simple_moves(board1, (7,4))
        self.assertEqual(len(moves), 17)

        moves = moves_gen.get_simple_moves(board1, (7,6))
        self.assertEqual(len(moves), 1)

        moves = moves_gen.get_simple_moves(board1, (0,7))
        self.assertEqual(len(moves), 2)

        moves = moves_gen.get_simple_moves(board1, (0,1))
        self.assertEqual(len(moves), 0)

        moves = moves_gen.get_simple_moves(board1, (1,0))
        self.assertEqual(len(moves), 2)

        board2 = [['Br', '  ', '  ', 'bq', 'BK', 'bb', 'bk', 'Br'],
                ['bp', 'bp', 'bp', '  ', '  ', 'bp', 'bp', 'bp'],
                ['bk', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', 'bp', 'bp', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', 'bb', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wp', 'wp', 'wp', '  ', 'wp', 'wp', 'wp'],
                ['Wr', 'wk', 'wb', 'wq', 'WK', 'wb', 'wk', 'Wr']]
        moves = moves_gen.get_simple_moves(board2, (5,7))
        self.assertEqual(len(moves), 5)
        
    def test_get_complex_moves(self):
        moves_gen = Moves()
        board1 =   [['Br', '  ', '  ', '  ', 'BK', 'bb', 'bk', 'Br'],
                    ['bp', 'bp', 'bp', 'bp', '  ', 'bp', 'bp', 'bp'],
                    ['  ', 'wp', '  ', '  ', '  ', '  ', '  ', '  '],
                    ['  ', '  ', '  ', '  ', 'bp', '  ', '  ', '  '],
                    ['  ', '  ', '  ', '  ', '  ', 'wp', '  ', 'bq'],
                    ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                    ['wp', '  ', 'wp', 'wp', 'wp', '  ', 'wp', 'wp'],
                    ['Wr', '  ', '  ', '  ', 'WK', '  ', '  ', 'Wr']]
        moves = moves_gen.get_complex_moves(board1, (4,7))
        self.assertEqual(len(moves), 2)

        moves = moves_gen.get_complex_moves(board1, (4,3))
        self.assertEqual(len(moves), 1)

        moves = moves_gen.get_complex_moves(board1, (0,1))
        self.assertEqual(len(moves), 2)

        board2 =   [['Br', '  ', '  ', '  ', 'BK', 'bb', 'bk', 'Br'],
                    ['bp', 'bp', 'bp', 'bp', '  ', 'bp', 'bp', 'bp'],
                    ['  ', 'wp', '  ', '  ', '  ', '  ', '  ', '  '],
                    ['  ', '  ', '  ', '  ', 'bp', '  ', '  ', '  '],
                    ['  ', '  ', '  ', '  ', '  ', 'wp', '  ', 'bq'],
                    ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                    ['wp', '  ', 'wp', 'wp', 'wK', '  ', 'wp', 'wp'],
                    ['Wr', '  ', '  ', '  ', '  ', '  ', '  ', 'Wr']]
        moves = moves_gen.get_complex_moves(board2, (4,6))
        self.assertEqual(len(moves), 0)

        board3 = [['  ', '  ', 'bb', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', 'bp', '  ', '  ', 'bK', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', 'wk', '  ', 'bp'],
                ['bp', 'br', '  ', '  ', '  ', '  ', '  ', 'wp'],
                ['  ', '  ', '  ', 'wp', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', 'wp', 'wp', '  ', '  ', 'wp', 'wp', 'wr'],
                ['Wr', '  ', '  ', '  ', 'WK', '  ', '  ', '  ']]
        moves = moves_gen.get_complex_moves(board3, (4,7))
        self.assertEqual(len(moves), 1)
        board4 = [['  ', '  ', 'bb', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', 'bp', '  ', '  ', 'bK', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', 'wk', '  ', 'bp'],
                ['bp', 'br', '  ', '  ', '  ', '  ', '  ', 'wp'],
                ['  ', '  ', '  ', 'wp', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['wr', 'wp', 'wp', '  ', '  ', 'wp', 'wp', 'wr'],
                ['  ', '  ', '  ', '  ', 'WK', '  ', '  ', '  ']]
        moves = moves_gen.get_complex_moves(board4, (4,7))
        self.assertEqual(len(moves), 0)

        board5 = [['  ', '  ', '  ', '  ', '  ', '  ', '  ', '  '],
                ['  ', '  ', '  ', '  ', '  ', 'bp', '  ', '  '],
                ['  ', 'bp', '  ', '  ', '  ', '  ', '  ', '  '],
                ['bp', '  ', '  ', 'br', 'bK', '  ', '  ', 'bp'],
                ['bp', '  ', 'wp', '  ', 'bp', 'wp', '  ', 'wp'],
                ['wp', '  ', '  ', '  ', '  ', 'we', '  ', '  '],
                ['  ', 'wp', 'wK', '  ', '  ', '  ', 'wp', '  '],
                ['  ', '  ', '  ', '  ', '  ', 'wb', '  ', 'wr']]
        moves = moves_gen.get_complex_moves(board5, (4,4))
        self.assertEqual(len(moves), 1)
        
        