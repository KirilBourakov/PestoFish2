import sys
reader = open(sys.argv[1], 'r')
writer = open(sys.argv[2], 'a')

c = 0
for x in reader:
    writer.write(x)
    c += 1
    if c >= int(sys.argv[3]):
        break

reader.close()
writer.close()