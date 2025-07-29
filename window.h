#ifndef __WINDOW_H__
#define __WINDOW_H__
#include <X11/Xlib.h>
#include <iostream>
#include <string>
using namespace std;

class Xwindow {
  Display *d;
  Window w;
  int s, width, height;
  GC gc;
  unsigned long colours[7];

 public:
  Xwindow(int width=500, int height=500);  // Constructor; displays the window.
  ~Xwindow();                              // Destructor; destroys the window.

  enum {White=0, Black, Red, Green, Blue, Orange, Purple}; // Available colours.

  int getWidth() const;
  int getHeight() const;

  // Draws a rectangle
  void fillRectangle(int x, int y, int width, int height, int colour=Black);

  // Draws a string
  void drawString(int x, int y, string msg);
  void drawString(int x, int y, string msg, int colour);

  void drawLine(int x1, int y1, int x2, int y2, int colour = Black);

};

#endif
