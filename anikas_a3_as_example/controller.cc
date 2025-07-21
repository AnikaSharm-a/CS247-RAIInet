#include "controller.h"
#include "game.h"
#include "textdisplay.h"
#include "graphicdisplay.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// Controller ctor with useGraphics bool param
Controller::Controller(bool useGraphics): game{new Game()}, td{nullptr}, useGraphics{useGraphics} {}

Controller::~Controller(){
  delete game;
  delete td;
  delete gd; // for graphics display
}

/*
 * Update the View(s) to indicate the new game state
*/
void Controller::notify(int r, int c, int state){
  if (td) td->notify(r, c, state);
  if (useGraphics && gd) gd->notify(r, c, state); // notify graphics display if exists
}

/*
 * Called when command init is received
*/
void Controller::init(istream & input, Game & g){
  int r, c, s;
  while (input >> r >> c) {
    if (r == -1 && c == -1) break; // end of input
    if (!(input >> s)) break; // if -1,-1 or other command inputs
    g.init(r, c, s);
  }
}

bool Controller::checkWin(int movesLeft){
  if(game->isWon() && movesLeft != 0) {
    cout << "Won" << endl;
    return true;
  }
  return false;
}

void Controller::play(){
  int moves = 0;
  string cmd;
  bool playing = false;
  while (cin >> cmd) {
    if (cmd == "new") {
      int n;
      cin >> n;
      game->init(n, this);
      delete td;
      td = new TextDisplay{n};
      // Graphics check and init
      if (useGraphics) {
        delete gd;
        gd = new GraphicDisplay{n};
      }
      moves = 0;
    }
    else if (cmd == "?"){
      cout << "White: 0" << endl;
      cout << "Black: 1" << endl;
      cout << "Red:   2" << endl;
      cout << "Green: 3" << endl;
      cout << "Blue:  4" << endl;
    }
    else if (cmd == "init") {
      init(cin, *game);
      td->print(cout);
      if (checkWin(moves)) {
        break;
      }
    }
    else if (cmd == "include") {
       string file;
       cin >> file;
       ifstream input(file.c_str());
       init(input, *game);
       td->print(cout);
       if (checkWin(moves)) {
        break;
      }
    }
    else if (cmd == "switch") {
      if (checkWin(moves)) {
        break; 
      }
      if(moves > 0){
        int next;
        if (cin >> next && (next >= 0 || next <= 4)) {
          game->change(next);
          td->print(cout);
          --moves;
          if (moves == 1) {
            cout << moves << " move left" << endl;
          } else {
            cout << moves << " moves left" << endl;
          }
          if (game->isWon()) {
            cout << "Won" << endl;
            break;
          }
          if (moves == 0) {
            cout << "Lost" << endl;
            break;
          }
        }
        else cerr << "Invalid move" << endl;
      }
    }
    else if (cmd == "game" && !playing) {
      cin >> moves;
      if (moves < 0) {
        moves = 0;
      }
      else if (moves == 1) {
        cout << moves << " move left" << endl;
      }
      else {
        cout << moves << " moves left" << endl;
      }
      if (checkWin(moves)) {
        break;
      }
    }
  }
}
