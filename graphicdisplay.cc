#include "graphicdisplay.h"
#include "game.h"
#include "board.h"
#include "player.h"
#include "link.h"
#include "cell.h"
#include <sstream>
#include <algorithm>

GraphicDisplay::GraphicDisplay(int gridSize, int width, int height)
    : xw{width, height}, gridSize{gridSize} {
    cellWidth = width / gridSize;
    cellHeight = (height - 200) / gridSize; // 62.5 is the hardcoded value
    lastDrawn.assign(gridSize, std::vector<char>(gridSize, ' '));
}

// Helper: choose text color (white if background is dark)
int textColorForBackground(int bg) {
    if (bg == Xwindow::Black || bg == Xwindow::Orange || bg == Xwindow::Blue) {
        return Xwindow::White;
    }
    return Xwindow::Black;
}

void GraphicDisplay::drawCell(int r, int c, const Cell &cell, const Player *p1) const {
    int x = c * cellWidth;
    // Shift grid down by 100 pixels to leave space for text
    int offsetY = 100;
    int y = offsetY + r * cellHeight;

    int bgColor = Xwindow::White;
    std::string text = "";
    int textColor = Xwindow::Black;

    if (!cell.isEmpty()) {
        Link *link = cell.getLink();
        Player *owner = link->getOwner();
        bool visible = (owner == p1 || link->isRevealed());

        // Set background color based on ownership and type
        if (link->getType() == LinkType::Data && visible) bgColor = Xwindow::Green;
        else if (link->getType() == LinkType::Virus && visible) bgColor = Xwindow::Red;
        else bgColor = Xwindow::Black;
        
        // if (visible) {
        //     bgColor = (link->getType() == LinkType::Data) ? Xwindow::Green : Xwindow::Red;
            text = std::string(1, link->getId());
        // } else {
        //     // Hidden link
        //     bgColor = Xwindow::Black;
        //     text = "?";
        // }

        xw.fillRectangle(x, y, cellWidth, cellHeight, bgColor);

        // Choose text color so it’s visible
        textColor = textColorForBackground(bgColor);
        xw.drawString(x + cellWidth/3, y + 2*cellHeight/3, text, textColor);

        // Draw boost indicator
        if (link->isBoosted()) {
            xw.drawString(x + 2*cellWidth/3, y + 2*cellHeight/3, "*", textColor);
        }
    } else {
        if (cell.getType() == CellType::ServerPort) {
            bgColor = Xwindow::Blue;
            xw.fillRectangle(x, y, cellWidth, cellHeight, bgColor);
            textColor = textColorForBackground(bgColor);
            xw.drawString(x + cellWidth/3, y + 2*cellHeight/3, "S", textColor);
        } else if (cell.getType() == CellType::Firewall) {
            bgColor = Xwindow::Orange;
            xw.fillRectangle(x, y, cellWidth, cellHeight, bgColor);
            textColor = textColorForBackground(bgColor);
            char fwChar = (cell.getOwnerId() == 1) ? 'm' : 'w';
            xw.drawString(x + cellWidth/3, y + 2*cellHeight/3,
                          std::string(1, fwChar), textColor);
        } else {
            xw.fillRectangle(x, y, cellWidth, cellHeight, bgColor);
        }
    }
}

void GraphicDisplay::print(const Game &game, std::ostream &out) const {
    const auto &board = *game.getBoard();
    const auto &players = game.getPlayers();
    const Player *p1 = players[0];
    const Player *p2 = players[1];

    int lineHeight = 15;
    int infoHeightTop = 70;
    int boardOffsetY = infoHeightTop;

    // ---------- 1. Build Player 1 info as a single string ----------
    std::ostringstream p1Stream;
    p1Stream << "Player 1:\n";
    p1Stream << "Downloaded: " << p1->getDownloadedData()
             << "D, " << p1->getDownloadedVirus() << "V\n";
    p1Stream << "Abilities: " << p1->getNumUnusedAbilities() << "\n";

    for (char id = 'a'; id <= 'h'; ++id) {
        auto *link = p1->getLink(id);
        if (!link) continue;
        p1Stream << id << ": "
                 << (link->getType() == LinkType::Data ? "D" : "V")
                 << link->getStrength() << " ";
    }
    std::string newP1Info = p1Stream.str();

    // ---------- 2. Only redraw if info changed ----------
    if (newP1Info != lastP1Info) {
        lastP1Info = newP1Info;

        // Clear background
        xw.fillRectangle(0, 0, gridSize * cellWidth, infoHeightTop, Xwindow::White);

        // Draw lines
        std::istringstream iss(newP1Info);
        std::string line;
        int y = 15;
        while (std::getline(iss, line)) {
            xw.drawString(10, y, line);
            y += lineHeight;
        }
    }

    // ---------- 3. Draw the board ----------
    for (int r = 0; r < gridSize; ++r) {
        for (int c = 0; c < gridSize; ++c) {
            const Cell &cell = board.at(r, c);

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

            if (lastDrawn[r][c] != currentSymbol) {
                const_cast<GraphicDisplay*>(this)->lastDrawn[r][c] = currentSymbol;
                drawCell(r, c, cell, p1);
            }
        }
    }

    // ---------- 4. Build Player 2 info as a single string ----------
    std::ostringstream p2Stream;
    p2Stream << "Player 2:\n";
    p2Stream << "Downloaded: " << p2->getDownloadedData()
             << "D, " << p2->getDownloadedVirus() << "V\n";
    p2Stream << "Abilities: " << p2->getNumUnusedAbilities() << "\n";

    for (char id = 'A'; id <= 'H'; ++id) {
        auto *link = p2->getLink(id);
        if (!link) continue;
        if (link->isRevealed()) {
            p2Stream << id << ": "
                     << (link->getType() == LinkType::Data ? "D" : "V")
                     << link->getStrength() << " ";
        } else {
            p2Stream << id << ": ? ";
        }
    }
    std::string newP2Info = p2Stream.str();

    // ---------- 5. Redraw Player 2 info only if changed ----------
    int p2StartAreaY = boardOffsetY + gridSize * cellHeight + 30;
    int p2AreaHeight = 70;
    if (newP2Info != lastP2Info) {
        lastP2Info = newP2Info;

        // Clear background
        xw.fillRectangle(0, p2StartAreaY, gridSize * cellWidth, p2AreaHeight, Xwindow::White);

        std::istringstream iss2(newP2Info);
        std::string line2;
        int y2 = p2StartAreaY + 15;
        while (std::getline(iss2, line2)) {
            xw.drawString(10, y2, line2);
            y2 += lineHeight;
        }
    }
}
