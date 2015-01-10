#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <algorithm>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

using namespace std;

const char * DATASTRUCTURE_BINARY_FILE = "datastructures.bin";
const int N_MAX_HAND_LETTERS = 7;
const int BOARD_SZ = 15;

set<char> letterSet = {'a','b','c','d','e','f','g','h','i','j','k','l',
                       'm','n','o','p','q','r','s','t','u','v','w','x','y','z'};

map<string, set<char> > midstringPrefixLetters, startstringSuffixLetters;
map<string, set<char> > validWordPrefixLetters, validWordSuffixLetters;
set<string> validWords;

/***************************************************
 * DATASTRUCTURE INITIALIZATION                    *
 ***************************************************/
void processDictionaryWord(string word) {
  validWords.insert(word);
  for (unsigned int i = 0; i < word.size(); i++) {
    for (unsigned int j = 0; j < word.size()-i+1; j++) {
      string substring = word.substr(i, j);
      if (i == 1 && j == word.size()-1) 
        validWordPrefixLetters[substring].insert(word[0]);
      if (i == 0 && j == word.size()-1) 
        validWordSuffixLetters[substring].insert(word[word.size()-1]);
      if (i != 0) 
        midstringPrefixLetters[substring].insert(word[i-1]);
      if (i == 0 && j < word.size()) 
        startstringSuffixLetters[substring].insert(word[j]);
    }
  }
}

void initDataStructures(string dictFileName) {
  midstringPrefixLetters = map<string, set<char> >();
  startstringSuffixLetters = map<string, set<char> >();
  validWordPrefixLetters = map<string, set<char> >();
  validWordSuffixLetters = map<string, set<char> >();
  validWords = set<string>();

  ifstream savedBinaryFile(DATASTRUCTURE_BINARY_FILE, ios_base::binary);
  if (savedBinaryFile.good()) {
    boost::archive::binary_iarchive iarch(savedBinaryFile);
    iarch >> midstringPrefixLetters >> startstringSuffixLetters;
    iarch >> validWordPrefixLetters >> validWordSuffixLetters;
    iarch >> validWords;
  }
  else {
    ifstream in(dictFileName.c_str()); 
    string curWord;
    while (in >> curWord) {
      processDictionaryWord(curWord);
    }
    in.close();
    ofstream binaryFile(DATASTRUCTURE_BINARY_FILE, ios_base::binary);
    boost::archive::binary_oarchive oarch(binaryFile);
    oarch << midstringPrefixLetters << startstringSuffixLetters;
    oarch << validWordPrefixLetters << validWordSuffixLetters;
    oarch << validWords;
  }
  savedBinaryFile.close();
}

void initBoard(string boardFileName, char board[BOARD_SZ][BOARD_SZ]) {
  ifstream in(boardFileName.c_str());
  for (int i = 0; i < BOARD_SZ; i++) {
    for (int j = 0; j < BOARD_SZ; j++) {
      in >> board[i][j];
    }
  }
  in.close();
}

void initHand(string handFileName, set<char> &hand) {
  ifstream in(handFileName.c_str());
  char curLetter;
  while (in >> curLetter) {
    hand.insert(curLetter);
  }
  in.close();
}

void initVerticalCrossSections(char board[BOARD_SZ][BOARD_SZ], 
                               set<char> verticalCrossSections[BOARD_SZ][BOARD_SZ]) {
  for (int i = 0; i < BOARD_SZ; i++) {
    for (int j = 0; j < BOARD_SZ; j++) {
      verticalCrossSections[i][j] = letterSet;
    }
  }
  
  for (int x = 0; x < BOARD_SZ; x++) {
    string curString = "";
    for (int y = 0; y < BOARD_SZ; y++) {
      if (board[x][y] != '0') {
        curString += board[x][y];
      }
      else {
        if (curString != "") {
          int topIndex = y-curString.size() - 1;
          if (topIndex >= 0) {
            verticalCrossSections[x][topIndex] = validWordPrefixLetters[curString];
          }
          verticalCrossSections[x][y] = validWordSuffixLetters[curString];
          curString = "";
        }
      }
    }
  }
}


void initHorizontalCrossSections(char board[BOARD_SZ][BOARD_SZ], 
                                 set<char> horizontalCrossSections[BOARD_SZ][BOARD_SZ]) {
  for (int i = 0; i < BOARD_SZ; i++) {
    for (int j = 0; j < BOARD_SZ; j++) {
      horizontalCrossSections[i][j] = letterSet;
    }
  }
  
  for (int y = 0; y < BOARD_SZ; y++) {
    string curString = "";
    for (int x = 0; x < BOARD_SZ; x++) {
      if (board[x][y] != '0') {
        curString += board[x][y];
      }
      else {
        if (curString != "") {
          int topIndex = x-curString.size() - 1;
          if (topIndex >= 0) {
            horizontalCrossSections[topIndex][y] = validWordPrefixLetters[curString];
          }
          horizontalCrossSections[x][y] = validWordSuffixLetters[curString];
          curString = "";
        }
      }
    }
  }
}

/***************************************************
 * DEBUG OPERATIONS                                *
 ***************************************************/
void printCharSet(set<char> &charSet) {
  for(set<char>::iterator it=charSet.begin(); it != charSet.end(); it++) {
    cout << *it;
  }
  cout << endl;
}

void printBoard(char board[BOARD_SZ][BOARD_SZ]) {
  for (int i = 0; i < BOARD_SZ; i++) {
    for (int j = 0; j < BOARD_SZ; j++) {
      cout << board[i][j];
    }
    cout << endl;
  }
}

int main(int argc, char *argv[]) {
  if (argc != 5) {
    printf("Usage: %s [board_file_name] [board_config_file_name] [hand_file_name] [dictionary_file_name]\n", argv[0]);
    return 0;
  }
  string boardFileName = argv[1];
  string boardConfigFileName = argv[2];
  string handFileName = argv[3];
  string dictFileName = argv[4];
  
  char board[BOARD_SZ][BOARD_SZ];
  set<char> hand;
  set<char> verticalCrossSections[BOARD_SZ][BOARD_SZ];
  set<char> horizontalCrossSections[BOARD_SZ][BOARD_SZ];
  
  initDataStructures(dictFileName);
  initBoard(boardFileName, board);
  initHand(handFileName, hand);
  initVerticalCrossSections(board, verticalCrossSections);
  initHorizontalCrossSections(board, horizontalCrossSections);
  
  
}
