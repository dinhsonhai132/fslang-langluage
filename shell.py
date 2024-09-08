class data_type:
    def __init__(self, type, value):
        self.type = type
        self.value = value


NUM = "NUM"
NONE = "NONE"
PLUS = "PLUS"
MINUS = "MINUS"
PRINT = "PRINT"


class lexer():
    def __init__(self, string) -> None:
        self.pos = 0
        self.string = string
        self.cur = self.string[self.pos]
        self.toks = []

    def tok(self):
        self.cur = self.string[self.pos]
        if self.cur in "1234567890":
            self.toks.append(data_type(PLUS, 0))
            self.toks.append(data_type(NUM, eval(self.cur)))

        while self.pos < len(self.string):
            self.cur = self.string[self.pos]
            if self.cur == "+":
                self.toks.append(data_type(PLUS, 0))
            elif self.cur == "-":
                self.toks.append(data_type(MINUS, 0))
            elif self.cur in "1234567890":
                self.toks.append(data_type(NUM, eval(self.cur)))
            self.pos += 1
        self.toks.append(data_type(NONE, 0))
        return self.toks


class parser:
    def __init__(self, toks):
        self.toks = toks
        self.tok_idx = 0
        self.cur_tok = self.toks[self.tok_idx]

    def calculators(self):
        operators = []
        numbers = []

        while self.tok_idx < len(self.toks) and self.cur_tok.type is not NONE:
            self.cur_tok = self.toks[self.tok_idx]
            if self.cur_tok.type == NUM:
                numbers.append(self.cur_tok.value)
            elif self.cur_tok.type == PLUS:
                operators.append(self.cur_tok.type)
            elif self.cur_tok.type == MINUS:
                operators.append(self.cur_tok.type)
            self.tok_idx += 1

        result = 0
        idx = 0
        while idx < len(operators):
            if operators[idx] == PLUS:
                result += numbers[idx]
            elif operators[idx] == MINUS:
                result -= numbers[idx]
            idx += 1

        return result

    def input_output(self):
        pass
