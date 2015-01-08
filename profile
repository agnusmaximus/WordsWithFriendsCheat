True
-Generating possible words...
-Filtering words...
-Scoring words...
--------------------------------------------------------------
[([u, n, b, e, l, t], 24), ([u, n, b, o, l, t], 24), ([a, b, o, u, t], 19), ([l, u, b, e], 18), ([b, l, u, e], 18), ([u, n, b, a, r], 18), ([a, b, o, r, t], 16), ([s, o, l, u, b, l, e], 16), ([s, o, l, u, b, l, y], 16), ([o, u, t, b, y], 16), ([b, u, t, t], 16), ([b, r, u, t], 16), ([t, u, b, e], 16), ([b, u, t, e], 16), ([r, u, b, e], 16), ([u, n, b, e], 16), ([s, n, u, b], 16), ([n, u, t, l, e, t], 16), ([n, u, b], 16), ([n, u, b, s], 16), ([n, u, m, b], 16), ([a, b, e, t], 15), ([b, l, a, t], 15), ([b, l, e, t], 15), ([b, l, o, t], 15), ([b, u, n, t], 15), ([b, u, s, t], 15), ([b, u, t, t], 15), ([b, e, l, t], 15), ([b, h, u, t], 15), ([b, o, l, t], 15), ([b, o, u, t], 15), ([b, r, u, t], 15), ([o, u, t, l, e, t], 14), ([o, r, b, i, t], 14), ([b, a, t, t], 14), ([b, i, t, t], 14), ([b, o, t, t], 14), ([b, o, r, t], 14), ([b, u, t, t], 14), ([b, r, a, t], 14), ([b, r, i, t], 14), ([b, r, u, t], 14), ([l, o, b, e], 14), ([l, u, b, e], 14), ([b, l, a, e], 14), ([b, l, u, e], 14), ([b, u, t, e], 14), ([b, a, l, e], 14), ([b, i, l, e], 14)]
--------------------------------------------------------------
Best Score: 24
u x:8 y:14 pt-val:2
n x:9 y:14 pt-val:2 on_board=True
b x:10 y:14 pt-val:4
e x:11 y:14 pt-val:0 wildcard=True
l x:12 y:14 pt-val:2
t x:13 y:14 pt-val:1
--------------------------------------------------------------
All newly formed words:
unbelt
--------------------------------------------------------------
         7249633 function calls (5468922 primitive calls) in 9.216 seconds

   Ordered by: internal time

   ncalls  tottime  percall  cumtime  percall filename:lineno(function)
916053/61    2.273    0.000    3.774    0.062 scrabble_cheat.py:208(form_vertical_words)
864780/61    2.158    0.000    3.599    0.059 scrabble_cheat.py:163(form_horizontal_words)
  1780834    1.791    0.000    1.791    0.000 {method 'has_keys_with_prefix' of 'marisa_trie._Trie' objects}
   906680    0.836    0.000    0.836    0.000 scrabble_cheat.py:30(__init__)
        1    0.388    0.388    0.491    0.491 scrabble_cheat.py:18(init_trie)
   367344    0.343    0.000    0.713    0.000 scrabble_cheat.py:37(__copy__)
   367344    0.250    0.000    1.303    0.000 copy.py:66(copy)
   367344    0.220    0.000    1.008    0.000 copy.py:123(_copy_inst)
    20629    0.088    0.000    0.217    0.000 scrabble_cheat.py:294(get_whole_horizontal_word)
        1    0.086    0.086    8.027    8.027 scrabble_cheat.py:126(generate_possible_words)
    14616    0.086    0.000    0.320    0.000 scrabble_cheat.py:254(determine_vertical_placements)
     8983    0.080    0.000    0.535    0.000 scrabble_cheat.py:346(causes_other_invalid_words)
   367344    0.075    0.000    0.075    0.000 {hasattr}
    16181    0.070    0.000    0.200    0.000 scrabble_cheat.py:311(get_whole_vertical_word)
    12952    0.069    0.000    0.249    0.000 scrabble_cheat.py:274(determine_horizontal_placements)
        1    0.060    0.060    8.713    8.713 scrabble_cheat.py:99(generate_word_candidates)
     9976    0.053    0.000    0.478    0.000 scrabble_cheat.py:326(get_all_newly_formed_seq)
   367344    0.045    0.000    0.045    0.000 {method 'get' of 'dict' objects}
        1    0.045    0.045    0.581    0.581 scrabble_cheat.py:143(filter_words)
   172886    0.040    0.000    0.040    0.000 {method 'lower' of 'str' objects}
   304076    0.040    0.000    0.040    0.000 {method 'append' of 'list' objects}
        1    0.034    0.034    0.034    0.034 {zip}
   172850    0.025    0.000    0.025    0.000 {method 'strip' of 'str' objects}
    36843    0.018    0.000    0.018    0.000 {range}
        1    0.011    0.011    9.216    9.216 scrabble_cheat.py:1(<module>)
    63327    0.009    0.000    0.009    0.000 scrabble_cheat.py:51(__str__)
    89884    0.008    0.000    0.008    0.000 {len}
      992    0.006    0.000    0.045    0.000 scrabble_cheat.py:372(score)
     1147    0.004    0.000    0.004    0.000 scrabble_cheat.py:353(score_individual_word)
    14639    0.003    0.000    0.003    0.000 {method 'join' of 'str' objects}
     1744    0.001    0.000    0.001    0.000 scrabble_cheat.py:40(__eq__)
        2    0.001    0.000    0.001    0.000 scrabble_cheat.py:73(read_board)
        1    0.000    0.000    0.001    0.001 {sorted}
     1147    0.000    0.000    0.000    0.000 {reduce}
        1    0.000    0.000    0.000    0.000 {method 'readline' of 'file' objects}
      225    0.000    0.000    0.000    0.000 scrabble_cheat.py:81(<lambda>)
      992    0.000    0.000    0.000    0.000 scrabble_cheat.py:108(<lambda>)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:112(accumulate_board_letters)
        4    0.000    0.000    0.000    0.000 {open}
      232    0.000    0.000    0.000    0.000 scrabble_cheat.py:66(get_scrabble_char)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:90(update_score_multipliers)
      218    0.000    0.000    0.000    0.000 scrabble_cheat.py:48(__repr__)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:83(read_hand)
        1    0.000    0.000    0.001    0.001 scrabble_cheat.py:80(read_board_config)
        6    0.000    0.000    0.000    0.000 scrabble_cheat.py:54(details)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:120(accumulate_hand_letters)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:29(Letter)
        1    0.000    0.000    0.000    0.000 {method 'disable' of '_lsprof.Profiler' objects}


