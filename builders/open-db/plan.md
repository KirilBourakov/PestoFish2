# To make:
Extract opening moves from wikipage
Turn a move into a pgn (NB), and the move before that as well (OB).
If NB is not in the db, add it to the db
Add a theory OB as inital, and NB as new


store pgn positions into sqlitedb
- pgn's should be connected via a table, making the schema:
    - pgn (primary key, String pgn)
    - theory (forign key pgn inital, forign key pgn new)

# To use:
1. query db for current position to see if theoretical moves exist
2. If it does, choose one at random, and compare PGN strings to find the difference, from which you can get the move