from engine.src.helpers.square_analysis import get_color, get_type, has_moved, is_empty_squares, is_empty, is_empty_include_en_passent
from engine.src.helpers.board_analysis import inbounds
from engine.src.constants.constants import KING, PAWN, KNIGHT, QUEEN, BLACK, WHITE, SHORT_CASTLE, LONG_CASTLE, DOUBLE_MOVE, CAPTURE, FORWARD, BACKWARD
from engine.src.constants.types import Vector

class Moves():
    def __init__(self) -> None:
        '''Constructs a Moves, which is a class for generating pseudolegal moves'''
        self.vectors: dict[str, Vector] = self.gen_vectors()

    def gen_vectors(self) ->  dict[str, Vector]:
        '''gets all the 'simple' moves (moves that are always avalible, unless they're blocked)

        returns a dict, who's key is the piece character. Pawns require color due to them only moving one direction.
        The value of the dict is a Vector, which is a dict with two properties:
        \t maxForce represents max squares a piece can travel
        \t directions the directions a piece can travel
        '''
        moves: dict[str, Vector] = {}
        
        # king moves (non castle)
        moves['K'] = Vector(maxForce=1, directions=[(-1, 1), (0, 1), (1,1),(-1,0), (1,0),(-1,-1), (0,-1), (1,-1)])

        # knight moves
        moves['k'] = Vector(maxForce=1, directions=[(+2, -1), (+2, +1),(-2, -1), (-2, +1),(-1, +2), (+1, +2),(-1, -2), (+1, -2)])

        # rook moves
        moves['r'] = Vector(maxForce=8, directions=[(0, 1), (-1,0), (1,0), (0,-1)])

        # bishop moves
        moves['b'] = Vector(maxForce=8, directions=[(-1, 1), (1,1), (-1,-1), (1,-1)])

        # queen
        moves['q'] = Vector(maxForce=8, directions=[(-1, 1), (-1,0), (-1,-1), (0,-1), (0, 1), (1,1), (1,0), (1,-1)])

        # pawn moves
        moves['bp'] = Vector(maxForce=1, directions=[(0, +1)])
        moves['wp'] = Vector(maxForce=1, directions=[(0, -1)])
        return moves

    def get_all_moves(self, board: list[list[str]], piece_location: tuple[int, int]) -> list[tuple[int, int, str]]:
        '''this method returns a list of all legal moves of a piece, not considering checks.
        
        Keyword arguments:
        \t board: a list of strings repersenting the board position
        \t piece_location: the location of the piece 
        '''
        final: list[tuple[int, int, str]] = self.get_simple_moves(board, piece_location)
        final.extend(self.get_complex_moves(board, piece_location))
        return final

    def get_simple_moves(self, board: list[list[str]], piece_location: tuple[int, int]) -> list[tuple[int, int, str]]:
        '''returns a list of tuples that represents possibly legal simple moves. This tuple repersents (new_x, new_y, move_type)
        This method doesn't return moves that move you through pieces.
        move_type indicates if the move is forward, back, or a capture
        
        Keyword arguments:
        \t board: a list of strings repersenting the board position
        \t piece_location: the location of the piece examined 
        '''
        piece: str = board[piece_location[1]][piece_location[0]]
        if is_empty(piece):
            return []
        piece_type: str = get_type(piece)
        piece_color: str = get_color(piece)
        index: str = piece_color + piece_type if piece_type == PAWN else piece_type
        move_vectors: Vector = self.vectors[index]
        
        final: list[tuple[int, int, str]] = []
        for direction in move_vectors['directions']:
            currForce = 1
            directionx, directiony = direction
            while currForce <= move_vectors['maxForce']:
                new_pos = (piece_location[0] + (directionx * currForce), piece_location[1] + (directiony * currForce))
                inbounds: bool = (new_pos[0] >= 0) and (new_pos[1] >= 0) and (new_pos[0] <= 7) and (new_pos[1] <= 7)
                if not inbounds: 
                    break

                if (is_empty(board[new_pos[1]][new_pos[0]])):
                    moveType: str = BACKWARD
                    if (piece_color == BLACK and directiony > 0) or (piece_color == WHITE and directiony < 0):
                        moveType = FORWARD
                    final.append((new_pos[0], new_pos[1], moveType))
                elif (piece_color != get_color(board[new_pos[1]][new_pos[0]]) and piece_type != PAWN):
                    # if get_type(board[new_pos[1]][new_pos[0]]) == KING:
                    #     for row in board:
                    #         print(row)
                    #     print(direction, currForce, piece_location, new_pos, piece_type)
                    #     raise IndexError('Trying to capture king')
                    final.append((new_pos[0], new_pos[1], CAPTURE))
                    break
                else: 
                    break
                currForce += 1
        # if piece_type == QUEEN and piece_color == BLACK:
        #     print(final)
        return final

    def get_complex_moves(self, board: list[list[str]], piece_location: tuple[int, int]) -> list[tuple[int, int, str]]:
        '''returns a list of tuples that represent possibly legal complex moves. This tuple repersents (new_x, new_y, move_type)
        This method does not send moves that move you through pieces. Complex moves are castling, pawn captures, pawn double moves
        
        Keyword arguments:
        \t board: a list of strings repersenting the board position
        \t piece_location: the location of the piece examined 
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
                final.append((6, y, LONG_CASTLE))

        elif (piece_type == PAWN):
            if (piece_color == BLACK):
                # double move
                black_pawn_on_start_rank: bool = piece_location[1] == 1
                empty_space: bool = is_empty(board[piece_location[1]+1][piece_location[0]]) and is_empty(board[piece_location[1]+2][piece_location[0]])
                if black_pawn_on_start_rank and empty_space:
                    final.append(((piece_location[0], piece_location[1]+2, DOUBLE_MOVE)))
                # captures (this also does enpassent)
                for i in [(1,1), (-1,1)]:
                    black_new_pos: tuple[int, int] = (piece_location[0] + i[0], piece_location[1] + i[1])
                    if inbounds(black_new_pos):
                        black_examined_square: str = board[black_new_pos[1]][black_new_pos[0]]
                        filled_by_white: bool = not is_empty_include_en_passent(black_examined_square) and get_color(black_examined_square) == WHITE
                        if filled_by_white:
                            final.append((piece_location[0]+i[0], piece_location[1]+i[1], CAPTURE))

            else:
                # double move
                white_pawn_on_start_rank: bool = piece_location[1] == 6
                empty_space = is_empty(board[piece_location[1]+1][piece_location[0]]) and is_empty(board[piece_location[1]+2][piece_location[0]])
                if white_pawn_on_start_rank and empty_space:
                    final.append(((piece_location[0], piece_location[1]-2, DOUBLE_MOVE)))
                # captures (this also does enpassent)
                for i in [(1,-1), (-1,-1)]:
                    white_new_pos: tuple[int, int] = (piece_location[0] + i[0], piece_location[1] + i[1])
                    if inbounds(white_new_pos):
                        white_examined_square: str = board[white_new_pos[1]][white_new_pos[0]]
                        filled_by_black: bool = not is_empty_include_en_passent(white_examined_square) and get_color(white_examined_square) == BLACK
                        if filled_by_black:
                            final.append((piece_location[0]+i[0], piece_location[1]+i[1], CAPTURE))
        return final