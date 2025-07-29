#include "graphicdisplay.h"
#include "game.h"
#include "board.h"
#include "player.h"
#include "link.h"
#include "cell.h"
#include <sstream>
#include <algorithm>
using namespace std;

GraphicDisplay::GraphicDisplay(int gridSize, int width, int height) : xw{width, height}, gridSize{gridSize} {
    cellWidth = (width - 10) / gridSize;
    cellHeight = (height - 200) / gridSize; // 62.5 is the hardcoded value
    lastDrawn.assign(gridSize, vector<DrawnState>(gridSize, DrawnState{' ', false, false, LinkType::Data, false}));

    // Board setup    
    int offsetX = 10;
    int offsetY = 100;
    int boardPixelSize = gridSize * cellWidth;
    int endY = offsetY + gridSize * cellHeight;

    // Draw vertical gridlines
    for (int i = 0; i <= gridSize; ++i) {
        int x = i * cellWidth;
        xw.drawLine(x + offsetX, offsetY, x + offsetX, endY);
    }

    // Draw horizontal gridlines
    for (int i = 0; i <= gridSize; ++i) {
        int y = offsetY + i * cellHeight;
        xw.drawLine(offsetX, y, boardPixelSize + offsetX, y);
    }

    // Draw column numbers at the top
    for (int c = 0; c < gridSize; ++c) {
        int x = c * cellWidth + cellWidth / 2;
        xw.drawString(x, offsetY - 5, to_string(c));
    }

    // Draw row numbers at the left
    for (int r = 0; r < gridSize; ++r) {
        int y = offsetY + r * cellHeight + cellHeight / 2;
        xw.drawString(3, y, to_string(r));
    }
}

// Helper: choose text color (white if background is dark)
int textColorForBackground(int bg) {
    if (bg == Xwindow::Black || bg == Xwindow::Orange || bg == Xwindow::Blue) {
        return Xwindow::White;
    }
    return Xwindow::Black;
}

void GraphicDisplay::drawCell(int r, int c, const Cell &cell, const Player *p1) const {
    int x = c * cellWidth + 10;
    int offsetY = 100;
    int y = offsetY + r * cellHeight;

    int bgColor = Xwindow::White;
    string text = "";
    int textColor = Xwindow::Black;

    // --- 1. Base background: empty cell ---
    xw.fillRectangle(x + margin, y + margin, cellWidth - 2*margin, cellHeight - 2*margin, Xwindow::White);

    // --- 2. Draw special cell background (firewall/server) ---
    if (cell.getType() == CellType::ServerPort) {
        bgColor = Xwindow::Blue;
        xw.fillRectangle(x + margin, y + margin, cellWidth - 2*margin, cellHeight - 2*margin, bgColor);
        textColor = textColorForBackground(bgColor);
        xw.drawString(x + cellWidth/3, y + 2*cellHeight/3, "S", textColor);
    } 
    else if (cell.getType() == CellType::Firewall) {
        bgColor = Xwindow::Orange;
        xw.fillRectangle(x + margin, y + margin, cellWidth - 2*margin, cellHeight - 2*margin, bgColor);
        textColor = textColorForBackground(bgColor);
        char fwChar = (cell.getOwnerId() == 1) ? 'm' : 'w';
        xw.drawString(x + cellWidth/3, y + 2*cellHeight/3, string(1, fwChar), textColor);
    }

    // --- 3. If there is a link, draw it ON TOP of whatever background was drawn ---
    if (!cell.isEmpty()) {
        Link *link = cell.getLink();
        Player *owner = link->getOwner();
        bool visible = (owner == p1 || link->isRevealed());

        // Select background color just for the link’s "highlight"
        if (link->isJammed()) bgColor = Xwindow::Purple;
        else if (visible && link->getType() == LinkType::Data) bgColor = Xwindow::Green;
        else if (visible && link->getType() == LinkType::Virus) bgColor = Xwindow::Red;
        else bgColor = Xwindow::Black;
        text = string(1, link->getId());

        // Draw a smaller rectangle (overlay) for the link, leaving a margin
        xw.fillRectangle(x + margin, y + margin, cellWidth - 2*margin, cellHeight - 2*margin, bgColor);

        // Draw link text
        textColor = textColorForBackground(bgColor);
        xw.drawString(x + cellWidth/3, y + 2*cellHeight/3, text, textColor);

        if (link->isBoosted()) {
            xw.drawString(x + 2*cellWidth/3, y + 2*cellHeight/3, "*", textColor);
        }
    }
}


