import subprocess
import sys
import os

def main(file_in, file_out, tmp_file):
    writer = open(file_out, 'w')
    tmp_writer = open(tmp_file, 'w')

    with open(file_in) as file:
        for row in file:
            tmp_writer.write(row)
            tmp_writer.close()

            result = subprocess.run(["pgn-extract.exe", "-Wfen", tmp_file], capture_output=True)
            out = result.stdout.decode().split('\n')
            for row in out:
                if row.count("[") == 0:
                    writer.write(row)
            
            writer.write('\n')
            tmp_writer = open(tmp_file, 'w')
            

    writer.close()
    tmp_writer.close()
    os.remove(tmp_file)

if __name__ == "__main__":
    file_in, file_out, tmp_file = sys.argv[1], sys.argv[2], sys.argv[3]
    main(file_in, file_out, tmp_file)
