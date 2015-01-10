Loading aux data from files...
-Generating possible words...
-Scoring words...
--------------------------------------------------------------
[([i, c, e, b, o, x], 60), ([z, i, b, e, t], 37), ([s, o, a, p, b, o, x], 36), ([s, p, a, z], 35), ([s, y, b, o], 33), ([b, o, s, s], 33), ([s, o, z, i, n], 32), ([g, o, n, z, o], 32), ([a, z, o], 31), ([a, z, o, n], 31), ([a, z, o], 31), ([a, z, o, n], 31), ([s, i, z, e], 31), ([s, i, z, y], 31), ([s, i, z, e], 31), ([s, i, z, y], 31), ([s, p, a, z], 31), ([g, o, n, z, o], 31), ([b, o, s, s], 31), ([a, z, o], 30), ([s, e, x, p, o, t], 30), ([s, p, u, e, d], 29), ([p, o, x], 28), ([p, o, x, e, s], 28), ([p, o, x, e, d], 28), ([p, o, x, e, s], 28), ([p, o, x, e, d], 28), ([g, o, x, e, s], 28), ([a, x, o, n], 27), ([a, x], 26), ([a, x, e], 26), ([a, p, e, x], 26), ([a, x], 26), ([a, x, e], 26), ([a, p, e, x], 26), ([i, b, e, x], 26), ([s, o, x], 26), ([s, e, x, t, o], 26), ([s, i, x, m, o], 26), ([s, e, x, t, o], 26), ([s, i, x, m, o], 26), ([p, i, x], 26), ([p, i, x, y], 26), ([p, o, x], 26), ([p, i, x], 26), ([p, i, x, y], 26), ([p, o, x], 26), ([s, a, x], 25), ([s, e, x], 25), ([s, e, x, t], 25)]
--------------------------------------------------------------
Score: 37
z x:4 y:12 pt-val:10
i x:5 y:12 pt-val:1
b x:6 y:12 pt-val:4
e x:7 y:12 pt-val:1
t x:8 y:12 pt-val:1
--------------------------------------------------------------
All newly formed words:
zibet
--------------------------------------------------------------
Score: 60
i x:1 y:10 pt-val:1
c x:2 y:10 pt-val:0 wildcard=True
e x:3 y:10 pt-val:0 wildcard=True
b x:4 y:10 pt-val:4
o x:5 y:10 pt-val:1
x x:6 y:10 pt-val:8
--------------------------------------------------------------
All newly formed words:
icebox
--------------------------------------------------------------
         4006336 function calls (3313396 primitive calls) in 16.624 seconds

   Ordered by: internal time

   ncalls  tottime  percall  cumtime  percall filename:lineno(function)
        3   13.046    4.349   13.046    4.349 {cPickle.load}
456751/33836    1.298    0.000    1.722    0.000 scrabble_cheat.py:335(extend_down)
   804094    0.535    0.000    0.535    0.000 scrabble_cheat.py:74(__init__)