void GraphicDisplay::print(const Game &game, ostream &out) const {
    const auto &board = *game.getBoard();
    const auto &players = game.getPlayers();
    const Player *p1 = players[0];
    const Player *p2 = players[1];
    const auto &foggedCells = game.getFoggedCells();

    int lineHeight = 15;
    int infoHeightTop = 70;
    int boardOffsetY = infoHeightTop;

    // ---------- 1. Build Player 1 info as a single string ----------
    ostringstream p1Stream;
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
    string newP1Info = p1Stream.str();

    // ---------- 2. Only redraw if info changed ----------
    if (newP1Info != lastP1Info) {
        lastP1Info = newP1Info;

        // Clear background
        xw.fillRectangle(0, 0, gridSize * cellWidth, infoHeightTop, Xwindow::White);

        // Draw lines
        istringstream iss(newP1Info);
        string line;
        int y = 15;
        while (getline(iss, line)) {
            xw.drawString(10, y, line);
            y += lineHeight;
        }
    }

    // ---------- 3. Draw the board ----------
    for (int r = 0; r < gridSize; ++r) {
        for (int c = 0; c < gridSize; ++c) {
            const Cell &cell = board.at(r, c);
            bool isFogged = (foggedCells.find({r, c}) != foggedCells.end());
            
            DrawnState newState{' ', false, false, LinkType::Data};
            if (isFogged) {
                newState.symbol = '?';
            }
            else if (!cell.isEmpty()) {
                Link *link = cell.getLink();
                Player *owner = link->getOwner();
                bool visible = (owner == p1 || link->isRevealed());

                newState.visible = visible;
                newState.boosted = link->isBoosted();
                newState.jammed = link->isJammed();
                newState.type = link->getType();
                newState.symbol = visible ? link->getId() : '?';
            }
            else if (cell.getType() == CellType::ServerPort) {
                newState.symbol = 'S';
            }
            else if (cell.getType() == CellType::Firewall) {
                newState.symbol = 'F';
            }

            // Only redraw if something changed
            const DrawnState &oldState = lastDrawn[r][c];
            if (isFogged || oldState.symbol != newState.symbol ||
                oldState.boosted != newState.boosted ||
                oldState.visible != newState.visible ||
                oldState.jammed  != newState.jammed  ||
                (newState.visible && oldState.type != newState.type)) {

                const_cast<GraphicDisplay*>(this)->lastDrawn[r][c] = newState;
                if (isFogged) {
                    int x = c * cellWidth + 10;
                    int offsetY = 100;
                    int y = offsetY + r * cellHeight;
                    int fogColor = Xwindow::Grey;
                    int margin = 4;

                    xw.fillRectangle(x + margin, y + margin, cellWidth - 2*margin, cellHeight - 2*margin, fogColor);
                    xw.drawString(x + cellWidth/3, y + 2*cellHeight/3, "?", Xwindow::Black);
                } else {
                    drawCell(r, c, cell, p1);
                }
            }

        }
    }

    // ---------- 4. Build Player 2 info as a single string ----------
    ostringstream p2Stream;
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
    string newP2Info = p2Stream.str();

    // ---------- 5. Redraw Player 2 info only if changed ----------
    int p2StartAreaY = boardOffsetY + gridSize * cellHeight + 50;
    int p2AreaHeight = 70;
    if (newP2Info != lastP2Info) {
        lastP2Info = newP2Info;

        // Clear background
        xw.fillRectangle(0, p2StartAreaY, gridSize * cellWidth, p2AreaHeight, Xwindow::White);

        istringstream iss2(newP2Info);
        string line2;
        int y2 = p2StartAreaY + 15;
        while (getline(iss2, line2)) {
            xw.drawString(10, y2, line2);
            y2 += lineHeight;
        }
    }
}
