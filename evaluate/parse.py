import csv

data = []
with open("positions.csv", mode="r", newline="") as file:
    reader = csv.DictReader(file)
    for row in reader:
        if (
            (row["score"] != "")
            and (abs(int(row["score"])) < 20)
            and int(row["fen"][-1]) > 5
        ):
            data.append(row["fen"])

with open("out.txt", mode="w", newline="") as file:
    for d in data:
        file.write(d + "\n")
