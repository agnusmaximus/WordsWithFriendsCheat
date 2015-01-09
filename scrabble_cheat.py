import sys
import os
import copy
from itertools import groupby
from operator import mul
import cPickle as pickle

# Score multipliers
BOARD_SZ = 15
(NORM, DL, TL, DW, TW) = (1, 2, 3, 4, 5)
POINT_VALS = {'a' : 1, 'b' : 4, 'c' : 4, 'd' : 2, 'e' : 1, 'f' : 4, 'g' : 3, 'h' : 3, 'i' : 1, \
              'j' : 10, 'k' : 5, 'l' : 2, 'm' : 4, 'n' : 2, 'o' : 1, 'p' : 4, 'q' : 10, 'r' : 1, \
              's' : 1, 't' : 1, 'u' : 2, 'v' : 5, 'w' : 4, 'x' : 8, 'y' : 3, 'z' : 10, '*' : 0}
midstring_prefix_letters = {}
startstring_suffix_letters = {}
valid_words = set()

DICT_FILE = "dict.txt"
VALID_WORDS_FILE = "valid_words.p"
STARTSTRING_SUFFIX_LETTERS_FILE = "startstring_suffix_letters.p"
MIDSTRING_PREFIX_LETTERS_FILE = "midstring_prefix_letters.p"

######################################################################
# AUX DATA STUFF
######################################################################
def add_startstring_suffix_letter(substring, letter):
    if substring not in startstring_suffix_letters:
        startstring_suffix_letters[substring] = set()
    startstring_suffix_letters[substring].add(letter)

def add_midstring_prefix_letter(substring, letter):
    if substring not in midstring_prefix_letters:
        midstring_prefix_letters[substring] = set()
    midstring_prefix_letters[substring].add(letter)

def add_aux_data(word):
    for i in range(len(word)):
        for j in range(i+1, len(word)+1):
            substring = word[i:j]
            if i == 0 and j == len(word):
                valid_words.add(substring)
            if i == 0 and j != len(word):
                add_startstring_suffix_letter(substring, word[j])
            if i != 0:
                add_midstring_prefix_letter(substring, word[i-1])

def init_aux_data():
    global startstring_suffix_letters, midstring_prefix_letters, valid_words
    if not os.path.exists(VALID_WORDS_FILE) or \
        not os.path.exists(STARTSTRING_SUFFIX_LETTERS_FILE) or \
        not os.path.exists(MIDSTRING_PREFIX_LETTERS_FILE):
        print("Computing aux data...")
        d = open(DICT_FILE, "r")
        for line in d:
            line = line.strip()
            add_aux_data(line)
        midstring_prefix_letters[""] = set("abcdefghijklmnopqrstuvwxyz")
        startstring_suffix_letters[""] = set("abcdefghijklmnopqrstuvwxyz")
        pickle.dump(valid_words, open(VALID_WORDS_FILE, "wb"))
        pickle.dump(startstring_suffix_letters, open(STARTSTRING_SUFFIX_LETTERS_FILE, "wb"))
        pickle.dump(midstring_prefix_letters, open(MIDSTRING_PREFIX_LETTERS_FILE, "wb"))
    else:
        print("Loading aux data from files...")
        valid_words = pickle.load(open(VALID_WORDS_FILE, "rb"))
        startstring_suffix_letters = pickle.load(open(STARTSTRING_SUFFIX_LETTERS_FILE, "rb"))
        midstring_prefix_letters = pickle.load(open(MIDSTRING_PREFIX_LETTERS_FILE, "rb"))

init_aux_data()

######################################################################
# SCRABBLE LETTER CLASS
######################################################################
class Letter:
    def __init__(self, character, x, y, is_on_board=False, wildcard=False):
        self.character = character
        self.x, self.y = x, y
        self.is_on_board = is_on_board
        self.is_wildcard = wildcard
        self.point_value = POINT_VALS[character] * int(not self.is_wildcard)
        
    def __copy__(self):
        return Letter(self.character, self.x, self.y, self.is_on_board, self.is_wildcard)

    def __eq__(self, other):
        return self.character == other.character and \
          self.x == other.x and \
          self.y == other.y
          
    def __ne__(self, other):
        return not self.__eq__(other)

    def __repr__(self):
        return self.character
    
    def __str__(self):
        return self.character

    def details(self):
        detail = self.character + " x:" + str(self.y+1) + " y:" + str(self.x+1) + \
          " pt-val:" + str(self.point_value)
        if self.is_on_board:
            detail += " on_board=True"
        if self.is_wildcard:
            detail += " wildcard=True"
        return detail

