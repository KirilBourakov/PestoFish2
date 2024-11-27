from html.parser import HTMLParser
import sys
import re

class Parse(HTMLParser):
    def __init__(self):
        super().__init__()
        self.inli = False
        self.searcher = re.compile(r"/*1. (N|)[a-h][0-9] (N|)[a-h][0-9]*") #

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
        if self.searcher.search(data):
            print(data)


def main():
    parser = Parse()
    with open(sys.argv[1], encoding="cp437") as file:
        for row in file:
            parser.feed(row)

if __name__ == "__main__":
    main()