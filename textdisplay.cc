#include "textdisplay.h"
#include "game.h"
#include "board.h"
#include "player.h"
#include "link.h"
#include "ability.h"

using namespace std;

TextDisplay::TextDisplay(int gridSize) : gameRef(nullptr), hasRedrawnThisTurn(false) {
    theDisplay.assign(gridSize, vector<char>(gridSize, '.'));
    // Mark server ports
    theDisplay[0][3] = 'S';
    theDisplay[0][4] = 'S';
    theDisplay[7][3] = 'S';
    theDisplay[7][4] = 'S';
}

void TextDisplay::notify(const NotificationData& data) {
    // Only redraw for link movements - this is the most important event
    if (gameRef && data.type == NotificationType::LinkMoved) {
        cout << endl; // Add spacing for link movements
        print(*gameRef, cout);
    }
    // For turn changes, just add spacing but don't redraw (since we already redrew for the move)
    else if (data.type == NotificationType::GameStateChanged) {
        cout << endl; // Add extra spacing for turn changes
    }
    // Ignore all other notifications
}

void TextDisplay::print(const Game &game, ostream &out) const {
    // Access players const-correctly
    const auto& players = game.getPlayers(); // Make sure getPlayers() returns const ref for const Game
    const Player* p1 = players[0];
    const Player* p2 = players[1];
    const Player* current = players[game.getCurrentPlayerIdx()];


    // Print Player 1 info
    out << "\nPlayer 1:\n";
    out << "Downloaded: " << p1->getDownloadedData() << "D, " << p1->getDownloadedVirus() << "V\n";
    out << "Abilities: " << p1->getNumUnusedAbilities() << "\n";
    // Show links for Player 1 depending on whether current player is Player 1
    int linkCount = 0;
    for (const auto& pair : p1->getLinks()) {
        char id = pair.first;
        auto link = pair.second.get();
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
        
        // Add newline after every 4 links
        linkCount++;
        if (linkCount == 4) {
            out << "\n";
        }
    }
    // Add final newline if we didn't just add one
    if (linkCount % 4 != 0) {
        out << "\n";
    }
    out << "\n"; // Add extra spacing before the board separator

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
                // Check if current player owns ALL fog on this cell
                bool ownsAllFog = true;
                for (const auto& fog : fogIt->second.second) {
                    int fogOwnerId = fog.second;
                    if (fogOwnerId != current->getId()) {
                        ownsAllFog = false;
                        break;
                    }
                }

                // Show '?' if current player doesn't own ALL fog on this cell
                if (!ownsAllFog) {
                    out << "?";
                    continue;  // skip to next cell
                }
                // else fallthrough to show normal content only if player owns all fog
            }
            if (!cell.isEmpty()) {
                auto link = cell.getLink();
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

    int linkCount2 = 0;
    for (const auto& pair : p2->getLinks()) {
        char id = pair.first;
        auto link = pair.second.get();
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
        
        // Add newline after every 4 links
        linkCount2++;
        if (linkCount2 == 4) {
            out << "\n";
        }
    }
    // Add final newline if we didn't just add one
    if (linkCount2 % 4 != 0) {
        out << "\n";
    }
    out << "\n";
}


