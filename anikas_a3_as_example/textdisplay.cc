#include "textdisplay.h"
#include <iostream>
using namespace std;

TextDisplay::TextDisplay(int n): theDisplay(n, vector<int>(n, 0)) {}

void TextDisplay::notify(int r, int c, int change) {
    theDisplay[r][c] = change; // change the colour of that cell
}

void TextDisplay::print(ostream &out) {
    int n = theDisplay.size();
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            out << theDisplay[r][c]; // print the cell's colour
        }
        out << endl;
    }
}
