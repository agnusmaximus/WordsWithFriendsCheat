from __future__ import print_function
import sys
import scrabble_cheat

class scrabble_base_ai:
    def __init__(self, board_file_name, config_file_name, hand_file_name):
        self.board_file_name = board_file_name
        self.config_file_name = config_file_name
        self.hand_file_name = hand_file_name

    def generate_move_internal(self):
        candidates = scrabble_cheat.generate_word_candidates(self.board, self.config, self.hand)
        if candidates == []:
            return None
        return candidates[0]

    def generate_move(self):
        self.board = scrabble_cheat.read_board(self.board_file_name)
        self.config = scrabble_cheat.read_board_config(self.config_file_name)
        self.hand = scrabble_cheat.read_hand(self.hand_file_name)
        scrabble_cheat.update_score_multipliers(self.board, self.config)
        return self.generate_move_internal()

    def make_move(self, move):
        if move == None:
            return 0
        for letter in move[0]:
            self.board[letter.x][letter.y] = letter.character
            if not letter.is_on_board:
                if not letter.is_wildcard:
                    self.hand.remove(letter.character)
                else:
                    self.hand.remove('*')
        self.write_hand_to_file()
        self.write_board_to_file()
        return move[1]

    def write_hand_to_file(self):
        f = open(self.hand_file_name, "w")
        print("".join(self.hand), file=f)
        f.close()

    def write_board_to_file(self):
        f = open(self.board_file_name, "w")
        for i in range(len(self.board)):
            line = ''
            for j in range(len(self.board[i])):
                if self.board[i][j] == ' ':
                    line += '0'
                else:
                    line += self.board[i][j]
            print(line, file=f)
        f.close()

if __name__=="__main__":
    base = scrabble_base_ai("scrabble_board.txt", "scrabble_board_config.txt", "scrabble_hand.txt")
    move = base.generate_move()
    base.make_move(move)
    print(move)
