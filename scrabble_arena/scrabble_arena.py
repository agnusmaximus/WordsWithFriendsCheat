from __future__ import print_function
import sys
import scrabble_cheat
import scrabble_base_ai
import random

SCRABBLE_BOARD_FILE = "scrabble_board.txt"
SCRABBLE_CONFIG_FILE = "scrabble_board_config.txt"
PLAYER_1_FILE = "1.txt"
PLAYER_2_FILE = "2.txt"

letter_bag = list("a"*9 + "b"*2 + "c"*2 + "d"*5 + "e"*13 + "f"*2 + "g"*3 + "h"*4 + "i"*8 + \
                  "j"*1 + "k"*1 + "l"*4 + "m"*2 + "n"*5 + "o"*8 + "p"*2 + "q"*1 + "r"*6 + "s"*5 + \
                  "t"*7 + "u"*4 + "v"*2 + "w"*2 + "x"*1 + "y"*2 + "z"*1)

def clear_board_file():
    empty_line = '0'*15
    f = open(SCRABBLE_BOARD_FILE, "w")
    for i in range(15):
        print(empty_line, file=f)
    f.close()

def clear_player_hand_files():
    f1, f2 = open(PLAYER_1_FILE, "w"), open(PLAYER_2_FILE, "w")
    print("", file=f1)
    print("", file=f2)
    f1.close()
    f2.close()

def get_tiles(hand_file):
    f = open(hand_file, "r")
    tiles = f.readline().strip()
    f.close()
    return tiles

def draw_n_tiles(n_needed):
    n_needed = min(n_needed, len(letter_bag))
    letters = random.sample(letter_bag, n_needed)
    for letter in letters:
        letter_bag.remove(letter)
    return "".join(letters)

def refill_hands(hand_file):
    tiles = get_tiles(hand_file)
    n_tiles_needed = 7 - len(tiles)
    extra_tiles = draw_n_tiles(n_tiles_needed)
    tiles += extra_tiles
    f = open(hand_file, "w")
    print(tiles, file=f)
    f.close()

def print_board():
    f = open(SCRABBLE_BOARD_FILE, "r")
    for line in f:
        print(line.strip())
    f.close()
    
if __name__=="__main__":
    clear_board_file()
    clear_player_hand_files()

    p1 = scrabble_base_ai.scrabble_base_ai(SCRABBLE_BOARD_FILE, SCRABBLE_CONFIG_FILE, PLAYER_1_FILE)
    p2 = scrabble_base_ai.scrabble_base_ai(SCRABBLE_BOARD_FILE, SCRABBLE_CONFIG_FILE, PLAYER_2_FILE)
    p1_score, p2_score = 0, 0

    while len(letter_bag) != 0 or (len(get_tiles(PLAYER_1_FILE)) > 0 and len(get_tiles(PLAYER_2_FILE)) > 0):
        refill_hands(PLAYER_1_FILE)
        p1_move = p1.generate_move()
        p1_did_make_move = p1_move != None
        p1_score += p1.make_move(p1_move)
        refill_hands(PLAYER_2_FILE)
        p2_move = p2.generate_move()
        p2_did_make_move = p2_move != None
        p2_score += p2.make_move(p2_move)

        print("P1: " + str(p1_score) + " P2: " + str(p2_score))
        print_board()
        
        if not p1_did_make_move and not p2_did_make_move:
            break

    if p1_score > p2_score:
        print("P1 WINS")
    elif p1_score < p2_score:
        print("P2 WINS")
    else:
        print("TIE")
    
    
