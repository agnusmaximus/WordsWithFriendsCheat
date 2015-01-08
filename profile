-Generating possible words...
-Filtering words...
-Scoring words...
--------------------------------------------------------------
[([r, e, a, m, s], 22), ([r, o, a, m, s], 22), ([t, e, r, m, s], 22), ([t, e, e, m, s], 22), ([t, e, a, m, s], 22), ([t, r, a, m, s], 22), ([t, e, e, m, s], 22), ([t, e, r, m, s], 22), ([t, e, a, m, s], 22), ([a, t, o, m, s], 22), ([r, e, m, s], 21), ([r, a, m, s], 21), ([r, o, m, s], 21), ([t, a, m, s], 21), ([t, o, m, s], 21), ([a, r, m, s], 21), ([s, o, u, t, e, r], 20), ([e, m, s], 20), ([o, m, s], 20), ([o, u, t, e, r], 19), ([a, e, r, a, t, e], 18), ([r, o, u, t, e], 17), ([s, a, u, t, e], 17), ([r, a, t, o, s], 17), ([a, u, t, o], 16), ([w, a, l, l, o, w, e, r], 16), ([a, x, e], 16), ([l, o, w, e, r], 16), ([y, e, a, r], 15), ([y, e, a, s], 15), ([y, e, a, r], 15), ([y, e, a, s], 15), ([w, a, l, l, o, w, s], 15), ([a, l, l, o, w, s], 15), ([l, o, w, s], 15), ([r, o, t, a, s], 15), ([t, o, r, a, s], 15), ([s, e, t, a, e], 15), ([s, t, o, a, e], 15), ([s, e, t, a, e], 15), ([a, r, e, a, s], 15), ([a, r, e, a, e], 15), ([a, r, e, a, s], 15), ([t, o, r, s, o], 15), ([u, t, a], 14), ([t, a, r, e, s], 14), ([t, a, r, o, s], 14), ([t, a, r, e, s], 14), ([t, a, r, o, s], 14), ([r, e, s, e, a, u], 14)]
--------------------------------------------------------------
Score: 22
r x:14 y:1 pt-val:1
e x:14 y:2 pt-val:1
a x:14 y:3 pt-val:1
m x:14 y:4 pt-val:4 on_board=True
s x:14 y:5 pt-val:1
--------------------------------------------------------------
All newly formed words:
reams
fivers
--------------------------------------------------------------
         2395841 function calls (2076150 primitive calls) in 2.519 seconds

   Ordered by: internal time

   ncalls  tottime  percall  cumtime  percall filename:lineno(function)
161449/49    0.412    0.000    0.748    0.015 scrabble_cheat.py:207(form_vertical_words)
158340/49    0.405    0.000    0.659    0.013 scrabble_cheat.py:163(form_horizontal_words)
        1    0.370    0.370    0.474    0.474 scrabble_cheat.py:18(init_trie)
   319789    0.337    0.000    0.337    0.000 {method 'has_keys_with_prefix' of 'marisa_trie._Trie' objects}
   163924    0.156    0.000    0.156    0.000 scrabble_cheat.py:30(__init__)
   132570    0.111    0.000    0.240    0.000 scrabble_cheat.py:37(__copy__)
   132570    0.086    0.000    0.446    0.000 copy.py:66(copy)
   132570    0.077    0.000    0.343    0.000 copy.py:123(_copy_inst)
    13009    0.049    0.000    0.133    0.000 scrabble_cheat.py:294(get_whole_horizontal_word)
    12933    0.043    0.000    0.127    0.000 scrabble_cheat.py:311(get_whole_vertical_word)
     4724    0.043    0.000    0.289    0.000 scrabble_cheat.py:346(causes_other_invalid_words)
   172875    0.042    0.000    0.042    0.000 {method 'lower' of 'str' objects}
     6175    0.037    0.000    0.302    0.000 scrabble_cheat.py:326(get_all_newly_formed_seq)
        1    0.033    0.033    0.033    0.033 {zip}
   126455    0.030    0.000    0.030    0.000 {method 'pop' of 'list' objects}
   126455    0.030    0.000    0.030    0.000 {method 'insert' of 'list' objects}
     4887    0.028    0.000    0.103    0.000 scrabble_cheat.py:254(determine_vertical_placements)
        1    0.027    0.027    1.624    1.624 scrabble_cheat.py:126(generate_possible_words)
   132570    0.026    0.000    0.026    0.000 {hasattr}
   172850    0.026    0.000    0.026    0.000 {method 'strip' of 'str' objects}
     4005    0.023    0.000    0.086    0.000 scrabble_cheat.py:274(determine_horizontal_placements)
        1    0.021    0.021    2.036    2.036 scrabble_cheat.py:99(generate_word_candidates)
        1    0.019    0.019    0.309    0.309 scrabble_cheat.py:143(filter_words)
   143657    0.019    0.000    0.019    0.000 {method 'append' of 'list' objects}
   132570    0.016    0.000    0.016    0.000 {method 'get' of 'dict' objects}
    25975    0.013    0.000    0.013    0.000 {range}
     1450    0.010    0.000    0.081    0.000 scrabble_cheat.py:372(score)
        1    0.008    0.008    2.519    2.519 scrabble_cheat.py:1(<module>)
     1679    0.006    0.000    0.007    0.000 scrabble_cheat.py:353(score_individual_word)
    39030    0.006    0.000    0.006    0.000 scrabble_cheat.py:51(__str__)
    56490    0.005    0.000    0.005    0.000 {len}
     9251    0.002    0.000    0.002    0.000 {method 'join' of 'str' objects}
     3736    0.002    0.000    0.002    0.000 scrabble_cheat.py:40(__eq__)
        1    0.001    0.001    0.001    0.001 {sorted}
     1679    0.000    0.000    0.000    0.000 {reduce}
        2    0.000    0.000    0.000    0.000 scrabble_cheat.py:73(read_board)
      225    0.000    0.000    0.000    0.000 scrabble_cheat.py:81(<lambda>)
     1450    0.000    0.000    0.000    0.000 scrabble_cheat.py:108(<lambda>)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:112(accumulate_board_letters)
      239    0.000    0.000    0.000    0.000 scrabble_cheat.py:48(__repr__)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:90(update_score_multipliers)
        4    0.000    0.000    0.000    0.000 {open}
      232    0.000    0.000    0.000    0.000 scrabble_cheat.py:66(get_scrabble_char)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:382(print_word_candidate)
        5    0.000    0.000    0.000    0.000 scrabble_cheat.py:54(details)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:120(accumulate_hand_letters)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:83(read_hand)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:80(read_board_config)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:29(Letter)
        1    0.000    0.000    0.000    0.000 {method 'readline' of 'file' objects}
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:393(candidate_contains_wildcard)
        1    0.000    0.000    0.000    0.000 {method 'disable' of '_lsprof.Profiler' objects}


