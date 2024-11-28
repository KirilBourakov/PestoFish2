import sqlite3
import sys

def main(db_name):
    db = sqlite3.connect(db_name)
    executor = db.cursor()

    executor.execute("CREATE TABLE positions(position, pk int NOT NULL, PRIMARY KEY (pk))")
    executor.execute("CREATE TABLE transpositions(pk int NOT NULL, inital int NOT NULL, new int NULL, FOREIGN KEY (inital) REFERENCES positions(pk), FOREIGN KEY (new) REFERENCES positions(pk), PRIMARY KEY (pk))")


if __name__ == '__main__':
    db = sys.argv[1]
    main(db)