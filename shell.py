import random
import math

INT = "INT"
PLUS = "PLUS"
MINUS = "MINUS"
DIV = "DIV"
TIME = "TIME"
DIGITS = "1234567890"


class data_type:
    def __init__(self, type ,value):
        self.type = type
        self.value = value


class lexer:
    def __init__(self, string):
        self.string = string
        self.pos = 0
        self.cur_tok = self.string[self.pos]


    def tok(self):
        toks = []
        self.cur = self.string[self.pos]


        while self.pos < len(self.string):
            if self.pos < len(self.string):
                self.cur = self.string[self.pos]
            else:
                self.cur = None
            if self.cur == "+":

                toks.append(data_type(PLUS, 0))
            elif self.cur == "-":

                toks.append(data_type(MINUS, 0))
            elif self.cur == "/":

                toks.append(data_type(DIV, 0))
            elif self.cur == "*":

                toks.append(data_type(TIME, 0))
            elif self.cur in DIGITS:
                number = ""

                while self.pos < len(self.string) and self.string[self.pos] in DIGITS:
                    number += self.string[self.pos]
                    self.pos += 1

                toks.append(data_type(INT, number))
                self.pos -= 1

            self.pos += 1
            
        return toks


class parser:
    def __init__(self, toks) -> None:
        self.toks = toks
        self.tok_idx = 0

    

    def eval(self):
        def get_next_tok():
            return self.toks[self.tok_idx]
            self.tok_idx += 1
        
        def factor():
            if self.toks[self.tok_idx].type == INT:
                return self.toks[self.tok_idx]
        
        def term():
            result = factor()
            while True:
                if (self.toks[self.tok_idx].type == TIME):
                    result *= factor()
                elif (self.toks[self.tok_idx].type == DIV):
                    result /= factor()
                else:
                    tok_idx -= 1
                    break
            return result

        def expr():
            result = term()
            while True:
                if (self.toks[self.tok_idx].type == PLUS):
                    result *= term()
                elif (self.toks[self.tok_idx].type == MINUS):
                    result /= term()
                else:
                    tok_idx -= 1
                    break
            return result
        
        return expr()
        
