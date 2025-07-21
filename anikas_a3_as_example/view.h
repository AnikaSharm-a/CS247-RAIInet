#ifndef __DISPLAY_H__
#define __DISPLAY_H__
#include <iostream>

class View {
  public:

  /*
  * The Controller calls notify to
  * update the (r,c) location to be change
  */
  virtual void notify(int r, int c, int change) = 0;

  virtual void print(std::ostream &out) = 0;

  virtual ~View() = 0;
};

#endif
