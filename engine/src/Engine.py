import copy, time
from multiprocessing import Pool, cpu_count, JoinableQueue, Queue
from engine.src.constants.static import BLACK, WHITE, KING, EMPTY, EN_PASSENT, PAWN
from  engine.src.constants.engineTypes import MoveType, boardType, RunType
from .helpers.square_analysis import get_color, get_type
from .helpers.board_analysis import sight_on_square, find_king
from .database.Searcher import Searcher
from .EngineRunner import EngineRunner
from .generator.generator import Generator

# TODO: engine checkmate not dropping user to checkmate screen
class Engine():
    def __init__(self) -> None:
        self.search: Searcher = Searcher()
        self.transposeTable: dict[str, float] = {}
        self.generator: Generator = Generator()

        self.to_examine: JoinableQueue = JoinableQueue()
        self.results: Queue = Queue()

        self.runners = [EngineRunner(self.to_examine, self.results, self.transposeTable) for i in range(cpu_count())]
        for runner in self.runners:
            runner.start()

    def accept_board(self, boardStr: str) -> list[list[str]]:
        '''Takes in a boardStr and parses the board in a way the engine can understand.
        Also extracts important features about the board'''
        split: list[str] = boardStr.split('/')
        self.fifty_move_rule_counter = int(split.pop())
        self.move_counter = int(split.pop())

        self.board: list[list[str]] = []
        for row in split:
            row = row.strip()
            s = row.split(" ")
            f_row = []
            for grid in s:
                f_row.append(grid.replace("--", "  "))
            self.board.append(f_row)
        # print("----------------- Accepted Pos ----------")
        # for row in self.board:
        #     print(row)
        return self.board
    
    def get_best_move(self) -> MoveType:
        s = time.time_ns()
        '''Gets the engine's best guess at what a move is.'''
        current_color = self.to_move(self.move_counter)
        
        mv = self.search.query_theory(self.board, current_color)
        if self.search.is_valid(mv):
            return mv
        
        possible_moves = self.generator.get_moves(self.board, find_king(self.board, current_color))
        for move in possible_moves:
            self.to_examine.put((move, self.board, current_color))
        
        self.to_examine.join()

        results: list[RunType] = []
        for i in range(self.results.qsize()):
            results.append(self.results.get())

        for result in results:
            self.transposeTable[str(result['board'])] = result['value']

        for runner in self.runners:
            runner.update(self.transposeTable)
    
        print((time.time_ns()-s) / 10000000)

        final_moves = sorted(results, key=lambda x: x['depth'], reverse=True)
        return self.get_best(final_moves, current_color)   

    def get_best_val(self, input: list[float], color: str) -> float:
        '''runs min or max, on the input, depending on the color provided'''
        if color == BLACK:
            return min(input)
        return max(input)

    def get_best(self, input: list[RunType], color: str) -> MoveType:
        '''Given a list of moves and their values, return the best move for a specific color'''
        if color == BLACK:
            return min(input, key=lambda x: x['value'])['move']
        return max(input, key=lambda x: x['value'])['move']

    def to_move(self, turn_count: int) -> str:
        '''gets who's move it is'''
        if turn_count % 2 == 0:
            return WHITE
        return BLACK

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