173873/9460    0.513    0.000    0.688    0.000 scrabble_cheat.py:267(extend_right)
 85247/13    0.404    0.000    2.189    0.168 scrabble_cheat.py:363(extend_up)
  1086187    0.160    0.000    0.160    0.000 {method 'get' of 'dict' objects}
   757004    0.103    0.000    0.112    0.000 {method 'join' of 'str' objects}
 20388/10    0.080    0.000    0.695    0.069 scrabble_cheat.py:295(extend_left)
    19903    0.070    0.000    0.175    0.000 scrabble_cheat.py:410(get_whole_vertical_word)
    20336    0.067    0.000    0.170    0.000 scrabble_cheat.py:395(get_whole_horizontal_word)
     7884    0.040    0.000    0.370    0.000 scrabble_cheat.py:471(score)
     7886    0.039    0.000    0.288    0.000 scrabble_cheat.py:425(get_all_newly_formed_seq)
    48586    0.034    0.000    0.146    0.000 copy.py:66(copy)
    48586    0.033    0.000    0.066    0.000 scrabble_cheat.py:80(__copy__)
    10863    0.032    0.000    0.035    0.000 scrabble_cheat.py:452(score_individual_word)
    48586    0.031    0.000    0.107    0.000 copy.py:123(_copy_inst)
    18260    0.022    0.000    0.022    0.000 {method 'intersection' of 'set' objects}
   139731    0.018    0.000    0.018    0.000 {method 'append' of 'list' objects}
    40335    0.014    0.000    0.014    0.000 {range}
        1    0.013    0.013    0.082    0.082 scrabble_cheat.py:233(generate_horizontal_cross_section)
        1    0.013    0.013    0.081    0.081 scrabble_cheat.py:207(generate_vertical_cross_section)
        1    0.012    0.012    3.573    3.573 scrabble_cheat.py:143(generate_word_candidates)
    48586    0.010    0.000    0.010    0.000 {hasattr}
    36353    0.006    0.000    0.006    0.000 scrabble_cheat.py:94(__str__)
        1    0.004    0.004   16.624   16.624 scrabble_cheat.py:1(<module>)
        1    0.003    0.003    0.004    0.004 {sorted}
    30393    0.003    0.000    0.003    0.000 {len}
    10863    0.002    0.000    0.002    0.000 {reduce}
    14196    0.002    0.000    0.002    0.000 scrabble_cheat.py:218(<genexpr>)
    13702    0.002    0.000    0.002    0.000 scrabble_cheat.py:225(<genexpr>)
    13598    0.002    0.000    0.002    0.000 scrabble_cheat.py:251(<genexpr>)
     6791    0.002    0.000    0.002    0.000 scrabble_cheat.py:83(__eq__)
    13702    0.002    0.000    0.002    0.000 scrabble_cheat.py:244(<genexpr>)
     4011    0.001    0.000    0.002    0.000 scrabble_cheat.py:332(add_legal_move)
     3873    0.001    0.000    0.002    0.000 scrabble_cheat.py:264(add_legal_move)
     6721    0.001    0.000    0.001    0.000 {method 'add' of 'set' objects}
     7884    0.001    0.000    0.001    0.000 scrabble_cheat.py:150(<lambda>)
        2    0.001    0.000    0.001    0.000 scrabble_cheat.py:114(read_board)
        1    0.000    0.000    3.184    3.184 scrabble_cheat.py:168(generate_possible_words)
       78    0.000    0.000    2.232    0.029 scrabble_cheat.py:327(generate_vertical_words)
       78    0.000    0.000    0.789    0.010 scrabble_cheat.py:259(generate_horizontal_words)
      156    0.000    0.000    0.000    0.000 scrabble_cheat.py:198(get_num_non_anchor_pos)
      225    0.000    0.000    0.000    0.000 scrabble_cheat.py:123(<lambda>)
        6    0.000    0.000    0.000    0.000 {open}
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:184(get_anchor_positions)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:154(accumulate_board_letters)
        1    0.000    0.000   13.047   13.047 scrabble_cheat.py:47(init_aux_data)
      232    0.000    0.000    0.000    0.000 scrabble_cheat.py:107(get_scrabble_char)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:134(update_score_multipliers)
        2    0.000    0.000    0.000    0.000 scrabble_cheat.py:481(print_word_candidate)
      204    0.000    0.000    0.000    0.000 scrabble_cheat.py:91(__repr__)
        3    0.000    0.000    0.000    0.000 {method 'close' of 'file' objects}
        3    0.000    0.000    0.000    0.000 {posix.stat}
      107    0.000    0.000    0.000    0.000 {method 'lower' of 'str' objects}
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:73(Letter)
       11    0.000    0.000    0.000    0.000 scrabble_cheat.py:97(details)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:125(read_hand)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:122(read_board_config)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:162(accumulate_hand_letters)
        3    0.000    0.000    0.000    0.000 genericpath.py:15(exists)
        1    0.000    0.000    0.000    0.000 {method 'readline' of 'file' objects}
       31    0.000    0.000    0.000    0.000 {method 'strip' of 'str' objects}
        2    0.000    0.000    0.000    0.000 scrabble_cheat.py:492(candidate_contains_wildcard)
        1    0.000    0.000    0.000    0.000 <string>:1(<module>)
        1    0.000    0.000    0.000    0.000 {method 'disable' of '_lsprof.Profiler' objects}


