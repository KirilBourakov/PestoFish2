from html.parser import HTMLParser
import sys
import re

class Parse(HTMLParser):
    def __init__(self, out):
        super().__init__()
        self.inli = False
        self.searcher = re.compile(r"/*1. (N|)[a-h][0-9] (N|)[a-h][0-9]*")
        self.file = open(out, 'w')

    def handle_starttag(self, tag, attrs):
        if tag == "li":
            self.inli = True

    def handle_endtag(self, tag):
        if tag == "li":
            self.inli = True

    def handle_data(self, data):
        if not self.inli:
            return
        if not data.strip():
            return
        data = data.strip().replace("\n", " ")
        result = self.searcher.search(data)
        if result:
            self.file.write(data[result.start():])
            self.file.write(" 1-0")
            self.file.write('\n')

    def finish(self):
        self.file.close()

def main():
    out = sys.argv[2]
    parser = Parse(out)
    with open(sys.argv[1], encoding="cp437") as file:
        for row in file:
            parser.feed(row)
    parser.finish()

if __name__ == "__main__":
    main()