from shell import lexer
from shell import parser
from shell import data_type
import random
import datetime


print('''fslang 0.0.1 (tags/v0.0.1de54cf5) [MSC v.1938 64 bit (AMD64)] on win32
type "tutorial" or access https://github.com/dinhsonhai132?tab=repositories for more information...''')

now = datetime.datetime.now()

while True:
    print(now.strftime("[%Y-%m-%d %H:%M:%S]"))
    player = input(">>> ")

    if player in "\t\n":
        continue

    elif player == "tutorial":
        print("access web https://dinhsonhai132.github.io/fslang.github.io/fslang.html for tutorial")
    elif player == "version":
        print('''fslang 0.0.1 (tags/v0.0.1de54cf5) [MSC v.1938 64 bit (AMD64)] on win32
type "tutorial" or access https://github.com/dinhsonhai132?tab=repositories for more information...''')
    elif player == "cls":
        for i in range(100):
            print("\n")
    else:
        lexers = lexer(player)
        toks = lexers.tok()
        parsers = parser(toks)
        
        print(parsers.caculator())
