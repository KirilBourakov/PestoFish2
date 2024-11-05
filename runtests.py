import unittest
from engine.tests.moves_tests import MovesTests
from engine.tests.generator_tests import GeneratorTests

def test_suite():
    suite = unittest.TestSuite()
    suite.addTest(unittest.makeSuite(MovesTests))
    suite.addTest(unittest.makeSuite(GeneratorTests))
    return suite

if __name__ == '__main__':
    unittest.main(defaultTest='test_suite')