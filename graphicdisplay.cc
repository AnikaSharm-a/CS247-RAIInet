#include "graphicdisplay.h"
#include "game.h"
#include "board.h"
#include "player.h"
#include "link.h"
#include "cell.h"

GraphicDisplay::GraphicDisplay(int gridSize, int width, int height)
    : xw{width, height}, gridSize{gridSize} {
    cellWidth = width / gridSize;
    cellHeight = height / gridSize;
    lastDrawn.assign(gridSize, std::vector<char>(gridSize, ' '));
}

// void GraphicDisplay::notify(int row, int col, CellType state) {
//     // This could be hooked to an observer if you want incremental updates
// }

void GraphicDisplay::drawCell(int r, int c, const Cell &cell, const Player *p1) const {
    int x = c * cellWidth;
    int y = r * cellHeight;

    if (!cell.isEmpty()) {
        Link *link = cell.getLink();
        Player *owner = link->getOwner();

        int color = Xwindow::Black; // default for unknown
        if (owner == p1) {
            color = (link->getType() == LinkType::Data) ? Xwindow::Green : Xwindow::Red;
        } else if (link->isRevealed()) {
            color = (link->getType() == LinkType::Data) ? Xwindow::Green : Xwindow::Red;
        } else {
            color = Xwindow::Black;
        }

        xw.fillRectangle(x, y, cellWidth, cellHeight, color);

        // Draw ID on top
        std::string idStr(1, link->getId());
        xw.drawString(x + cellWidth/3, y + 2*cellHeight/3, idStr);
        
        // Draw boost indicator if link is boosted
        if (link->isBoosted()) {
            xw.drawString(x + 2*cellWidth/3, y + 2*cellHeight/3, "*");
        }
    } else {
        if (cell.getType() == CellType::ServerPort) {
            xw.fillRectangle(x, y, cellWidth, cellHeight, Xwindow::Blue);
        } else if (cell.getType() == CellType::Firewall) {
            xw.fillRectangle(x, y, cellWidth, cellHeight, Xwindow::Black);
        } else {
            xw.fillRectangle(x, y, cellWidth, cellHeight, Xwindow::White);
        }
    }
}

void GraphicDisplay::print(const Game &game, std::ostream &out) const {
    const auto &board = *game.getBoard();
    const auto &players = game.getPlayers();
    const Player *p1 = players[0];
    const Player *p2 = players[1];

    // Compare and update only changed cells
    for (int r = 0; r < gridSize; ++r) {
        for (int c = 0; c < gridSize; ++c) {
            const Cell &cell = board.at(r, c);

            // Represent cell as a char for comparison
            char currentSymbol = '.';
            if (!cell.isEmpty()) {
                Link *link = cell.getLink();
                Player *owner = link->getOwner();
                if (owner == p1 || link->isRevealed())
                    currentSymbol = link->getId();
                else
                    currentSymbol = '?';
            } else if (cell.getType() == CellType::ServerPort) currentSymbol = 'S';
            else if (cell.getType() == CellType::Firewall) currentSymbol = 'F';
            else currentSymbol = '.';

            // Only redraw if the cell content has changed
            if (lastDrawn[r][c] != currentSymbol) {
                const_cast<GraphicDisplay*>(this)->lastDrawn[r][c] = currentSymbol;
                drawCell(r, c, cell, p1);
            }
        }
    }

    // Draw player info
    int textY = gridSize * cellHeight + 20;
    xw.drawString(10, textY, "P1: D" + std::to_string(p1->getDownloadedData()) +
                              " V" + std::to_string(p1->getDownloadedVirus()) +
                              " Abilities:" + std::to_string(p1->getNumUnusedAbilities()));
    xw.drawString(10, textY + 20, "P2: D" + std::to_string(p2->getDownloadedData()) +
                                  " V" + std::to_string(p2->getDownloadedVirus()) +
                                  " Abilities:" + std::to_string(p2->getNumUnusedAbilities()));
}
