#include "textdisplay.h"

TextDisplay::TextDisplay(int gridSize) {
    theDisplay.resize(gridSize, std::vector<int>(gridSize, 0));
}

void TextDisplay::notify(int row, int col, CellType state) {
    // Encode CellType into int: 0=Normal,1=Firewall,2=ServerPort
    int val = 0;
    if (state == CellType::Firewall) val = 1;
    else if (state == CellType::ServerPort) val = 2;

    theDisplay[row][col] = val;
}

void TextDisplay::print(std::ostream &out) const {
    for (int r = 0; r < (int)theDisplay.size(); ++r) {
        for (int c = 0; c < (int)theDisplay[r].size(); ++c) {
            char ch = '.';
            switch (theDisplay[r][c]) {
                case 0: ch = '.'; break;
                case 1: ch = 'F'; break;
                case 2: ch = 'S'; break;
            }
            out << ch;
        }
        out << "\n";
    }
}
