#ifndef __GRAPHICDISPLAY_H__
#define __GRAPHICDISPLAY_H__

#include "view.h"
#include "window.h"

class GraphicDisplay : public View {
  Xwindow xw;
  int gridSize;
  int cellWidth;
  int cellHeight;

public:
  GraphicDisplay(int n);

  // update the cell at (r, c) with new state (color)
  void notify(int r, int c, int change) override;

  // for graphical view override
  void print(std::ostream &out) override;
};

#endif
