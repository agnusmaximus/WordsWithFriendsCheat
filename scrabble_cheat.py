import sys
import copy
from itertools import groupby
from operator import mul
from pytrie import StringTrie

# Score multipliers
BOARD_SZ = 15
(NORM, DL, TL, DW, TW) = (1, 2, 3, 4, 5)
POINT_VALS = {'a' : 1, 'b' : 4, 'c' : 4, 'd' : 2, 'e' : 1, 'f' : 4, 'g' : 3, 'h' : 3, 'i' : 1, \
              'j' : 10, 'k' : 5, 'l' : 2, 'm' : 4, 'n' : 2, 'o' : 1, 'p' : 4, 'q' : 10, 'r' : 1, \
              's' : 1, 't' : 1, 'u' : 2, 'v' : 5, 'w' : 4, 'x' : 8, 'y' : 3, 'z' : 10, '*' : 0}
trie = None
valid_words = None

######################################################################
# TRIE STUFF
######################################################################
def init_trie(dict_file_path):
    dict_file = open(dict_file_path, "r")
    words = set()
    for line in dict_file:
        line = line.strip().lower()
        words.add(line)
    word_pairings = dict(zip(words, range(len(words))))
    return (StringTrie(word_pairings), words)

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
    return board

def read_board_config(board_config_file_path):
    return read_board(board_config_file_path, lambda x: int(x))

def read_hand(scrabble_hand_file_path):
    f = open(scrabble_hand_file_path, "r")
    return [get_scrabble_char(x) for x in list(f.readline().strip())]

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
    print("-Filtering words...")
    possible_words = filter_words(board, possible_words)
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
    possible_words = []
    for board_letter in board_letters:
        """working_set = hand_letters + [board_letter]
        valid_words = words_from_character_set(working_set)
        vertical_placements = [determine_vertical_placements(x) for x in valid_words]
        vertical_placements = [x for x in vertical_placements if x]
        horizontal_placements = [determine_horizontal_placements(x) for x in valid_words]
        horizontal_placements = [x for x in horizontal_placements if x]
        possible_words += vertical_placements + horizontal_placements"""
        horizontal_words = form_horizontal_words(board, board_letter, hand_letters)
        horizontal_words = [determine_horizontal_placements(x) for x in horizontal_words]
        horizontal_words = [x for x in horizontal_words if x]
        vertical_words = form_vertical_words(board, board_letter, hand_letters)
        vertical_words = [determine_vertical_placements(x) for x in vertical_words]
        vertical_words = [x for x in vertical_words if x]
        possible_words += vertical_words
        possible_words += horizontal_words
    for word in possible_words:
        for character in word:
            assert(character.x >= 0 and character.y >= 0 and \
                   character.x < BOARD_SZ and character.y < BOARD_SZ)
    return possible_words

def filter_words(board, word_candidates):
    collision_filtered_words = []
    for word_candidate in word_candidates:
        is_valid_candidate = True
        did_all_overlap = True
        for letter in word_candidate:
            x, y = (letter.x, letter.y)
            if board[x][y] != ' ' and board[x][y] != letter.character:
                is_valid_candidate = False
                break
            if board[x][y] == ' ':
                did_all_overlap = False
        if is_valid_candidate and not did_all_overlap:
            collision_filtered_words.append(word_candidate)
    valid_words = []
    for word_candidate in collision_filtered_words:
        if not causes_other_invalid_words(board, word_candidate):
            valid_words.append(word_candidate)
    return valid_words

"""def words_from_character_set(char_set, cur_word_list=[], cur_word=""):
    sol = []
    if cur_word in valid_words:
        sol.append([copy.copy(x) for x in cur_word_list])
    if char_set == []:
        return sol
    if cur_word != "" and len(trie.keys(prefix=cur_word)) == 0:
        return sol
    for i, char in enumerate(char_set):
        if char.character == '*':
            for wildcard in list("abcdefghijklmnopqrstuvwxyz"):
                cur_word_list.append(Letter(wildcard, char.x, char.y, char.is_on_board, True))
                cur_word += wildcard
                new_char_set = char_set[:i] + char_set[i+1:]
                sol += words_from_character_set(new_char_set, cur_word_list, cur_word)
                cur_word = cur_word[:-1]
                cur_word_list.pop(-1)
        else:
            cur_word_list.append(char)
            cur_word += char.character
            new_char_set = char_set[:i] + char_set[i+1:]
            sol += words_from_character_set(new_char_set, cur_word_list, cur_word)
            cur_word = cur_word[:-1]
            cur_word_list.pop(-1)
    return sol"""

