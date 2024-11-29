import unittest

from engine.src.database.Searcher import Searcher

class SearcherTests(unittest.TestCase):
    def test_get_move_from_fen(self):
        searcher = Searcher()
        # white short castle
        move = searcher.get_move_from_fen('rnb1kbnr/ppp1qppp/3p4/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R', 
                                          'rnb1kbnr/ppp1qppp/3p4/4p3/2B1P3/5N2/PPPP1PPP/RNBQ1RK1')
        
        self.assertEqual(move['original'], (4,7))
        self.assertEqual(move['new'], (6,7))

        # black enpassent
        move = searcher.get_move_from_fen('rnb1kbnr/p1p1qppp/3p4/4p3/Pp2P3/3B1N2/1PPP1PPP/RNBQ1RK1',
                                          'rnb1kbnr/p1p1qppp/3p4/4p3/4P3/p2B1N2/1PPP1PPP/RNBQ1RK1')
        self.assertEqual(move['original'], (1,4))
        self.assertEqual(move['new'], (0,5))