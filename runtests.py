import unittest
from engine.tests.moves_tests import MovesTests
from engine.tests.generator_tests import GeneratorTests
from engine.tests.engine_result_tests import EngineResultTests
from engine.tests.engine_value_tests import EngineValueTests
from engine.tests.engine_get_best_move import EngineGetBestMoveTest
from engine.tests.engine_is_terminal_tests import EngineIsTerminalTest
from engine.tests.searcher_tests import SearcherTests

def test_suite():
    suite = unittest.TestSuite()
    suite.addTest(unittest.makeSuite(MovesTests))
    suite.addTest(unittest.makeSuite(GeneratorTests))
    suite.addTest(unittest.makeSuite(EngineResultTests))
    suite.addTest(unittest.makeSuite(EngineValueTests))
    suite.addTest(unittest.makeSuite(EngineGetBestMoveTest))
    suite.addTest(unittest.makeSuite(EngineIsTerminalTest))
    suite.addTest(unittest.makeSuite(SearcherTests))
    return suite

if __name__ == '__main__':
    unittest.main(defaultTest='test_suite')