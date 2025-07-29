#include "textdisplay.h"
#include "game.h"
#include "board.h"
#include "player.h"
#include "link.h"
#include "ability.h"

using namespace std;

TextDisplay::TextDisplay(int gridSize) {
    theDisplay.assign(gridSize, vector<char>(gridSize, '.'));
    // Mark server ports
    theDisplay[0][3] = 'S';
    theDisplay[0][4] = 'S';
    theDisplay[7][3] = 'S';
    theDisplay[7][4] = 'S';
}

void TextDisplay::print(const Game &game, ostream &out) const {
    // Access players const-correctly
    const auto& players = game.getPlayers(); // Make sure getPlayers() returns const ref for const Game
    const Player* p1 = players[0];
    const Player* p2 = players[1];
    const Player* current = players[game.getCurrentPlayerIdx()];


    // Print Player 1 info
    out << "Player 1:\n";
    out << "Downloaded: " << p1->getDownloadedData() << "D, " << p1->getDownloadedVirus() << "V\n";
    out << "Abilities: " << p1->getNumUnusedAbilities() << "\n";

    // Show links for Player 1 depending on whether current player is Player 1
    for (const auto& pair : p1->getLinks()) {
        char id = pair.first;
        Link* link = pair.second;
        if (current == p1) {
            // Current player sees all own links fully
            string typeStr = (link->getType() == LinkType::Virus ? "V" : "D");
            out << id << ": " << typeStr << link->getStrength() << " ";
        } else {
            // Opponent view: show revealed or '?'
            if (link->isRevealed()) {
                string typeStr = (link->getType() == LinkType::Virus ? "V" : "D");
                out << id << ": " << typeStr << link->getStrength() << " ";
            } else {
                out << id << ": ? ";
            }
        }
    }
    out << "\n";

    out << "========\n";

    // Board print (8x8)
    const auto& board = *game.getBoard();
    const auto& fogged = game.getFoggedCells();

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            const Cell& cell = board.at(r, c);
            std::pair<int, int> coord = {r, c};

            auto fogIt = fogged.find(coord);
            if (fogIt != fogged.end()) {
                // Get fog owner from tuple: std::tuple<CellType, int, int>
                int fogOwnerId = std::get<2>(fogIt->second);

                // Show '?' only if fog owner is NOT current player
                if (fogOwnerId != current->getId()) {
                    out << "?";
                    continue;  // skip to next cell
                }
                // else fallthrough to show normal content for fog owner
            }
            if (!cell.isEmpty()) {
                Link* link = cell.getLink();
                Player* owner = link->getOwner();

                if (owner == p1) {
                    out << link->getId();
                } else {
                    out << link->getId();
                }
            } else {
                // Print Server Ports as 'S', otherwise '.'
                if (cell.getType() == CellType::ServerPort) {
                    out << "S";
                } else if (cell.getType() == CellType::Firewall) {
                    out << (cell.getOwnerId() == 1 ? 'm' : 'w');
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
        if (current == p2) {
            // Current player sees own links fully
            string typeStr = (link->getType() == LinkType::Virus ? "V" : "D");
            out << id << ": " << typeStr << link->getStrength() << " ";
        } else {
            // Opponent sees revealed or '?'
            if (link->isRevealed()) {
                string typeStr = (link->getType() == LinkType::Virus ? "V" : "D");
                out << id << ": " << typeStr << link->getStrength() << " ";
            } else {
                out << id << ": ? ";
            }
        }
    }
    out << "\n";
}

