import pygame
import game.constants.globals as globals

class AbstractPiece():
    def __init__(self, type, asset, moveset, color, hops=False):
        self.type = type
        self.asset = asset
        self.moveset = moveset
        self.has_moved = False
        self.hops = hops
        self.color = color

    def getPossibleMoves(self, pos):
        moves_list = []
        for move in self.moveset:
            new_pos = (pos[0] + move[0], pos[1] + move[1])
            if (new_pos[0] >= 0) and (new_pos[1] >= 0) and (new_pos[0] <= 7) and (new_pos[1] <= 7):   
                if len(move) == 3:
                    moves_list.append(new_pos + (move[2],))
                else:
                    moves_list.append(new_pos,)
        return moves_list
    
    def get_legal_moves(self, board_obj, pos=None):
        if pos is None:
            pos = board_obj.selected_square
        piece = board_obj.board[pos[1]][pos[0]]
        moves = piece.getPossibleMoves(pos)
        oldx, oldy = pos

        # given all possibly legal moves, loop over and remove illegal ones
        purged_moves = []
        for move in moves:
            legal = True

            # certain moves come with functional conditions. If the move does so, make sure the conditions are met
            if (len(move) == 3):
                newx, newy, func = move
                possible = func(old_position=pos, color=piece.color, new_position=(newx, newy), board=board_obj.board, move_num=board_obj.move_counter)
                if (not possible[0]):
                    continue
                move = (newx, newy, possible[1])
            else:
                newx, newy = move
            
            # This checks that there is nothing blocking you from moving to that square
            x_walker, y_walker = oldx, oldy
            while (abs(x_walker-newx) > 1 or abs(y_walker-newy) > 1) and piece.hops == False:
                # walk across, taking the same path as the piece.  
                if x_walker < newx:
                    x_walker += 1
                if x_walker > newx:
                    x_walker -= 1
                if y_walker < newy:
                    y_walker += 1
                if y_walker > newy:
                    y_walker -= 1
                # if you meet another piece, the move is illegal
                board_square_not_empty = board_obj.board[y_walker][x_walker] is not None
                if board_square_not_empty:
                    not_occupied_by_en_passent = board_obj.board[y_walker][x_walker].type != globals.EN_PASSANT_FLAG
                    if not_occupied_by_en_passent:
                        legal = False
        
            square_contains_same_color_piece = ((board_obj.board[newy][newx] is not None) and 
                                                (piece.color == board_obj.board[newy][newx].color) and 
                                                (board_obj.board[newy][newx].type != globals.EN_PASSANT_FLAG)
                                                )
            if square_contains_same_color_piece:
                continue

            # if this moves ends with your king in check, it is not allowed
            copied = board_obj.self_copy()
            copied.board[newy][newx] = piece
            copied.board[oldy][oldx] = None
            if copied.in_check(self.color):
                continue
            
            castling = len(move) == 3 and (move[2] == globals.LONG_CASTLE_FLAG or move[2] == globals.SHORT_CASTLE_FLAG)
            if castling:
                # can't castle out of check
                if board_obj.in_check(self.color):
                    continue
                # can't castle through check
                color = globals.Color.WHITE if self.color == globals.Color.BLACK else globals.Color.BLACK
                if move[2] == globals.LONG_CASTLE_FLAG:
                    if len(board_obj.get_sight_on_square_color((2, oldy), color)) > 0:
                        continue
                elif move[2] == globals.SHORT_CASTLE_FLAG:
                    if len(board_obj.get_sight_on_square_color((5, oldy), color)) > 0:
                        continue
            
            if legal: 
                purged_moves.append(move)
                       
        return purged_moves

    def show(self, x, y):
        window = pygame.display.get_surface()
        window.blit(self.asset, (x*globals.grid_size+(globals.resize_num/2), y*globals.grid_size+(globals.resize_num/2)))