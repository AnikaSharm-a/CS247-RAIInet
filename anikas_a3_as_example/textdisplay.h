#ifndef __TEXTDISPLAY_H__
#define __TEXTDISPLAY_H__
#include <iostream>
#include <vector>
#include "view.h"

class TextDisplay : public View {
  std::vector<std::vector<int>> theDisplay;

 public:
  TextDisplay(int n); //one arg constructor where the parameter is the gridSize

  /*
   * The Controller calls notify to
   * update the (r,c) location to be change
   */
  void notify(int r, int c, int change) override;

  /*
   * Prints the grid as specified in the assignment specification.
   */
  void print(std::ostream &out);
};

#endif
