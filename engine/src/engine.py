import copy, time
from multiprocessing import Pool, cpu_count
from engine.src.constants.static import BLACK, WHITE, KING, EMPTY, EN_PASSENT, PAWN
from  engine.src.constants.engineTypes import MoveType, boardType
from .helpers.square_analysis import get_color, get_type
from .helpers.board_analysis import sight_on_square, find_king
from .helpers.helpers import flip
from .generator.generator import Generator
from .evaluator.evaluator import Evaluator
# TODO: king vanishes
class engine():
    def __init__(self) -> None:
        self.generator: Generator = Generator()
        self.evaluator: Evaluator = Evaluator()
        self.transposeTable: dict[str, float] = {}

    def accept_board(self, boardStr: str) -> list[list[str]]:
        '''Takes in a boardStr and parses the board in a way the engine can understand.
        Also extracts important features about the board'''
        split: list[str] = boardStr.split('/')
        self.fifty_move_rule_counter: int = int(split.pop())
        self.move_counter: int = int(split.pop())

        self.board: list[list[str]] = []
        for row in split:
            row = row.strip()
            s = row.split(" ")
            f_row = []
            for grid in s:
                f_row.append(grid.replace("--", "  "))
            self.board.append(f_row)
        
        return self.board
    
    def get_best_move(self) -> MoveType:
        s = time.time_ns()
        '''Gets the engine's best guess at what a move is.'''
        current_color = self.to_move(self.move_counter)
        possible_moves = self.generator.get_moves(self.board, find_king(self.board, current_color))
        value_moves: list[tuple[MoveType, float, boardType, str]] = [(move, -1, self.board, current_color) for move in possible_moves]
        # TODO: value moves not being updated
        with Pool(processes=cpu_count()) as pool:
            value_moves = pool.starmap(self.transformer, value_moves)

        # test this things impact on preformance
        for m in value_moves:
            self.transposeTable[str(m[2])] = m[1]
        print((time.time_ns()-s) / 10000000)
        return self.get_best(value_moves, current_color)   

    def transformer(self, move: MoveType, dummy: float, board: boardType, color: str) -> tuple[MoveType, float, boardType, str]:
        '''transforms a list of value moves into one that carries a result and a transformed position'''
        new_pos: list[list[str]] = self.result(board, move)
        pos_val: float = self.value(new_pos, color)
        return (move, pos_val, new_pos, color)  

    def value(self, pos: list[list[str]], perspective: str, curr_depth: int = 1, 
            max_depth: int=3, max_val:float=float('-inf'), min_val:float=float('inf')) -> float:
        '''Estimates the value of a move using evaluator and MINIMAX. Currently unfinished. 

        Keyword arguments:
        \t pos -- a board position
        \t perspective -- the perspective from which to examine the moves (IE, the person who just moved)
        \t currDepth -- the depth to which we have explored (default = 1)
        \t max_depth -- the max deppth of the engine (default = 3)
        \t max_val -- the top most value found (used in pruning) (default = -inf) 
        \t min_val -- the bottom most value found (used in pruning) (default = inf) 
        '''
        # TODO: taking far too long. Replace with bfs search to make code more debuggable 
        # TODO: eval seems to be wrong on occasion? Was getting negatives for seemingly no reason
        # base cases
        if str(pos) in self.transposeTable:
            return self.transposeTable[str(pos)]
        finished: bool = self.is_termainal(pos)
        if curr_depth >= max_depth or finished:
            return self.evaluator.eval(pos, finished)
        
        enemy_perspective: str = flip(perspective)
        # get all the possible moves
        possible_moves: list[MoveType] = self.generator.get_moves(pos, find_king(pos, enemy_perspective))
        # initalize dummy values for the best_value
        best_value = float('-inf') if enemy_perspective == WHITE else float('inf')
        # for every move
        for move in possible_moves:
            new_pos: list[list[str]] = self.result(pos, move)
            # get the value of the new position
            pos_val: float = self.value(new_pos, enemy_perspective, curr_depth=curr_depth+1, max_val=max_val, min_val=min_val)
            # update the value as needed
            best_value = self.get_best_val([best_value, pos_val], enemy_perspective)

            # keep track of max and min
            if enemy_perspective == WHITE:
                max_val = max(max_val, pos_val)
            else:
                min_val = min(min_val, pos_val)
            # prune
            if min_val <= max_val:
                break
            
        return best_value

    def get_best_val(self, input: list[float], color: str) -> float:
        '''runs min or max, on the input, depending on the color provided'''
        if color == BLACK:
            return min(input)
        return max(input)

    def get_best(self, input: list[tuple[MoveType, float, boardType, str]], color: str) -> MoveType:
        '''Given a list of moves and their values, return the best move for a specific color'''
        if color == BLACK:
            return min(input, key=lambda x: x[1])[0]
        return max(input, key=lambda x: x[1])[0]

    def to_move(self, turn_count: int) -> str:
        '''gets who's move it is'''
        if turn_count % 2 == 0:
            return WHITE
        return BLACK
    
    def is_termainal(self, board: list[list[str]]) -> bool:
        '''Returns if the game is over. 
        Keyword arguements:
        \t board - the board 
        '''
        for color in [WHITE, BLACK]:
            enemy = flip(color)

            # see if the enemy king is in check, but has no moves
            moves = self.generator.get_moves(board, find_king(board, enemy)) 
            eyes_on_king: dict[str, list[tuple[int, int]]] = sight_on_square(board, find_king(board, color))
            king_in_check: bool = len(eyes_on_king[enemy]) > 0
            # a king is in checkmate
            if king_in_check: 
                if len(moves) == 0:
                    return True
            # stalemate
            if len(moves) == 0:
                return True

        # draw by fifty move rule
        if self.fifty_move_rule_counter / 2 >= 50:
            return True
        return False
    
    def result(self, board: list[list[str]], move: MoveType) -> list[list[str]]:
        '''Simulates a board position
        
        Keyword arguements:
        \t board - the board 
        \t move - a dict of moveType, representing the move
        '''
        oldx, oldy = move['original']
        newx, newy = move['new']

        piece_type = get_type(board[oldy][oldx])
        color = get_color(board[oldy][oldx])

        new_board: list[list[str]] = copy.deepcopy(board)
        new_board[newy][newx] = new_board[oldy][oldx]
        new_board[newy][newx] = new_board[newy][newx][0].lower() + new_board[newy][newx][1]
        new_board[oldy][oldx] = EMPTY

        if move['promotion'] != '':
            new_board[newy][newx] = new_board[newy][newx][0].lower() + move['promotion']
        # enpassent
        if get_type(board[newy][newx]) == EN_PASSENT and piece_type == PAWN:
            offset = 1 if color == WHITE else -1
            
            new_board[newy+offset][newx] = EMPTY
        
        # double move (to place enpassent)
        delta_y = abs(oldy - newy)
        if piece_type == PAWN and delta_y == 2:
            offset = -1 if color == BLACK else 1
            new_board[newy+offset][newx] = color + EN_PASSENT

        # castling (if the king is moving more then 1 square, it must be castling)
        delta_x = abs(oldx - newx)
        if piece_type == KING and delta_x > 1:
            if newx == 2:
                new_board[newy][0] = EMPTY
                new_board[newy][newx+1] = board[newy][0].lower()
            elif newx == 6:
                new_board[newy][7] = EMPTY
                new_board[newy][5] = board[newy][0].lower()

        return new_board