def form_horizontal_words(board, board_letter, hand, used_board_letter=False,
                          posx=-1, posy=-1, cur_word_list=[], cur_word=""):
    sol = []
    if cur_word != "" and len(trie.keys(prefix=cur_word)) == 0:
        return sol
    if cur_word in valid_words and used_board_letter:
        sol.append([copy.copy(x) for x in cur_word_list])
    if hand == []:
        if used_board_letter:
            return sol
        return []
    if posx >= BOARD_SZ:
        return sol
    if used_board_letter and board[posx][posy] != ' ':
        return form_horizontal_words(board, board_letter, 
                                     hand, used_board_letter,
                                     posx+1, posy, 
                                     cur_word_list + [Letter(board[posx][posy], posx, posy, is_on_board=True)],
                                     cur_word+board[posx][posy])
    incr = 1 if used_board_letter else 0
    for i, letter in enumerate(hand):
        new_hand = hand[:i] + hand[i+1:]
        partial_solution = []
        if letter.character == '*':
            for wildcard in list("abcdefghijklmnopqrstuvwxyz"):
                partial_solution += \
                form_horizontal_words(board, board_letter, new_hand,
                                      used_board_letter,
                                      posx+incr, posy,
                                      cur_word_list + [Letter(wildcard, -1, -1, wildcard=True)],
                                      cur_word + wildcard)
        else:
            partial_solution += form_horizontal_words(board, board_letter, new_hand,
                                                     used_board_letter,
                                                     posx+incr, posy, 
                                                     cur_word_list + [letter],
                                                     cur_word + letter.character)
        sol += partial_solution
    if not used_board_letter:
        sol += form_horizontal_words(board, board_letter, hand, True,
                                     board_letter.x+1, board_letter.y,
                                     cur_word_list + [board_letter],
                                     cur_word + board_letter.character)
    return sol

def form_vertical_words(board, board_letter, hand, used_board_letter=False,
                        posx=-1, posy=-1, cur_word_list=[], cur_word=""):
    sol = []
    if cur_word != "" and len(trie.keys(prefix=cur_word)) == 0:
        return []
    if cur_word in valid_words and used_board_letter:
        sol.append([copy.copy(x) for x in cur_word_list])
    if hand == []:
        if used_board_letter:
            return sol
        return []
    if posy >= BOARD_SZ:
        return sol
    if used_board_letter and board[posx][posy] != ' ':
        return form_vertical_words(board, board_letter, 
                                     hand, used_board_letter,
                                     posx, posy+1, 
                                     cur_word_list + [Letter(board[posx][posy], posx, posy, is_on_board=True)],
                                     cur_word+board[posx][posy])
    incr = 1 if used_board_letter else 0
    for i, letter in enumerate(hand):
        new_hand = hand[:i] + hand[i+1:]
        partial_solution = []
        if letter.character == '*':
            for wildcard in list("abcdefghijklmnopqrstuvwxyz"):
                partial_solution += \
                form_vertical_words(board, board_letter, new_hand,
                                    used_board_letter,
                                    posx, posy+incr,
                                    cur_word_list + [Letter(wildcard, -1, -1, wildcard=True)],
                                    cur_word + wildcard)
        else:
            partial_solution += form_vertical_words(board, board_letter, new_hand,
                                                    used_board_letter,
                                                    posx, posy+incr, 
                                                    cur_word_list + [letter],
                                                    cur_word + letter.character)
        sol += partial_solution
    if not used_board_letter:
        sol += form_vertical_words(board, board_letter, hand, True,
                                   board_letter.x, board_letter.y+1,
                                   cur_word_list + [board_letter],
                                   cur_word + board_letter.character)
    return sol

    
def determine_vertical_placements(letters):
    letter_on_board, index = None, -1
    for i, letter in enumerate(letters):
        if letter.x != -1 and letter.y != -1:
           letter_on_board, index = letter, i
           break
    if letter_on_board is None:
        return None
    placed_letters = []
    for i, letter in enumerate(letters):
        disp = i - index
        copied_letter = copy.copy(letter)
        copied_letter.x = letter_on_board.x
        copied_letter.y = letter_on_board.y + disp
        if copied_letter.x < 0 or copied_letter.y < 0 or \
          copied_letter.x >= BOARD_SZ or copied_letter.y >= BOARD_SZ:
          return None
        placed_letters.append(copied_letter)
    return placed_letters

def determine_horizontal_placements(letters):
    letter_on_board, index = None, -1
    for i, letter in enumerate(letters):
        if letter.x != -1 and letter.y != -1:
           letter_on_board, index = letter, i
           break
    if letter_on_board is None:
        return None
    placed_letters = []
    for i, letter in enumerate(letters):
        disp = i - index
        copied_letter = copy.copy(letter)
        copied_letter.y = letter_on_board.y
        copied_letter.x = letter_on_board.x + disp
        if copied_letter.x < 0 or copied_letter.y < 0 or \
          copied_letter.x >= BOARD_SZ or copied_letter.y >= BOARD_SZ:
          return None
        placed_letters.append(copied_letter)
    return placed_letters

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
      horizontal_word.append(Letter(board[i][y], i, y))
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
      vertical_word.append(Letter(board[x][i], x, i))
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
    
if __name__ == "__main__":
    board = read_board("scrabble_board.txt")
    config =  read_board_config("scrabble_board_config.txt")
    hand = read_hand("scrabble_hand.txt")
    trie, valid_words = init_trie("dict.txt")
    update_score_multipliers(board, config)
    word_candidates = generate_word_candidates(board, config, hand)
    print('--------------------------------------------------------------')
    print(word_candidates[:50])
    print('--------------------------------------------------------------')
    print("Best Score: " + str(word_candidates[0][1]))
    for detail in [x.details() for x in word_candidates[0][0]]:
        print(detail)
    print('--------------------------------------------------------------')
    print("All newly formed words:")
    formed_seqs = get_all_newly_formed_seq(board, word_candidates[0][0])
    for seq in formed_seqs:
        print("".join([str(x) for x in seq]))
    print('--------------------------------------------------------------')
    
