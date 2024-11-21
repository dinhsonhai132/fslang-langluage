'''
this is test file
'''
import random
import math

INT = "INT"
PLUS = "PLUS"
MINUS = "MINUS"
DIV = "DIV"
TIME = "TIME"
DIGITS = "1234567890"
ALPHA = "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm"
VAR = "VAR"

class data_type:
    def __init__(self, type ,value, name):
        self.type = type
        self.value = value
        self.name = name

class store:
    def __init__(self, name, val):
        self.name = name
        self.val = val

vars = []

class lexer:
    def __init__(self, string):
        self.string = string
        self.pos = 0
        self.cur_tok = self.string[self.pos]
        self.toks = []
    
    def make_int(self):
        number = ""
        while self.pos < len(self.string) and self.string[self.pos] in DIGITS:
            number += self.string[self.pos]
            self.pos += 1

        self.toks.append(data_type(INT, number, ""))
        self.pos -= 1
    
    def tok(self):
        self.cur = self.string[self.pos]

        while self.pos < len(self.string):

            if self.pos < len(self.string):
                self.cur = self.string[self.pos]
            else:
                self.cur = None

            if self.cur == "+":
                self.toks.append(data_type(PLUS, 0))

            elif self.cur == "-":
                self.toks.append(data_type(MINUS, 0))

            elif self.cur == "/":
                self.toks.append(data_type(DIV, 0))

            elif self.cur == "*":
                self.toks.append(data_type(TIME, 0))

            elif self.cur in DIGITS:
                self.make_int()
            
            elif self.cur in ALPHA:
                name = ""
                val = 0
            
                while self.pos < len(self.string):
                    cur_tok = self.string[self.pos]
                    if cur_tok in ALPHA:
                        name += cur_tok
                    elif cur_tok in DIGITS:
                        val = val * 10 + int(cur_tok)
                    self.pos += 1
                vars.append(store(name, int(val)))
                self.toks.append(data_type(VAR, 0, name))

            self.pos += 1
            
        return self.toks


class parser:
    def __init__(self, toks) -> None:
        self.toks = toks
        self.tok_idx = 0

    def eval(self):
        def get_next_tok():
            if self.tok_idx < len(self.toks):
                tok = self.toks[self.tok_idx]
                self.tok_idx += 1
            else:
                tok = '\0'
            return tok
        
        def factor():
            tok = get_next_tok()
            if tok.type == INT:
                return int(tok.value)
            elif tok.type == VAR:
                for var in vars:
                    if var.name == tok.name:
                        return var.val
        
        def term():
            result = factor()
            while True:
                tok = get_next_tok()
                if (tok.type == TIME):
                    result *= factor()
                elif (tok.type == DIV):
                    result /= factor()
                else:
                    self.tok_idx -= 1
                    break
            return result

        def expr():
            result = term()
            while True:
                tok = get_next_tok()
                if (tok.type == PLUS):
                    result += term()
                elif (tok.type == MINUS):
                    result = term()
                else:
                    self.tok_idx -= 1
                    break
            return result
        
        return expr()
    
while True:
    player = input()
    lexers = lexer(player)
    toks = lexers.tok()
    parsers = parser(toks)
    print(parsers.eval())
