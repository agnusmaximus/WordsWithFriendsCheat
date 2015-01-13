#include <iostream>
#include <unordered_set>
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
#include <boost/tr1/functional.hpp>
#include <sys/time.h>
#include <thread>
#include <omp.h>
#include <valgrind/callgrind.h>

using namespace std;

#define DEBUG 1
#define DEBUG_PRINT(message) if (DEBUG) cout << message << " t=" << getTimeMillis()-baseTime << endl;

long int baseTime;
const static int NORM=1, DL=2, TL=3, DW=4, TW=5;
const static int N_LETTERS = 26;
const static char * DATASTRUCTURE_BINARY_FILE = "datastructures.bin";
const static int N_MAX_HAND_LETTERS = 7;
const static int BOARD_SZ = 15;
const static int BINGO_SCORE = 35;
const static int N_TOP_WORDS_DISPLAY = 5;

set<char> letterSet = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
map<string, set<char> > midstringPrefixLetters, startstringSuffixLetters;
set<string> validWords;
int pointValues[] = {1, 4, 4, 2, 1, 4, 3, 3, 1, 10, 5, 2, 4, 2, 1, 4, 10, 1, 1, 1, 2, 5, 4, 8, 3, 10};

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

bool wordSort(pair<vector<letter>, int> a, 
              pair<vector<letter>, int> b) {
  return a.second > b.second;
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

void printVectLetter(vector<letter> &letters) {
  for (int i = 0; i < letters.size(); i++) {
    cout << letters[i].character;
  }
  cout << endl;
}

void printVectLetterDescriptive(vector<letter> &letters) {
  for (int i = 0; i < letters.size(); i++) {
    cout << letters[i].character << " | x : " << letters[i].pos.y+1;
    cout << " y : " << letters[i].pos.x+1 << endl;
  }
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
  char multiplier;
  for (int i = 0; i < BOARD_SZ; i++) {
    for (int j = 0; j < BOARD_SZ; j++) {
      in >> multiplier;
      scoreMultipliers[i][j] = (int)(multiplier-'0');
    }
  }
  in.close();
}
 
void updateScoreMultipliers(char board[BOARD_SZ][BOARD_SZ], 
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

/***************************************************
 * SCORING                                         *
 ***************************************************/
int letterMultiplier(int multiplier[BOARD_SZ][BOARD_SZ], int x, int y) {
  switch (multiplier[x][y]) {
  case DL:
    return 2;
  case TL:
    return 3;
  }
  return 1;
}

int wordMultiplier(int multiplier[BOARD_SZ][BOARD_SZ], int x, int y) {
  switch (multiplier[x][y]) {
  case DW:
    return 2;
  case TW:
    return 3;
  }
  return 1;
}

int letterValue(char letter) {
  return pointValues[letter-'a'];
}

int scoreWord(char board[BOARD_SZ][BOARD_SZ], int multipliers[BOARD_SZ][BOARD_SZ],
              int x, int y, bool xdir, bool ydir) {
  if (ydir) {
    while (y > 0 && board[x][y-1] != '0') y--;
  }
  if (xdir) {
    while (x > 0 && board[x-1][y] != '0') x--;
  }
  int score = 0, wordScoreMultiplier = 1;
  if (ydir) {
    while (y < BOARD_SZ && board[x][y] != '0') {
      score += letterValue(board[x][y]) * letterMultiplier(multipliers, x, y);
      wordScoreMultiplier *= wordMultiplier(multipliers, x, y);
      y += 1;
    }
  }
  if (xdir) {
    while (x < BOARD_SZ && board[x][y] != '0') {
      score += letterValue(board[x][y]) * letterMultiplier(multipliers, x, y);
      wordScoreMultiplier *= wordMultiplier(multipliers, x, y);
      x += 1;
    }
  }
  return score * wordScoreMultiplier;  
}

void initVerticalCrossSections(char board[BOARD_SZ][BOARD_SZ], 
                               int multipliers[BOARD_SZ][BOARD_SZ], 
                               set<char> verticalCrossSections[BOARD_SZ][BOARD_SZ],
                               int verticalCrossSectionScores[BOARD_SZ][BOARD_SZ][N_LETTERS]) {
  for (int i = 0; i < BOARD_SZ; i++) {
    for (int j = 0; j < BOARD_SZ; j++) {
      verticalCrossSections[i][j] = letterSet;
      for (char k = 'a'; k <= 'z'; k++) {
        verticalCrossSectionScores[i][j][k] = 0;
      }
    }
  }
  
  for (int x = 0; x < BOARD_SZ; x++) {
    for (int y = 0; y < BOARD_SZ; y++) {
      if ((y+1 < BOARD_SZ && board[x][y+1] != '0' && board[x][y] == '0') || 
          (y-1 >= 0 && board[x][y-1] != '0' && board[x][y] == '0')) {
        for (char i = 'a'; i <= 'z'; i++) {
          board[x][y] = i;
          string crossSectionWord = getContiguousWord(board, x, y, false, true);
          if (validWords.find(crossSectionWord) == validWords.end()) {
            verticalCrossSections[x][y].erase(i);
            verticalCrossSectionScores[x][y][i-'a'] = 0;
          }
          else {
            verticalCrossSectionScores[x][y][i-'a'] = scoreWord(board, multipliers, x, y, false, true);
          }
          board[x][y] = '0';
        }
      }
    }
  }
}

void initHorizontalCrossSections(char board[BOARD_SZ][BOARD_SZ], 
                                 int multipliers[BOARD_SZ][BOARD_SZ],
                                 set<char> horizontalCrossSections[BOARD_SZ][BOARD_SZ],
                                 int horizontalCrossSectionScores[BOARD_SZ][BOARD_SZ][N_LETTERS]) {
  for (int i = 0; i < BOARD_SZ; i++) {
    for (int j = 0; j < BOARD_SZ; j++) {
      horizontalCrossSections[i][j] = letterSet;
      for (char k = 'a'; k <= 'z'; k++) {
        horizontalCrossSectionScores[i][j][k] = 0;
      }
    }
  }
  
  for (int y = 0; y < BOARD_SZ; y++) {
    for (int x = 0; x < BOARD_SZ; x++) {
      if ((x+1 < BOARD_SZ && board[x+1][y] != '0' && board[x][y] == '0') || 
          (x-1 >= 0 && board[x-1][y] != '0' && board[x][y] == '0')) {
        for (char i = 'a'; i <= 'z'; i++) {
          board[x][y] = i;
          string crossSectionWord = getContiguousWord(board, x, y, true, false);
          if (validWords.find(crossSectionWord) == validWords.end()) {
            horizontalCrossSections[x][y].erase(i);
          }
          else {
            horizontalCrossSectionScores[x][y][i-'a'] = scoreWord(board, multipliers, x, y, true, false);
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
bool visited(string &curString, position pos, int xdir, int ydir) {
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
                   multiset<char> &hand, int multipliers[BOARD_SZ][BOARD_SZ], int crossScores[BOARD_SZ][BOARD_SZ][N_LETTERS],
                   set<char> cross[BOARD_SZ][BOARD_SZ], bool didPlaceLetter,
                   int xdir, int ydir, vector<pair<vector<letter>, int> > &sol, 
                   int curScore, int curMultiplier, int addedScore) {
  string curString = wordAccumulate(partialSolution);
  
  if (visited(curString, pos, xdir, ydir)) {
    return;
  }
  if (validWords.find(curString) != validWords.end() && didPlaceLetter) {
    int totalScore = curScore * curMultiplier;
    if (hand.size() == 0) totalScore += BINGO_SCORE;
    totalScore += addedScore;
    sol.push_back(pair<vector<letter>, int>(partialSolution, totalScore));
  }
  if (pos.y >= BOARD_SZ) {
    return;
  }
  if (pos.x >= BOARD_SZ) {
    return;
  }
  
  if (board[pos.x][pos.y] == '0') {
    for (char l : hand) {
      if (startstringSuffixLetters[curString].find(l) != startstringSuffixLetters[curString].end() &&
          cross[pos.x][pos.y].find(l) != cross[pos.x][pos.y].end()) {
        multiset<char> newHand = hand;
        newHand.erase(newHand.find(l));
        partialSolution.push_back({l, {pos.x, pos.y}});
        extendForward(partialSolution, {pos.x+xdir, pos.y+ydir}, board, newHand, 
                      multipliers, crossScores, cross, didPlaceLetter, xdir, ydir, sol, 
                      curScore + letterValue(l) * letterMultiplier(multipliers, pos.x, pos.y),
                      curMultiplier * wordMultiplier(multipliers, pos.x, pos.y),
                      addedScore + crossScores[pos.x][pos.y][l-'a']);
        partialSolution.pop_back();
      }
    }
  }
  else {
    if (startstringSuffixLetters[curString].find(board[pos.x][pos.y]) != 
        startstringSuffixLetters[curString].end()) {
      partialSolution.push_back({board[pos.x][pos.y], {pos.x, pos.y}});
      extendForward(partialSolution, {pos.x+xdir, pos.y+ydir}, board, hand, 
                    multipliers, crossScores, cross, didPlaceLetter, xdir, ydir, sol, 
                    curScore + letterValue(board[pos.x][pos.y]),
                    curMultiplier,
                    addedScore);
      partialSolution.pop_back();
    }
  }
}

void extendBackward(vector<letter> &partialSolution, position pos, char board[BOARD_SZ][BOARD_SZ],
                    multiset<char> &hand, int multipliers[BOARD_SZ][BOARD_SZ], int limit, 
                    int crossScores[BOARD_SZ][BOARD_SZ][N_LETTERS], set<char> cross[BOARD_SZ][BOARD_SZ], bool didPlaceLetter, int xdir, int ydir, 
                    vector<pair<vector<letter>, int> > &sol, int curScore, int curMultiplier) {
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
    extendForward(partialSolution, {nextx, nexty}, board, hand, 
                  multipliers, crossScores, cross, didPlaceLetter,
                  xdir * -1, ydir * -1, sol, curScore, curMultiplier, 0);
  }
  if (limit > 0) {
    for (char l : hand) {
      if (midstringPrefixLetters.find(curString) != midstringPrefixLetters.end() &&
          midstringPrefixLetters[curString].find(l) != midstringPrefixLetters[curString].end() &&
          cross[pos.x][pos.y].find(l) != cross[pos.x][pos.y].end()) {
        multiset<char> newHand = hand;
        newHand.erase(newHand.find(l));
        board[pos.x][pos.y] = l;
        partialSolution.push_back({l, {pos.x, pos.y}});
        extendBackward(partialSolution, {pos.x+xdir, pos.y+ydir}, board, 
                       newHand, multipliers, limit-1, crossScores, cross, true, xdir, ydir, sol,
                       curScore + letterValue(l) * letterMultiplier(multipliers, pos.x, pos.y), 
                       curMultiplier * wordMultiplier(multipliers, pos.x, pos.y));
        partialSolution.pop_back();
        board[pos.x][pos.y] = '0';
      }
    }
  }
}

vector<pair<vector<letter>, int> > generateHorizontalWords(multiset<char> &hand, 
                                                           char board[BOARD_SZ][BOARD_SZ], 
                                                           int multipliers[BOARD_SZ][BOARD_SZ], 
                                                           position anchorPosition, set<char> crossSections[BOARD_SZ][BOARD_SZ],
                                                           int crossSectionScores[BOARD_SZ][BOARD_SZ][N_LETTERS],
                                                           vector<pair<vector<letter>, int> > &sol) {
  vector<pair<vector<letter>, int> > words;
  vector<letter> partials;

  int limitLeft = getNumNonAnchorPos(-1, 0, anchorPosition, board);

  if (limitLeft == 0 && anchorPosition.x-1 >= 0) {
    int i;
    for (i = anchorPosition.x; i > 0 && board[i-1][anchorPosition.y] != ' '; i--);
    extendForward(partials, (position){i, anchorPosition.y}, board, hand, 
                  multipliers, crossSectionScores, crossSections, true, 1, 0, sol, 0, 1, 0);
  }
  else {
    extendBackward(partials, (position){anchorPosition.x, anchorPosition.y}, 
                   board, hand, multipliers, limitLeft, crossSectionScores, crossSections, true, -1, 0, sol, 0, 1);
  }
  return words;
}

vector<pair<vector<letter>, int> > generateVerticalWords(multiset<char> &hand,
                                                         char board[BOARD_SZ][BOARD_SZ], 
                                                         int multipliers[BOARD_SZ][BOARD_SZ],
                                                         position anchorPosition, set<char> crossSections[BOARD_SZ][BOARD_SZ],
                                                         int crossSectionScores[BOARD_SZ][BOARD_SZ][N_LETTERS],
                                                         vector<pair<vector<letter>, int> > &sol) {
  vector<pair<vector<letter>, int> > words;
  vector<letter> partials;
  
  int limitUp = getNumNonAnchorPos(0, -1, anchorPosition, board);
  if (limitUp == 0 && anchorPosition.y-1 >= 0) {
    int i;
    for (i = anchorPosition.y; i > 0 && board[anchorPosition.x][i-1] != ' '; i--);
    extendForward(partials, (position){anchorPosition.x, i}, board, hand, 
                  multipliers, crossSectionScores, crossSections, true, 0, 1, sol, 0, 1, 0);
  }
  else {
    extendBackward(partials, (position){anchorPosition.x, anchorPosition.y}, 
                   board, hand, multipliers, limitUp, crossSectionScores, crossSections, true, 0, -1, sol, 0, 1);
  }
  return words;
}
 
vector<pair<vector<letter>, int> > generateValidWords(multiset<char> &hand, 
                                           char board[BOARD_SZ][BOARD_SZ],  
                                           int multipliers[BOARD_SZ][BOARD_SZ],
                                           set<char>  verticalCrossSections[BOARD_SZ][BOARD_SZ], 
                                           set<char>  horizontalCrossSection[BOARD_SZ][BOARD_SZ],
                                           int verticalCrossSectionScores[BOARD_SZ][BOARD_SZ][N_LETTERS],
                                           int horizontalCrossSectionScores[BOARD_SZ][BOARD_SZ][N_LETTERS]) {
  unordered_set<position, positionHash, positionCompare> anchorPositions = generateAnchorPositions(board);
  vector<pair<vector<letter>, int> > validWords;
  
  for (const position & anchorPosition : anchorPositions) {
    generateHorizontalWords(hand, board, multipliers, anchorPosition, 
                            verticalCrossSections, verticalCrossSectionScores,
                            validWords);
    generateVerticalWords(hand, board, multipliers, anchorPosition, 
                          horizontalCrossSection, horizontalCrossSectionScores,
                          validWords);
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
  int verticalCrossSectionScores[BOARD_SZ][BOARD_SZ][N_LETTERS];
  int horizontalCrossSectionScores[BOARD_SZ][BOARD_SZ][N_LETTERS];

  DEBUG_PRINT("Initializing Data Structures...");
  initDataStructures(dictFileName);

  DEBUG_PRINT("Initializing Board Properties...");
  initBoard(boardFileName, board);
  initHand(handFileName, hand);
  initScoreMultipliers(boardConfigFileName, scoreMultipliers);
  updateScoreMultipliers(board, scoreMultipliers);
  initVerticalCrossSections(board, scoreMultipliers, 
                            verticalCrossSections, verticalCrossSectionScores);
  initHorizontalCrossSections(board, scoreMultipliers, 
                              horizontalCrossSections, horizontalCrossSectionScores);

  DEBUG_PRINT("Generating Words...");
  CALLGRIND_START_INSTRUMENTATION;
  vector<pair<vector<letter>, int> > validWords = generateValidWords(hand, board, scoreMultipliers, 
                                                          verticalCrossSections,
                                                          horizontalCrossSections,
                                                          verticalCrossSectionScores,
                                                          horizontalCrossSectionScores);
  CALLGRIND_STOP_INSTRUMENTATION;
  CALLGRIND_DUMP_STATS;
  DEBUG_PRINT("Sorting Words...");
  sort(validWords.begin(), validWords.end(), wordSort);
  for (int i = 0; i < N_TOP_WORDS_DISPLAY; i++) {
    cout << "Score: " << validWords[i].second << endl;
    printVectLetterDescriptive(validWords[i].first);
  }
  DEBUG_PRINT("Done.");
}
