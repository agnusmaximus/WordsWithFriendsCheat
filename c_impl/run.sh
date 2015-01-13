if [ $# -eq 0 ]
then
    ./a.out ../scrabble_board.txt ../scrabble_board_config.txt ../scrabble_hand.txt ../dict.txt 
fi
if [ $# -eq 1 ]
then
    if [ "$1" == "--lldb" ]
    then
        lldb ./a.out ../scrabble_board.txt ../scrabble_board_config.txt ../scrabble_hand.txt ../dict.txt 
    fi
    if [ "$1" == "--callgrind" ]
    then
        valgrind --tool=callgrind --instr-atstart=no ./a.out ../scrabble_board.txt ../scrabble_board_config.txt ../scrabble_hand.txt ../dict.txt 
    fi
    if [ "$1" == "--valgrind" ]
    then
        valgrind --show-leak-kinds=all --leak-check=full ./a.out ../scrabble_board.txt ../scrabble_board_config.txt ../scrabble_hand.txt ../dict.txt 
    fi
fi