######################################################################
# IO
######################################################################
def get_scrabble_char(char):
    if char == '0':
        return ' '
    if char == '*':
        return '*'
    return char.lower()

def read_board(board_file_path, op=get_scrabble_char):
    board_file = open(board_file_path, "r")
    board = []
    for line in board_file:
        board.append([op(x) for x in list(line.strip())])
    board_file.close()
    return board

def read_board_config(board_config_file_path):
    return read_board(board_config_file_path, lambda x: int(x))

def read_hand(scrabble_hand_file_path):
    f = open(scrabble_hand_file_path, "r")
    return_value = [get_scrabble_char(x) for x in list(f.readline().strip())]
    f.close()
    return return_value

######################################################################
# MISC
######################################################################
def update_score_multipliers(board, config):
    for i in range(len(board)):
        for j in range(len(board[i])):
            if board[i][j] != ' ':
                config[i][j] = NORM

######################################################################
# SEARCH
######################################################################
def generate_word_candidates(board, config, hand):
    board_letters = accumulate_board_letters(board)
    hand_letters = accumulate_hand_letters(hand)
    print("-Generating possible words...")
    possible_words = generate_possible_words(board, board_letters, hand_letters)
    print("-Scoring words...")
    word_score_pairings = [(x, score(board, config, x)) for x in possible_words]
    sorted_score_pairings = sorted(word_score_pairings, key=lambda x: x[1], reverse=True)
    unique_sorted_score_pairings = [k for k,v in groupby(sorted_score_pairings)]
    return unique_sorted_score_pairings

def accumulate_board_letters(board):
    letters = []
    for i in range(len(board)):
        for j in range(len(board[i])):
            if board[i][j] != ' ':
                letters.append(Letter(board[i][j], i, j, True))
    return letters

def accumulate_hand_letters(hand):
    letters = []
    for letter in hand:
        letters.append(Letter(letter, -1, -1, False, letter == '*'))
    return letters

def generate_possible_words(board, board_letters, hand_letters):
    anchor_positions = get_anchor_positions(board_letters, board)
    if len(anchor_positions) == 0:
        anchor_positions.add((7, 7))
    possible_words = []
    vertical_cross_section = generate_vertical_cross_section(hand_letters, board_letters, board)
    horizontal_cross_section = generate_horizontal_cross_section(hand_letters, board_letters, board)
    for anchor_pos in anchor_positions:
        possible_words += \
          generate_horizontal_words(board, hand_letters, anchor_pos, anchor_positions,
                                    vertical_cross_section)
        possible_words += \
          generate_vertical_words(board, hand_letters, anchor_pos, anchor_positions,
                                  horizontal_cross_section)
    return possible_words

def get_anchor_positions(board_letters, board):
    anchor_positions = set()
    for letter in board_letters:
        x, y = letter.x, letter.y
        if x+1 < BOARD_SZ and board[x+1][y] == ' ':
            anchor_positions.add((x+1, y))
        if x-1 >= 0 and board[x-1][y] == ' ':
            anchor_positions.add((x-1, y))
        if y+1 < BOARD_SZ and board[x][y+1] == ' ':
            anchor_positions.add((x, y+1))
        if y-1 >= 0 and board[x][y-1] == ' ':
            anchor_positions.add((x, y-1))
    return anchor_positions

def get_num_non_anchor_pos(dirx, diry, x, y, all_anchor_pos, board):
    assert(dirx <= 0 and diry <= 0)
    x, y, k = x+dirx, y+diry, 0
    while x >= 0 and y >= 0:
        if (x, y) in all_anchor_pos or board[x][y] != ' ':
            return k
        x, y, k = x+dirx, y+diry, k+1
    return k

