from shell import lexer
from shell import parser
import random
import datetime

print('''fslang 0.0.1 (tags/v0.0.1de54cf5) [MSC v.1938 64 bit (AMD64)] on win32
type "help" or access https://github.com/dinhsonhai132?tab=repositories for more information...''')
now = datetime.datetime.now()
while True:
    print(now.strftime("[%Y-%m-%d %H:%M:%S]"))
    player = input(">>> ")
    lexers = lexer(player)
    toks = lexers.tok()
    parsers = parser(toks)
    print(parsers.caculator())
