#include "textdisplay.h"
#include "game.h"
#include "board.h"
#include "player.h"
#include "link.h"
#include "ability.h"

TextDisplay::TextDisplay(int gridSize) {
    theDisplay.assign(gridSize, std::vector<char>(gridSize, '.'));
    // Mark server ports
    theDisplay[0][3] = 'S';
    theDisplay[0][4] = 'S';
    theDisplay[7][3] = 'S';
    theDisplay[7][4] = 'S';
}

// void TextDisplay::notify(int row, int col, char symbol) {
//     theDisplay[row][col] = symbol;
// }

static void printPlayerInfo(std::ostream &out, Player *p, bool showLinks) {
    out << "Player " << p->getId() << ":\n";
    out << "Downloaded: " << p->getDownloadedData()
        << "D, " << p->getDownloadedVirus() << "V\n";
    out << "Abilities: " << p->getUnusedAbilityCount() << "\n";

    int count = 0;
    for (auto &entry : p->getLinks()) {
        char id = entry.first;
        Link *link = entry.second;
        out << id << ": ";
        if (showLinks || link->isRevealed()) {
            out << (link->getType() == LinkType::Data ? 'D' : 'V') << link->getStrength();
        } else {
            out << "?";
        }

        ++count;
        if (count % 4 == 0) out << "\n";
        else out << " ";
    }
    if (count % 4 != 0) out << "\n";
}

void TextDisplay::print(const Game &game, std::ostream &out) const {
    // Access players const-correctly
    const auto& players = game.getPlayers(); // Make sure getPlayers() returns const ref for const Game
    const Player* p1 = players[0];
    const Player* p2 = players[1];


    // Print Player 1 info
    out << "Player 1:\n";
    out << "Downloaded: " << p1->getDownloadedData() << "D, " << p1->getDownloadedVirus() << "V\n";
    out << "Abilities: " << p1->getNumUnusedAbilities() << "\n";

    // Player 1 links: iterate map
    for (const auto& pair : p1->getLinks()) {
        char id = pair.first;
        Link* link = pair.second;
        std::string typeStr = (link->getType() == LinkType::Virus ? "V" : "D");
        out << id << ": " << typeStr << link->getStrength() << " ";
    }
    out << "\n";

    out << "========\n";

    // Board print (8x8)
    const auto& board = *game.getBoard();

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            const Cell& cell = board.at(r, c);
            if (!cell.isEmpty()) {
                Link* link = cell.getLink();
                Player* owner = link->getOwner();

                if (owner == p1) {
                    out << link->getId();
                } else {
                    // Opponent links: show revealed or '?'
                    // if (link->isRevealed()) {
                        out << link->getId();
                    // } else {
                    //  out << "?";
                    // }
                }
            } else {
                // Print Server Ports as 'S', otherwise '.'
                if (cell.getType() == CellType::ServerPort) {
                    out << "S";
                } else {
                    out << ".";
                }
            }
        }
        out << "\n";
    }

    out << "========\n";

    
    // Print Player 2 info similarly
    out << "Player 2:\n";
    out << "Downloaded: " << p2->getDownloadedData() << "D, " << p2->getDownloadedVirus() << "V\n";
    out << "Abilities: " << p2->getNumUnusedAbilities() << "\n";

    for (const auto& pair : p2->getLinks()) {
        char id = pair.first;
        Link* link = pair.second;  
        if (link->isRevealed()) {
            std::string typeStr = (link->getType() == LinkType::Virus ? "V" : "D");
            out << id << ": " << typeStr << link->getStrength() << " ";
        } else {
            out << id << ": ? ";
        }
    }
    out << "\n";
}

