from square_analysis import get_color, get_type, has_moved, is_empty_squares, is_empty
from constants import KING, PAWN, BLACK, WHITE, SHORT_CASTLE, LONG_CASTLE, DOUBLE_MOVE, PAWN_CAPTURE
from typing import TypedDict

class Moves():
    def __init__(self) -> None:
        self.simple_moves: dict[str, list[tuple[int, int]]] = self.gen_simple_moves()


    def gen_simple_moves(self) ->  dict[str, list[tuple[int, int]]]:
        '''gets all the 'simple' moves (moves that are always avalible, unless they're blocked)

        returns a dict, who's key is the piece character. Pawns require color due to them only moving one direction.
        '''
        moves: dict[str, list[tuple[int, int]]] = {}
        
        # king moves (non castle)
        moves['K'] = [(-1, 1), (0, 1), (1,1),(-1,0), (1,0),(-1,-1), (0,-1), (1,-1)]

        # knight moves
        moves['k'] = [(+2, -1), (+2, +1),(-2, -1), (-2, +1),(-1, +2), (+1, +2),(-1, -2), (+1, -2)]

        # rook moves
        rook_moves: list[tuple[int, int]] = []
        for i in range(1, 16):
            rook_moves.append((i-8, 0))
            rook_moves.append((0, i-8))
        rook_moves.remove((0,0))
        moves['r'] = rook_moves

        # bishop moves
        bishop_moves: list[tuple[int, int]] = []
        for i in range(1, 16):
            bishop_moves.append((i-8, i-8))
            bishop_moves.append((-(i-8), i-8))
        bishop_moves.remove((0,0))
        bishop_moves.remove((0,0))
        moves['b'] = bishop_moves

        queen_moves: list[tuple[int, int]] = rook_moves.copy()
        queen_moves.extend(bishop_moves)
        moves['q'] = queen_moves

        # pawn moves
        moves['bp'] = [(0, +1)]
        moves['wp'] = [(0, -1)]
        return moves

    def get_complex_moves(self, board: list[list[str]], piece_location: tuple[int, int]) -> list[tuple[int, int, str]]:
        '''returns a list of tuples that represent possibly legal complex moves. This tuple repersents (new_x, new_y, move_type)
        This method does not send moves that move you through pieces. 
        
        Keyword arguments:
        \t board: a list of strings repersenting the board position
        \t piece: the location of the piece examined 
        '''
        final: list[tuple[int, int, str]] = []

        piece: str = board[piece_location[1]][piece_location[0]]
        piece_type: str = get_type(piece)
        piece_color: str = get_color(piece)


        # castling
        if (piece_type == KING and not has_moved(piece)):
            y: int = 0 if piece_color == BLACK else 7
            queen_rook_has_not_moved_and_queen_side_empty: bool = not has_moved(board[y][0]) and is_empty_squares([board[y][1], board[y][2], board[y][3]])
            if queen_rook_has_not_moved_and_queen_side_empty:
                final.append((2, y, SHORT_CASTLE))

            king_rook_has_not_moved_and_king_side_empty: bool = not has_moved(board[y][7]) and is_empty_squares([board[y][6], board[y][5]])
            if king_rook_has_not_moved_and_king_side_empty:
                final.append((7, y, LONG_CASTLE))

        elif (piece_type == PAWN):
            if (piece_color == BLACK):
                # double move
                black_pawn_on_start_rank: bool = piece_location[1] == 1
                if black_pawn_on_start_rank:
                    final.append(((0, +2, DOUBLE_MOVE)))
                # captures (this also does enpassent)
                for i in [(1,1), (-1,1)]:
                    black_new_pos: tuple[int, int] = (piece_location[0] + i[0], piece_location[1] + i[1])
                    black_examined_square: str = board[black_new_pos[1]][black_new_pos[0]]
                    filled_by_white: bool = not is_empty(black_examined_square) and get_color(black_examined_square) == WHITE
                    if filled_by_white:
                        final.append((i[0], i[1], PAWN_CAPTURE))

            else:
                # double move
                white_pawn_on_start_rank: bool = piece_location[1] == 6
                if white_pawn_on_start_rank:
                    final.append(((0, -2, DOUBLE_MOVE)))
                # captures (this also does enpassent)
                for i in [(1,-1), (-1,-1)]:

                    white_new_pos: tuple[int, int] = (piece_location[0] + i[0], piece_location[1] + i[1])
                    white_examined_square: str = board[white_new_pos[1]][white_new_pos[0]]
                    filled_by_black: bool = not is_empty(white_examined_square) and get_color(white_examined_square) == BLACK
                    if filled_by_black:
                        final.append((i[0], i[1], PAWN_CAPTURE))
        return final