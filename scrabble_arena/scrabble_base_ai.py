from __future__ import print_function
import sys
import scrabble_cheat

class scrabble_base_ai:
    def __init__(self, board_file_name, config_file_name, hand_file_name):
        self.board_file_name = board_file_name
        self.config_file_name = config_file_name
        self.hand_file_name = hand_file_name
        self.board = scrabble_cheat.read_board(board_file_name)
        self.config = scrabble_cheat.read_board_config(config_file_name)
        self.hand = scrabble_cheat.read_hand(hand_file_name)
        scrabble_cheat.update_score_multipliers(self.board, self.config)

    def generate_move(self):
        candidates = scrabble_cheat.generate_word_candidates(self.board, self.config, self.hand)
        return candidates[0]

    def make_move(self, move):
        for letter in move[0]:
            self.board[letter.x][letter.y] = letter.character
            if not letter.is_on_board:
                if not letter.is_wildcard:
                    self.hand.remove(letter.character)
                else:
                    self.hand.remove('*')
        
        self.write_hand_to_file()
        #self.write_board_to_file()

    def write_hand_to_file():
        f = open(self.hand_file_name, "w")
        print("".join(self.hand), file=f)
        f.close()
        

base = scrabble_base_ai("scrabble_board.txt", "scrabble_board_config.txt", "scrabble_hand.txt")
move = base.generate_move()
print(move)
base.make_move(move)
