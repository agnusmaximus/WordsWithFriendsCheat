-Generating possible words...
-Filtering words...
-Scoring words...
--------------------------------------------------------------
[([b, a, z, o, o, s], 60), ([b, a, z, o, o], 57), ([b, o, z, o, s], 57), ([b, o, z, o], 54), ([a, d, z], 39), ([b, o, g, u, s], 24), ([b, a, u, d], 21), ([g, a, u, d], 19), ([z, a, g, s], 17), ([s, h, o, c, k, s], 17), ([d, u, b], 16), ([z, a, g], 16), ([z, o, o, s], 15), ([s, a, w], 15), ([s, o, w], 15), ([s, u, b], 14), ([a, z, o], 14), ([z, o, a], 14), ([z, o, o], 14), ([z, o, a], 14), ([z, o, o], 14), ([u, d, o, s], 12), ([s, n, o, t], 12), ([b, u, d], 11), ([d, u, o], 10), ([b, u, s], 9), ([b, r, o, a, d], 9), ([e, d], 9), ([s, u, l, u, s], 8), ([u, l, u, s], 8), ([s, a, u], 8), ([s, o, u], 8), ([o, u, d], 8), ([u, s], 8), ([l, a], 8), ([l, o], 8), ([a, w], 8), ([o, w], 8), ([e, o, n, s], 7), ([o, n, s], 7), ([v, a, s], 7), ([o, b, i], 6), ([i, f, s], 6), ([e, s], 6), ([u, d, o], 5), ([e, s], 5), ([u, s], 5), ([r, o, a, d], 5), ([a, b], 5), ([n, a], 5)]
--------------------------------------------------------------
Score: 60
b x:12 y:1 pt-val:4
a x:12 y:2 pt-val:1
z x:12 y:3 pt-val:10 on_board=True
o x:12 y:4 pt-val:1
o x:12 y:5 pt-val:1
s x:12 y:6 pt-val:1
--------------------------------------------------------------
All newly formed words:
bazoos
--------------------------------------------------------------
         1425844 function calls (1115902 primitive calls) in 2.366 seconds

   Ordered by: internal time

   ncalls  tottime  percall  cumtime  percall filename:lineno(function)
        1    0.517    0.517    0.650    0.650 scrabble_cheat.py:17(init_trie)
153317/103    0.471    0.000    0.744    0.007 scrabble_cheat.py:186(form_horizontal_words)
156831/103    0.465    0.000    0.736    0.007 scrabble_cheat.py:230(form_vertical_words)
   310148    0.382    0.000    0.382    0.000 {method 'has_keys_with_prefix' of 'marisa_trie._Trie' objects}
    80257    0.097    0.000    0.097    0.000 scrabble_cheat.py:30(__init__)
    58442    0.058    0.000    0.130    0.000 scrabble_cheat.py:37(__copy__)
   172929    0.049    0.000    0.049    0.000 {method 'lower' of 'str' objects}
    58442    0.047    0.000    0.239    0.000 copy.py:66(copy)
        1    0.046    0.046    0.046    0.046 {zip}
    58442    0.041    0.000    0.185    0.000 copy.py:123(_copy_inst)
   172850    0.034    0.000    0.034    0.000 {method 'strip' of 'str' objects}
     2528    0.016    0.000    0.055    0.000 scrabble_cheat.py:300(determine_vertical_placements)
        1    0.014    0.014    1.705    1.705 scrabble_cheat.py:102(generate_word_candidates)
     2250    0.014    0.000    0.049    0.000 scrabble_cheat.py:320(determine_horizontal_placements)
    58442    0.013    0.000    0.013    0.000 {hasattr}
     2197    0.013    0.000    0.032    0.000 scrabble_cheat.py:340(get_whole_horizontal_word)
     1234    0.013    0.000    0.079    0.000 scrabble_cheat.py:392(causes_other_invalid_words)
     2136    0.011    0.000    0.028    0.000 scrabble_cheat.py:357(get_whole_vertical_word)
        1    0.010    0.010    2.366    2.366 scrabble_cheat.py:1(<module>)
    58442    0.008    0.000    0.008    0.000 {method 'get' of 'dict' objects}
     1329    0.007    0.000    0.068    0.000 scrabble_cheat.py:372(get_all_newly_formed_seq)
    46809    0.007    0.000    0.007    0.000 {method 'append' of 'list' objects}
        1    0.007    0.007    0.086    0.086 scrabble_cheat.py:166(filter_words)
        1    0.007    0.007    1.600    1.600 scrabble_cheat.py:150(generate_possible_words)
     4366    0.006    0.000    0.006    0.000 {range}
      103    0.005    0.000    0.797    0.008 scrabble_cheat.py:135(generate_all_vertical_words)
      103    0.004    0.000    0.797    0.008 scrabble_cheat.py:129(generate_all_horizontal_words)
     8755    0.001    0.000    0.001    0.000 scrabble_cheat.py:51(__str__)
    12314    0.001    0.000    0.001    0.000 {len}
       94    0.001    0.000    0.004    0.000 scrabble_cheat.py:418(score)
     1939    0.000    0.000    0.000    0.000 {method 'join' of 'str' objects}
        2    0.000    0.000    0.001    0.000 scrabble_cheat.py:73(read_board)
      126    0.000    0.000    0.000    0.000 scrabble_cheat.py:399(score_individual_word)
      225    0.000    0.000    0.000    0.000 scrabble_cheat.py:82(<lambda>)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:115(accumulate_board_letters)
        1    0.000    0.000    0.000    0.000 {method 'readline' of 'file' objects}
        4    0.000    0.000    0.000    0.000 {open}
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:93(update_score_multipliers)
      126    0.000    0.000    0.000    0.000 {reduce}
      145    0.000    0.000    0.000    0.000 scrabble_cheat.py:40(__eq__)
      232    0.000    0.000    0.000    0.000 scrabble_cheat.py:66(get_scrabble_char)
        1    0.000    0.000    0.000    0.000 {sorted}
        3    0.000    0.000    0.000    0.000 {method 'close' of 'file' objects}
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:428(print_word_candidate)
      165    0.000    0.000    0.000    0.000 scrabble_cheat.py:48(__repr__)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:84(read_hand)
       94    0.000    0.000    0.000    0.000 scrabble_cheat.py:111(<lambda>)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:123(accumulate_hand_letters)
        6    0.000    0.000    0.000    0.000 scrabble_cheat.py:54(details)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:29(Letter)
        1    0.000    0.000    0.001    0.001 scrabble_cheat.py:81(read_board_config)
        1    0.000    0.000    0.000    0.000 scrabble_cheat.py:439(candidate_contains_wildcard)
        1    0.000    0.000    0.000    0.000 {method 'disable' of '_lsprof.Profiler' objects}


