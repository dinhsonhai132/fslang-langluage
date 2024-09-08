from shell import data_type
from shell import lexer
from shell import parser

run = True
print('''
fslang 0.0.2 (tags/v0.0.2:de54cfe) [MSC v.1938 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.''')
while run:
    player = input(">> ")
    if player == "help":
        print('go to website [https://fslang.html] for help')
    lexers = lexer(player)
    tok = lexers.tok()
    parsers = parser(tok)
    calculators = parsers.calculators()
    print(calculators)
    continue
