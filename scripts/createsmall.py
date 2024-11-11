import sys
read_file = open(sys.argv[1], "r", encoding="cp437")
write_file = open(sys.argv[2], "w", encoding="cp437")

c = 0
for x in read_file:
    if c == int(sys.argv[3]):
        break
    write_file.write(x)
    c += 1