def generate_vertical_cross_section(hand_letters, board_letters, board):
    hand_set = set([x.character for x in hand_letters])
    cross_section = [[None for x in range(BOARD_SZ)] for y in range(BOARD_SZ)]
    for board_letter in board_letters:
        board_character, x, y = board_letter.character, board_letter.x, board_letter.y
        for hand_character in hand_set:
            if y < BOARD_SZ-1:
                if cross_section[x][y+1] == None:
                    cross_section[x][y+1] = set()
                placed_letter = Letter(hand_character, x, y+1)
                whole_seq = get_whole_vertical_word(board, [placed_letter])
                if "".join(c.character for c in whole_seq) in valid_words:
                    cross_section[x][y+1].add(hand_character)
            if y > 0:
                if cross_section[x][y-1] == None:
                    cross_section[x][y-1] = set()
                placed_letter = Letter(hand_character, x, y-1)
                whole_seq = get_whole_vertical_word(board, [placed_letter])
                if "".join(c.character for c in whole_seq) in valid_words:
                    cross_section[x][y-1].add(hand_character)
    for i in range(BOARD_SZ):
        for j in range(BOARD_SZ):
            if cross_section[i][j] == None:
                cross_section[i][j] = hand_set
    return cross_section

def generate_horizontal_cross_section(hand_letters, board_letters, board):
    hand_set = set([x.character for x in hand_letters])
    cross_section = [[None for x in range(BOARD_SZ)] for y in range(BOARD_SZ)]
    for board_letter in board_letters:
        board_character, x, y = board_letter.character, board_letter.x, board_letter.y
        for hand_character in hand_set:
            if x < BOARD_SZ-1:
                if cross_section[x+1][y] == None:
                    cross_section[x+1][y] = set()
                placed_letter = Letter(hand_character, x+1, y)
                whole_seq = get_whole_horizontal_word(board, [placed_letter])
                if "".join(c.character for c in whole_seq) in valid_words:
                    cross_section[x+1][y].add(hand_character)
            if x > 0:
                if cross_section[x-1][y] == None:
                    cross_section[x-1][y] = set()
                placed_letter = Letter(hand_character, x-1, y)
                whole_seq = get_whole_horizontal_word(board, [placed_letter])
                if "".join(c.character for c in whole_seq) in valid_words:
                    cross_section[x-1][y].add(hand_character)
    for i in range(BOARD_SZ):
        for j in range(BOARD_SZ):
            if cross_section[i][j] == None:
                cross_section[i][j] = hand_set
    return cross_section

def generate_horizontal_words(board, hand, cur_anchor_pos, all_anchor_pos, cross_section):
    anchor_x, anchor_y = cur_anchor_pos[0], cur_anchor_pos[1]
    k = get_num_non_anchor_pos(-1, 0, anchor_x, anchor_y, all_anchor_pos, board)
    generated_valid_words = []
    
    def add_legal_move(seq):
        generated_valid_words.append(seq)
    
    def extend_right(cur_seq, (x, y), board, hand):
        cur_string  = "".join([letter.character for letter in cur_seq])
        if x >= BOARD_SZ:
            if cur_string in valid_words:
                add_legal_move(cur_seq)
            return
        if board[x][y] == ' ':
            if cur_string in valid_words:
                add_legal_move(cur_seq)
            for i, letter in enumerate(hand):
                if letter.character in startstring_suffix_letters.get(cur_string, []) and \
                    letter.character in cross_section[x][y]:
                    remaining_hand = hand[:i] + hand[i+1:]
                    extend_right(cur_seq+[Letter(letter.character, x, y)], 
                                 (x+1, y), board, remaining_hand)
        else:
            if board[x][y] in startstring_suffix_letters.get(cur_string, []):
                extend_right(cur_seq+[Letter(board[x][y], x, y)], 
                             (x+1, y), board, hand)      
    
    def extend_left(cur_seq, (x, y), board, hand, limit):
        cur_string = "".join([letter.character for letter in cur_seq])
        if cur_string in startstring_suffix_letters:
            extend_right([], (x, y), board, hand)
        if limit > 0:
            for i, letter in enumerate(hand):
                if letter.character in midstring_prefix_letters.get(cur_string, []) and \
                  letter.character in cross_section[x][y]:
                    remaining_hand = hand[:i] + hand[i+1:]
                    board[x][y] = letter.character
                    extend_left([Letter(letter.character, x, y)]+cur_seq, 
                                (x-1, y), board, remaining_hand, limit-1)
                    board[x][y] = ' '

    if k == 0 and anchor_x-1 >= 0:
        while anchor_x > 0 and board[anchor_x-1][anchor_y] != ' ':
            anchor_x -= 1
        extend_right([], (anchor_x, anchor_y), board, hand)
    else:
        extend_left([], cur_anchor_pos, board, hand, k)
    return generated_valid_words

