import sys
import csv

file_in, file_out_dir, batchsize = sys.argv[1], sys.argv[2], int(sys.argv[3])


writer = open(f"{file_out_dir}/1.csv", 'a')
line_counter = 0
file_counter = 1

with open(sys.argv[1], newline='') as file:
    reader = csv.DictReader(file)
    for row in reader:

        # write headers into CSV
        if line_counter == 0:
            for i, key in enumerate(row.keys()):
                writer.write(key)
                if i < len(row.keys()) - 1:
                    writer.write(',')
            writer.write('\n')

        for i, key in enumerate(row.keys()):
            writer.write(row[key])
            if i < len(row.keys()) - 1:
                writer.write(',')

        writer.write('\n')
        line_counter += 1
        if line_counter > batchsize:
            file_counter += 1
            line_counter = 0
            writer.close()
            writer = open(f"{file_out_dir}/{file_counter}.csv", 'a')

writer.close()