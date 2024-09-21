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
        self.cur_tok = self.toks[self.tok_idx]
    

    def caculator(self):
        operators = []
        numbers = []


        while self.tok_idx < len(self.toks):
            self.cur_tok = self.toks[self.tok_idx]
            if self.cur_tok.type == INT:
                numbers.append(int(self.cur_tok.value))

            elif self.cur_tok.type == PLUS:
                operators.append(self.cur_tok.type)

            elif self.cur_tok.type == MINUS:
                operators.append(self.cur_tok.type)  

            elif self.cur_tok.type == TIME:
                operators.append(self.cur_tok.type)

            elif self.cur_tok.type == DIV:
                operators.append(self.cur_tok.type)

            self.tok_idx += 1


        idx = 0
        new_operators = []
        new_numbers = []
        factor = 0


        while idx < len(operators):
            if operators[idx] == TIME:
                factor = numbers[idx] * numbers[idx + 1]
                new_numbers.append(factor)

            elif operators[idx] == DIV:
                factor = numbers[idx] / numbers[idx + 1]
                new_numbers.append(factor)

            elif operators[idx] == PLUS and operators[idx - 1] not in [DIV, TIME, None]:
                new_operators.append(operators[idx])
                new_numbers.append(numbers[idx])

            elif operators[idx] == MINUS and operators[idx - 1] not in [DIV, TIME, None]:
                new_operators.append(operators[idx])
                new_numbers.append(numbers[idx])

            elif operators[idx] == PLUS:
                new_operators.append(operators[idx])

            elif operators[idx] == MINUS:
                new_operators.append(operators[idx])

            else:
                new_operators.append(operators[idx])

            idx += 1


        if operators[len(operators) - 1] in [PLUS, MINUS]:
            new_numbers.append(numbers[len(numbers) - 1])


        new_idx = 0
        result = new_numbers[0]


        while new_idx < len(new_operators):
            if new_operators[new_idx] == PLUS:
                result += new_numbers[new_idx + 1]

            elif operators[new_idx] == MINUS:
                result -= new_numbers[new_idx + 1]

            new_idx += 1


        print(result)
    
    def print(self):
        pass