def generate_vertical_words(board, hand, cur_anchor_pos, all_anchor_pos, cross_section):
    anchor_x, anchor_y = cur_anchor_pos[0], cur_anchor_pos[1]
    k = get_num_non_anchor_pos(0, -1, anchor_x, anchor_y, all_anchor_pos, board)
    generated_valid_words = []
    
    def add_legal_move(seq):
        generated_valid_words.append(seq)
    
    def extend_down(cur_seq, (x, y), board, hand):
        cur_string  = "".join([letter.character for letter in cur_seq])
        if y >= BOARD_SZ:
            if cur_string in valid_words:
                add_legal_move(cur_seq)
            return
        if board[x][y] == ' ':
            if cur_string in valid_words:
                add_legal_move(cur_seq)
            for i, letter in enumerate(hand):
                if letter.character in startstring_suffix_letters.get(cur_string, []) and \
                    letter.character in cross_section[x][y]:
                    remaining_hand = hand[:i] + hand[i+1:]
                    extend_down(cur_seq+[Letter(letter.character, x, y)], 
                                 (x, y+1), board, remaining_hand)
        else:
            if board[x][y] in startstring_suffix_letters.get(cur_string, []):
                extend_down(cur_seq+[Letter(board[x][y], x, y)], 
                             (x, y+1), board, hand)      
    
    def extend_up(cur_seq, (x, y), board, hand, limit):
        cur_string = "".join([letter.character for letter in cur_seq])
        if cur_string in startstring_suffix_letters:
            extend_down([], (x, y), board, hand)
        if limit > 0:
            for i, letter in enumerate(hand):
                if letter.character in midstring_prefix_letters.get(cur_string, []) and \
                  letter.character in cross_section[x][y]:
                    remaining_hand = hand[:i] + hand[i+1:]
                    board[x][y] = letter.character
                    extend_up([Letter(letter.character, x, y)]+cur_seq, 
                                (x, y-1), board, remaining_hand, limit-1)
                    board[x][y] = ' '

    if k == 0 and anchor_y-1 >= 0:
        while anchor_y > 0 and board[anchor_x][anchor_y-1] != ' ':
            anchor_y -= 1
        extend_down([], (anchor_x, anchor_y), board, hand)
    else:
        extend_up([], cur_anchor_pos, board, hand, k)
    return generated_valid_words
    
def get_whole_horizontal_word(board, word):
  left_x, right_x, y, offset = word[0].x, word[-1].x, word[0].y, 0
  while left_x > 0 and board[left_x-1][y] != ' ':
    offset -= 1
    left_x -= 1
  while right_x < BOARD_SZ-1 and board[right_x+1][y] != ' ':
    right_x += 1
  horizontal_word, count = [], 0
  for index, i in enumerate(range(left_x, right_x+1)):
    while count < len(word) and word[count].is_on_board:
      count += 1
    if board[i][y] != ' ':
      horizontal_word.append(Letter(board[i][y], i, y, is_on_board=True))
    else:
      horizontal_word.append(copy.copy(word[offset+index]))
  return horizontal_word

