import sqlite3
import sys

def main(db_name, input):
    db = sqlite3.connect(db_name)
    executor = db.cursor()
    
    executor.execute('CREATE TABLE positions(position TINYTEXT UNIQUE, pk int NOT NULL, PRIMARY KEY (pk))')
    executor.execute('CREATE TABLE transpositions(pk int NOT NULL, inital int NOT NULL, new int NULL, FOREIGN KEY (inital) REFERENCES positions(pk), FOREIGN KEY (new) REFERENCES positions(pk), PRIMARY KEY (pk))')
    executor.execute('INSERT INTO positions(position, pk) VALUES (?,?)', ('rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR  w  KQkq  -', 1))

    pk = 2
    trans_pk = 1
    with open(input) as file:
        curr_pos = ''
        prev_pos = curr_pos
        for row in file:
            prev_pos = curr_pos.strip()
            curr_pos = row.strip()
            
            if curr_pos == '' or prev_pos == '':
                continue

            curr_pos_db = executor.execute('SELECT pk FROM positions WHERE position == ?', (curr_pos,)).fetchone()
            prev_pos_db = executor.execute('SELECT pk FROM positions WHERE position == ?', (prev_pos,)).fetchone()

            if curr_pos_db is None:
                executor.execute('INSERT INTO positions(position, pk) VALUES (?, ?)', (curr_pos, pk))
                db.commit()
                pk += 1
                curr_pos_db = executor.execute('SELECT pk FROM positions WHERE position == ?', (curr_pos,)).fetchone()

            executor.execute("INSERT INTO transpositions(pk, inital, new) VALUES (?,?,?)", (trans_pk, prev_pos_db[0], curr_pos_db[0]))
            db.commit()
            trans_pk += 1

if __name__ == '__main__':
    db, input = sys.argv[1], sys.argv[2]
    main(db, input)