from html.parser import HTMLParser
import sys

class Parse(HTMLParser):
    def __init__(self):
        super().__init__()
        self.inli = False

    def handle_starttag(self, tag, attrs):
        if tag == "li":
            self.inli = True

    def handle_endtag(self, tag):
        if tag == "li":
            self.inli = True

    def handle_data(self, data):
        if self.inli:
            if data.strip():
                print(data.strip())

def main():
    parser = Parse()
    with open(sys.argv[1], encoding="cp437") as file:
        for row in file:
            parser.feed(row)

if __name__ == "__main__":
    main()