def get_whole_vertical_word(board, word):
  top_y, bottom_y, x, offset = word[0].y, word[-1].y, word[0].x, 0
  while top_y > 0 and board[x][top_y-1] != ' ':
    offset -= 1
    top_y -= 1
  while bottom_y < BOARD_SZ-1 and board[x][bottom_y+1] != ' ':
    bottom_y += 1
  vertical_word = []
  for index, i in enumerate(range(top_y, bottom_y+1)):
    if board[x][i] != ' ':
      vertical_word.append(Letter(board[x][i], x, i, is_on_board=True))
    else:
      vertical_word.append(copy.copy(word[offset+index]))
  return vertical_word

def get_all_newly_formed_seq(board, word):
    x_disp, y_disp = word[0].x-word[-1].x, word[0].y-word[-1].y
    is_vertical_word, is_horizontal_word = y_disp != 0, x_disp != 0
    assert((is_vertical_word ^ is_horizontal_word) == 1)

    newly_formed_seqs = []
    if is_vertical_word:
        newly_formed_seqs.append(get_whole_vertical_word(board, word))
        for letter in word:
            x, y = letter.x, letter.y
            if board[x][y] == ' ':
                newly_formed_seqs.append(get_whole_horizontal_word(board, [letter]))
    elif is_horizontal_word:
        newly_formed_seqs.append(get_whole_horizontal_word(board, word))
        for letter in word:
            x, y = letter.x, letter.y
            if board[x][y] == ' ':
                newly_formed_seqs.append(get_whole_vertical_word(board, [letter]))
    return [x for x in newly_formed_seqs if len(x) > 1]
  
def causes_other_invalid_words(board, word):
    for seqs in get_all_newly_formed_seq(board, word):
        cur_seq_str = "".join([str(x) for x in seqs])
        if cur_seq_str not in valid_words:
            return True
    return False

def score_individual_word(board, config, word):
    multipliers = []
    base_score = 0
    n_hand_words_used = 0
    for letter in word:
        x, y = letter.x, letter.y
        letter_base_points = letter.point_value
        if config[x][y] == DW:
            multipliers.append(2)
        if config[x][y] == TW:
            multipliers.append(3)
        if config[x][y] == DL:
            letter_base_points *= 2
        if config[x][y] == TL:
            letter_base_points *= 3
        base_score += letter_base_points
        n_hand_words_used += int(board[x][y] == ' ')
    return reduce(mul, multipliers, 1) * base_score + int(n_hand_words_used == 7) * 35

def score(board, config, word):
    total_score = 0
    for seq in get_all_newly_formed_seq(board, word):
        assert("".join([str(x) for x in seq]) in valid_words)
        total_score += score_individual_word(board, config, seq)
    return total_score

######################################################################
# DEBUG/PRINTS/HELPERS
######################################################################
def print_word_candidate(word_candidate):
    print("Score: " + str(word_candidate[1]))
    for detail in [x.details() for x in word_candidate[0]]:
        print(detail)
    print('--------------------------------------------------------------')
    print("All newly formed words:")
    formed_seqs = get_all_newly_formed_seq(board, word_candidate[0])
    for seq in formed_seqs:
        print("".join([str(x) for x in seq]))
    print('--------------------------------------------------------------')

def candidate_contains_wildcard(word_candidate):
    for letter in word_candidate:
        if letter.is_wildcard:
            return True
    return False
    
if __name__ == "__main__":
    board = read_board("scrabble_board.txt")
    config =  read_board_config("scrabble_board_config.txt")
    hand = read_hand("scrabble_hand.txt")
    update_score_multipliers(board, config)
    word_candidates = generate_word_candidates(board, config, hand)
    if len(word_candidates) == 0:
        print("No possible words can be made.")
        sys.exit(0)
    print('--------------------------------------------------------------')
    print(word_candidates[:50])
    print('--------------------------------------------------------------')
    top_candidate_no_wildcard = None
    top_candidate = word_candidates[0]
    for word_candidate in word_candidates:
        if not candidate_contains_wildcard(word_candidate[0]):
            top_candidate_no_wildcard = word_candidate
            break
    if top_candidate_no_wildcard != top_candidate:
        print_word_candidate(top_candidate_no_wildcard)
    print_word_candidate(top_candidate)
    
    
