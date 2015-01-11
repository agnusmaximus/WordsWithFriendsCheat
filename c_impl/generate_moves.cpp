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
const static int NORM=1, DL=2, TL=3, DW=4, TW=5;
const static char * DATASTRUCTURE_BINARY_FILE = "datastructures.bin";
const static int N_MAX_HAND_LETTERS = 7;
const static int BOARD_SZ = 15;

set<char> letterSet = {'a','b','c','d','e','f','g','h','i','j','k','l',
                       'm','n','o','p','q','r','s','t','u','v','w','x','y','z'};
map<string, set<char> > midstringPrefixLetters, startstringSuffixLetters;
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
 * DEBUG OPERATIONS                                *
 ***************************************************/
void printCharSet(set<char> &charSet) {
  for(set<char>::iterator it=charSet.begin(); it != charSet.end(); it++) {
    cout << *it;
  }
  cout << endl;
}

void printVectLetter(vector<letter> &letters) {
  for (int i = 0; i < letters.size(); i++) {
    cout << letters[i].character;
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

/***************************************************
 * DATASTRUCTURE INITIALIZATION                    *
 ***************************************************/
void processDictionaryWord(string word) {
  validWords.insert(word);
  for (unsigned int i = 0; i < word.size(); i++) {
    for (unsigned int j = 0; j < word.size()-i+1; j++) {
      string substring = word.substr(i, j);
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
  validWords = set<string>();

  ifstream savedBinaryFile(DATASTRUCTURE_BINARY_FILE, ios_base::binary);
  if (savedBinaryFile.good()) {
    boost::archive::binary_iarchive iarch(savedBinaryFile);
    iarch >> midstringPrefixLetters >> startstringSuffixLetters;
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

string getContiguousWord(char board[BOARD_SZ][BOARD_SZ], int x, int y,
                         bool xdir, bool ydir) {
  if (ydir) {
    while (y > 0 && board[x][y-1] != '0') y--;
  }
  if (xdir) {
    while (x > 0 && board[x-1][y] != '0') x--;
  }
  string seq = "";
  if (ydir) {
    while (y < BOARD_SZ && board[x][y] != '0') {
      seq += board[x][y];
      y += 1;
    }
  }
  if (xdir) {
    while (x < BOARD_SZ && board[x][y] != '0') {
      seq += board[x][y];
      x += 1;
    }
  }
  return seq;
}

void initVerticalCrossSections(char board[BOARD_SZ][BOARD_SZ], 
                               set<char> verticalCrossSections[BOARD_SZ][BOARD_SZ]) {
  for (int i = 0; i < BOARD_SZ; i++) {
    for (int j = 0; j < BOARD_SZ; j++) {
      verticalCrossSections[i][j] = letterSet;
    }
  }
  
  for (int x = 0; x < BOARD_SZ; x++) {
    for (int y = 0; y < BOARD_SZ; y++) {
      if ((x+1 < BOARD_SZ && board[x+1][y] != '0' && board[x][y] == '0') || 
          (x-1 >= 0 && board[x-1][y] != '0' && board[x][y] == '0')) {
        for (char i = 'a'; i <= 'z'; i++) {
          board[x][y] = i;
          if (validWords.find(getContiguousWord(board, x, y, false, true)) == validWords.end()) {
            verticalCrossSections[x][y].erase(i);
          }
          board[x][y] = '0';
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
    for (int x = 0; x < BOARD_SZ; x++) {
      if ((y+1 < BOARD_SZ && board[x][y+1] != '0' && board[x][y] == '0') || 
          (y-1 >= 0 && board[x][y-1] != '0' && board[x][y] == '0')) {
        for (char i = 'a'; i <= 'z'; i++) {
          board[x][y] = i;
          if (validWords.find(getContiguousWord(board, x, y, true, false)) == validWords.end()) {
            horizontalCrossSections[x][y].erase(i);
          }
          board[x][y] = '0';
        }
      }
    }
  }
}

/***************************************************
 * HELPERS                                         *
 ***************************************************/
string wordAccumulate(vector<letter> &seq) {
  string res = "";
  for (int i = 0; i < seq.size(); i++) {
    res += seq[i].character;
  }
  return res;
}

/***************************************************
 * SEARCH                                          *
 ***************************************************/
bool visited(string curString, position pos, int xdir, int ydir) {
  static map<string, map<int, bool> > mem;
  xdir += 1;
  ydir += 1;
  int positionHash = pos.x * 100 + pos.y;
  int dirHash = xdir * 10 + ydir;
  int hash = positionHash * 100 + dirHash;
  bool res = false;
  if (mem.find(curString) != mem.end() &&
      mem[curString].find(hash) != mem[curString].end()) {
    res = mem[curString][hash];
  }
  else {
    mem[curString][hash] = true;
  }
  return res;
}

unordered_set<position, positionHash, positionCompare> generateAnchorPositions(char board[BOARD_SZ][BOARD_SZ]) {
  unordered_set<position, positionHash, positionCompare> anchorPositions;
  for (int i = 0; i < BOARD_SZ; i++) {
    for (int j = 0; j < BOARD_SZ; j++) {
      if (board[i][j] != '0') {
        for (int x = i-1; x <= i+1; x++) {
          for (int y = j-1; y <= j+1; y++) {
            if (x >= 0 && y >=0 &&
                x < BOARD_SZ && y < BOARD_SZ &&
                board[x][y] == '0') {
              anchorPositions.insert((position){x, y});
            }
          }
        }
      }
    }
  }
  return anchorPositions;
}

int getNumNonAnchorPos(int xdir, int ydir, position anchorPos, char board[BOARD_SZ][BOARD_SZ]) {
  int x = anchorPos.x+xdir, y = anchorPos.y+ydir, k = 0;
  while (x >= 0 && y >= 0) {
    for (int i = x-1; i <= x+1; i++) {
      for (int j = y-1; j <= y+1; j++) {
        if (i >= 0 && j >= 0 && i < BOARD_SZ && j < BOARD_SZ) {
          if (board[i][j] != '0') {
            return k;
          }
        }
      }
    }
    x += xdir;
    y += ydir;
    k += 1;
  }
  return k;
}

void extendForward(vector<letter> &partialSolution, position pos, char board[BOARD_SZ][BOARD_SZ],
                   multiset<char> &hand, set<char> cross[BOARD_SZ][BOARD_SZ], bool didPlaceLetter,
                   int xdir, int ydir, vector<vector<letter> > &sol) {
  string curString = wordAccumulate(partialSolution);
  
  if (visited(curString, pos, xdir, ydir)) {
    return;
  }
  if (validWords.find(curString) != validWords.end() && didPlaceLetter) {
    sol.push_back(partialSolution);
    if (curString.size() == 15) {
      cout << curString << endl;
    }
  }
  if (pos.y >= BOARD_SZ) {
    return;
  }
  if (pos.x >= BOARD_SZ) {
    return;
  }
  
  if (board[pos.x][pos.y] == '0') {
    for (char letter : hand) {
      if (startstringSuffixLetters[curString].find(letter) != startstringSuffixLetters[curString].end() &&
          cross[pos.x][pos.y].find(letter) != cross[pos.x][pos.y].end()) {
        multiset<char> newHand = hand;
        newHand.erase(newHand.find(letter));
        partialSolution.push_back({letter, {pos.x, pos.y}});
        extendForward(partialSolution, {pos.x+xdir, pos.y+ydir}, board, newHand, 
                      cross, didPlaceLetter, xdir, ydir, sol);
        partialSolution.pop_back();
      }
    }
  }
  else {
    if (startstringSuffixLetters[curString].find(board[pos.x][pos.y]) != 
        startstringSuffixLetters[curString].end()) {
      partialSolution.push_back({board[pos.x][pos.y], {pos.x, pos.y}});
      extendForward(partialSolution, {pos.x+xdir, pos.y+ydir}, board, hand, 
                    cross, didPlaceLetter, xdir, ydir, sol);
      partialSolution.pop_back();
    }
  }
}

void extendBackward(vector<letter> &partialSolution, position pos, char board[BOARD_SZ][BOARD_SZ],
                    multiset<char> &hand, int limit, set<char> cross[BOARD_SZ][BOARD_SZ],
                    bool didPlaceLetter, int xdir, int ydir, vector<vector<letter> > &sol) {
  string curString = wordAccumulate(partialSolution);
  if (visited(curString, pos, xdir, ydir)) {
    return;
  }
  if (startstringSuffixLetters.find(curString) != startstringSuffixLetters.end() &&
      startstringSuffixLetters[curString].size() != 0) {
    int nextx = pos.x, nexty = pos.y;
    if (ydir == -1) {
      nexty += partialSolution.size() + (1 ? didPlaceLetter : 0);
    }
    if (xdir == -1) {
      nextx += partialSolution.size() + (1 ? didPlaceLetter : 0);
    }
    extendForward(partialSolution, {nextx, nexty}, board, hand, cross, didPlaceLetter,
                  xdir * -1, ydir * -1, sol);
  }
  if (limit > 0) {
    for (char letter : hand) {
      if (midstringPrefixLetters.find(curString) != midstringPrefixLetters.end() &&
          midstringPrefixLetters[curString].find(letter) != midstringPrefixLetters[curString].end() &&
          cross[pos.x][pos.y].find(letter) != cross[pos.x][pos.y].end()) {
        multiset<char> newHand = hand;
        newHand.erase(newHand.find(letter));
        board[pos.x][pos.y] = letter;
        partialSolution.push_back({letter, {pos.x, pos.y}});
        extendBackward(partialSolution, {pos.x+xdir, pos.y+ydir}, board, 
                       newHand, limit-1, cross, true, xdir, ydir, sol);
        partialSolution.pop_back();
        board[pos.x][pos.y] = '0';
      }
    }
  }
}

vector<vector<letter> > generateHorizontalWords(char board[BOARD_SZ][BOARD_SZ], multiset<char> &hand,
                                                position anchorPosition, set<char> crossSections[BOARD_SZ][BOARD_SZ],
                                                vector<vector<letter> > &sol) {
  vector<vector<letter> > words;
  vector<letter> partials;
  int limitLeft = getNumNonAnchorPos(-1, 0, anchorPosition, board);

  if (limitLeft == 0 && anchorPosition.x-1 >= 0) {
    int i;
    for (i = anchorPosition.x; i > 0 && board[i-1][anchorPosition.y] != ' '; i--);
    extendForward(partials, (position){i, anchorPosition.y}, board, hand, 
                  crossSections, true, 1, 0, sol);
  }
  else {
    extendBackward(partials, (position){anchorPosition.x, anchorPosition.y}, 
                   board, hand, limitLeft, crossSections, true, -1, 0, sol);
  }
  return words;
}

vector<vector<letter> > generateVerticalWords(char board[BOARD_SZ][BOARD_SZ], multiset<char> &hand,
                                              position anchorPosition, set<char> crossSections[BOARD_SZ][BOARD_SZ],
                                              vector<vector<letter> > &sol) {
  vector<vector<letter> > words;
  vector<letter> partials;
  int limitUp = getNumNonAnchorPos(0, -1, anchorPosition, board);
  if (limitUp == 0 && anchorPosition.y-1 >= 0) {
    int i;
    for (i = anchorPosition.y; i > 0 && board[anchorPosition.x][i-1] != ' '; i--);
    extendForward(partials, (position){anchorPosition.x, i}, board, hand, 
                  crossSections, true, 0, 1, sol);
  }
  else {
    extendBackward(partials, (position){anchorPosition.x, anchorPosition.y}, 
                   board, hand, limitUp, crossSections, true, 0, -1, sol);
  }
  return words;
}
 
vector<vector<letter> > generateValidWords(char board[BOARD_SZ][BOARD_SZ], multiset<char> &hand, 
                                           set<char>  verticalCrossSections[BOARD_SZ][BOARD_SZ], 
                                           set<char>  horizontalCrossSection[BOARD_SZ][BOARD_SZ]) {
  unordered_set<position, positionHash, positionCompare> anchorPositions = generateAnchorPositions(board);
  vector<vector<letter> > validWords;
  
  for (const position & anchorPosition : anchorPositions) {
    generateHorizontalWords(board, hand, anchorPosition, verticalCrossSections, validWords);
    generateVerticalWords(board, hand, anchorPosition, horizontalCrossSection, validWords);
  }
  
  return validWords;
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
  vector<vector<letter> > validWords = generateValidWords(board, hand, 
                                                          verticalCrossSections,
                                                          horizontalCrossSections);
  DEBUG_PRINT("Done.");
}
