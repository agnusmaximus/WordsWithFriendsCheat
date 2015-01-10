Loading aux data from files...
-Generating possible words...
-Scoring words...
--------------------------------------------------------------
[([d, e, c, a, f], 33), ([b, e, a, d], 31), ([c, a, f, e], 27), ([d, e, a, f], 27), ([f, a, c, e], 27), ([b, e, a, d], 26), ([c, a, b], 25), ([c, a, g, e], 25), ([g, a, e, d], 24), ([b, a, d, e], 23), ([b, a, g], 23), ([c, a, d, e], 23), ([d, a, c, e], 23), ([f, a, d, e], 23), ([f, a, g], 23), ([g, a, b], 23), ([e, g, a, d], 23), ([a, c, e], 21), ([b, a, d], 21), ([c, a, d], 21), ([d, a, b], 21), ([f, a, d], 21), ([g, a, e, d], 21), ([g, a, e], 19), ([g, a, e], 19), ([a, g, e], 19), ([a, c, e], 19), ([d, a, g], 19), ([g, a, d], 19), ([a, c, e], 18), ([c, a, v, e], 18), ([f, a, v, e], 18), ([b, a], 17), ([b, e], 17), ([f, a], 17), ([b, a], 17), ([b, e], 17), ([f, a], 17), ([a, b], 17), ([a, g, e], 17), ([a, b], 17), ([a, g, e], 17), ([g, a, e], 17), ([g, a, v, e], 17), ([a, g, e], 16), ([a, g], 15), ([a, b], 15), ([e, f], 15), ([c, a, b], 15), ([b, a, l, e], 14)]
--------------------------------------------------------------
Score: 33
d x:2 y:2 pt-val:2
e x:3 y:2 pt-val:1
c x:4 y:2 pt-val:4
a x:5 y:2 pt-val:1
f x:6 y:2 pt-val:4
--------------------------------------------------------------
All newly formed words:
decaf
aged
--------------------------------------------------------------
         247878 function calls (245630 primitive calls) in 2.948 seconds

   Ordered by: internal time

   ncalls  tottime  percall  cumtime  percall filename:lineno(function)
        3    2.749    0.916    2.749    0.916 {marshal.load}
    61116    0.039    0.000    0.039    0.000 scrabble_cheat.py:74(__init__)
     5439    0.029    0.000    0.056    0.000 scrabble_cheat.py:397(get_whole_horizontal_word)
     5389    0.028    0.000    0.054    0.000 scrabble_cheat.py:412(get_whole_vertical_word)
    13583    0.017    0.000    0.026    0.000 {method 'join' of 'str' objects}
        1    0.013    0.013    0.081    0.081 scrabble_cheat.py:207(generate_vertical_cross_section)
        1    0.013    0.013    0.083    0.083 scrabble_cheat.py:233(generate_horizontal_cross_section)
        2    0.009    0.004    0.009    0.005 scrabble_cheat.py:114(read_board)
    49471    0.006    0.000    0.006    0.000 {method 'append' of 'list' objects}
   969/17    0.004    0.000    0.010    0.001 scrabble_cheat.py:364(extend_up)
 1473/612    0.004    0.000    0.006    0.000 scrabble_cheat.py:336(extend_down)
    10924    0.004    0.000    0.004    0.000 {range}
     4536    0.003    0.000    0.015    0.000 copy.py:66(copy)
     4536    0.003    0.000    0.006    0.000 scrabble_cheat.py:80(__copy__)
     4536    0.003    0.000    0.011    0.000 copy.py:123(_copy_inst)
    14456    0.002    0.000    0.002    0.000 scrabble_cheat.py:225(<genexpr>)
    14898    0.002    0.000    0.002    0.000 scrabble_cheat.py:251(<genexpr>)
    14664    0.002    0.000    0.002    0.000 scrabble_cheat.py:244(<genexpr>)
    13494    0.002    0.000    0.002    0.000 scrabble_cheat.py:218(<genexpr>)
  494/106    0.002    0.000    0.003    0.000 scrabble_cheat.py:267(extend_right)
    11405    0.002    0.000    0.002    0.000 {method 'get' of 'dict' objects}
     6990    0.001    0.000    0.001    0.000 {method 'add' of 'set' objects}
      187    0.001    0.000    0.008    0.000 scrabble_cheat.py:473(score)
     4536    0.001    0.000    0.001    0.000 {hasattr}
      343    0.001    0.000    0.001    0.000 scrabble_cheat.py:454(score_individual_word)
        1    0.001    0.001    2.948    2.948 scrabble_cheat.py:1(<module>)
      188    0.001    0.000    0.006    0.000 scrabble_cheat.py:427(get_all_newly_formed_seq)
        1    0.000    0.000    0.000    0.000 {method 'readline' of 'file' objects}
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:184(get_anchor_positions)
        1    0.000    0.000    0.188    0.188 scrabble_cheat.py:143(generate_word_candidates)
    58/11    0.000    0.000    0.001    0.000 scrabble_cheat.py:295(extend_left)
      225    0.000    0.000    0.000    0.000 scrabble_cheat.py:123(<lambda>)
       76    0.000    0.000    0.003    0.000 scrabble_cheat.py:259(generate_horizontal_words)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:154(accumulate_board_letters)
       76    0.000    0.000    0.012    0.000 scrabble_cheat.py:328(generate_vertical_words)
     1007    0.000    0.000    0.000    0.000 scrabble_cheat.py:94(__str__)
        1    0.000    0.000    0.179    0.179 scrabble_cheat.py:168(generate_possible_words)
        6    0.000    0.000    0.000    0.000 {open}
     1212    0.000    0.000    0.000    0.000 {len}
      152    0.000    0.000    0.000    0.000 scrabble_cheat.py:198(get_num_non_anchor_pos)
      232    0.000    0.000    0.000    0.000 scrabble_cheat.py:107(get_scrabble_char)
      343    0.000    0.000    0.000    0.000 {reduce}
        1    0.000    0.000    0.000    0.000 {sorted}
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:134(update_score_multipliers)
        1    0.000    0.000    2.749    2.749 scrabble_cheat.py:47(init_aux_data)
      134    0.000    0.000    0.000    0.000 scrabble_cheat.py:333(add_legal_move)
        3    0.000    0.000    0.000    0.000 {method 'close' of 'file' objects}
      154    0.000    0.000    0.000    0.000 scrabble_cheat.py:83(__eq__)
        1    0.000    0.000    0.001    0.001 scrabble_cheat.py:125(read_hand)
      110    0.000    0.000    0.000    0.000 {method 'lower' of 'str' objects}
       53    0.000    0.000    0.000    0.000 scrabble_cheat.py:264(add_legal_move)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:483(print_word_candidate)
      158    0.000    0.000    0.000    0.000 scrabble_cheat.py:91(__repr__)
      187    0.000    0.000    0.000    0.000 scrabble_cheat.py:150(<lambda>)
        3    0.000    0.000    0.000    0.000 {posix.stat}
       31    0.000    0.000    0.000    0.000 {method 'strip' of 'str' objects}
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:162(accumulate_hand_letters)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:73(Letter)
        3    0.000    0.000    0.000    0.000 genericpath.py:15(exists)
        5    0.000    0.000    0.000    0.000 scrabble_cheat.py:97(details)
        1    0.000    0.000    0.001    0.001 scrabble_cheat.py:122(read_board_config)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:494(candidate_contains_wildcard)
        1    0.000    0.000    0.000    0.000 {method 'disable' of '_lsprof.Profiler' objects}


