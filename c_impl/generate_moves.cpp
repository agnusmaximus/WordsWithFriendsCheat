#include <iostream>
#include <set>
#include <unordered_set>
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
#include <boost/tr1/functional.hpp>
#include <sys/time.h>

using namespace std;

#define DEBUG 1
#define DEBUG_PRINT(message) if (DEBUG) cout << message << " t=" << getTimeMillis()-baseTime << endl;

long int baseTime;
const int NORM=1, DL=2, TL=3, DW=4, TW=5;
const char * DATASTRUCTURE_BINARY_FILE = "datastructures.bin";
const int N_MAX_HAND_LETTERS = 7;
const int BOARD_SZ = 15;

set<char> letterSet = {'a','b','c','d','e','f','g','h','i','j','k','l',
                                 'm','n','o','p','q','r','s','t','u','v','w','x','y','z'};
map<string, set<char> > midstringPrefixLetters, startstringSuffixLetters;
map<string, set<char> > validWordPrefixLetters, validWordSuffixLetters;
set<string> validWords;

struct position {
  int x, y;
};
typedef struct position positions;

struct positionCompare {
  bool operator() (const position &a, const position &b) const {
    return a.x == b.x && a.y == b.y;
  }
};

struct positionHash {
  size_t operator() (const position &a) const { 
    return a.x * 100 + a.y;
  }
};

struct letter {
  char character;
  position pos;
};
typedef struct letter letter;

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

void initHand(string handFileName, multiset<char> &hand) {
  ifstream in(handFileName.c_str());
  char curLetter;
  while (in >> curLetter) {
    hand.insert(curLetter);
  }
  in.close();
}

void initScoreMultipliers(string boardConfigFileName, 
                          int scoreMultipliers[BOARD_SZ][BOARD_SZ]) {
  ifstream in(boardConfigFileName.c_str());
  int multiplier = -1;
  for (int i = 0; i < BOARD_SZ; i++) {
    for (int j = 0; j < BOARD_SZ; j++) {
      in >> multiplier;
      scoreMultipliers[i][j] = multiplier-'0';
    }
  }
  in.close();
}
 
void updateScoreMultiplieres(char board[BOARD_SZ][BOARD_SZ], 
                             int scoreMultipliers[BOARD_SZ][BOARD_SZ]) {
  for (int i = 0; i < BOARD_SZ; i++) {
    for (int j = 0; j < BOARD_SZ; j++) {
      if (board[i][j] != '0') {
        scoreMultipliers[i][j] = NORM;
      }
    }
  }
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
 * SEARCH                                          *
 ***************************************************/
unordered_set<position, 
              positionHash, 
              positionCompare> generateAnchorPositions(char board[BOARD_SZ][BOARD_SZ]) {
  unordered_set<position, positionHash, positionCompare> anchorPositions;
  for (int i = 0; i < BOARD_SZ; i++) {
    for (int j = 0; j < BOARD_SZ; j++) {
      if (board[i][j] != '0') {
        for (int x = i-1; x <= i+1; x++) {
          for (int y = j-1; y <= j+1; y++) {
            if (x >= 0 && y >=0 &&
                x < BOARD_SZ && y < BOARD_SZ) {
              anchorPositions.insert((position){i, j});
            }
          }
        }
      }
    }
  }
  return anchorPositions;
}
 
set<vector<letter> > generateValidWords(char board[BOARD_SZ][BOARD_SZ], multiset<char> &hand, 
                                        set<char> verticalCrossSections[BOARD_SZ][BOARD_SZ], 
                                        set<char> horizontalCrossSection[BOARD_SZ][BOARD_SZ]) {
  unordered_set<position, positionHash, positionCompare> anchorPositions = generateAnchorPositions(board);
  set<vector<letter> > validWords;
  
  return validWords;
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

long int getTimeMillis() {
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

int main(int argc, char *argv[]) {
  baseTime = getTimeMillis();

  if (argc != 5) {
    printf("Usage: %s [board_file_name] [board_config_file_name] [hand_file_name] [dictionary_file_name]\n", argv[0]);
    return 0;
  }
  string boardFileName = argv[1];
  string boardConfigFileName = argv[2];
  string handFileName = argv[3];
  string dictFileName = argv[4];
  
  char board[BOARD_SZ][BOARD_SZ];
  int scoreMultipliers[BOARD_SZ][BOARD_SZ];
  multiset<char> hand;
  set<char> verticalCrossSections[BOARD_SZ][BOARD_SZ];
  set<char> horizontalCrossSections[BOARD_SZ][BOARD_SZ];
  
  DEBUG_PRINT("Initializing Data Structures...");
  initDataStructures(dictFileName);
  DEBUG_PRINT("Initializing Board Properties...");
  initBoard(boardFileName, board);
  initHand(handFileName, hand);
  initScoreMultipliers(boardConfigFileName, scoreMultipliers);
  updateScoreMultiplieres(board, scoreMultipliers);
  initVerticalCrossSections(board, verticalCrossSections);
  initHorizontalCrossSections(board, horizontalCrossSections);
  DEBUG_PRINT("Generating Words...");
  set<vector<letter> > validWords = generateValidWords(board, hand, 
                                                    verticalCrossSections,
                                                    horizontalCrossSections);
  DEBUG_PRINT("Done.");
}
