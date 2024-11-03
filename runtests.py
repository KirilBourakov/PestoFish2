import unittest
from engine.tests.moves_tests import MovesTests

def test_suite():
    suite = unittest.TestSuite()
    suite.addTest(unittest.makeSuite(MovesTests))
    return suite

if __name__ == '__main__':
    unittest.main(defaultTest='test_suite')