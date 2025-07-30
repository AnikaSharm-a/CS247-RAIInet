#include "textdisplay.h"
#include "game.h"
#include "board.h"
#include "player.h"
#include "link.h"
#include "ability.h"

using namespace std;

TextDisplay::TextDisplay(int gridSize) : gameRef(nullptr), hasRedrawnThisTurn(false) {
    theDisplay.assign(gridSize, vector<char>(gridSize, '.'));
    //server ports
    theDisplay[0][3] = 'S';
    theDisplay[0][4] = 'S';
    theDisplay[7][3] = 'S';
    theDisplay[7][4] = 'S';
}

void TextDisplay::notify(const NotificationData& data) {
    //redraw for link movement
    if (gameRef && data.type == NotificationType::LinkMoved) {
        cout << endl;
        print(*gameRef, cout);
    }
    //turn changes - not rerender
    else if (data.type == NotificationType::GameStateChanged) {
        cout << endl;
    }
    //ignore other notifications - for graphicDisplay
}

void TextDisplay::print(const Game &game, ostream &out) const {
    //access players
    const auto& players = game.getPlayers(); 
    const Player* p1 = players[0];
    const Player* p2 = players[1];
    const Player* current = players[game.getCurrentPlayerIdx()];


    //print player 1 info
    out << "\nPlayer 1:\n";
    out << "Downloaded: " << p1->getDownloadedData() << "D, " << p1->getDownloadedVirus() << "V\n";
    out << "Abilities: " << p1->getNumUnusedAbilities() << "\n";
    //show links depending on current player
    int linkCount = 0;
    for (const auto& pair : p1->getLinks()) {
        char id = pair.first;
        auto link = pair.second.get();
        if (current == p1) {
            //see all links fully
            string typeStr = (link->getType() == LinkType::Virus ? "V" : "D");
            out << id << ": " << typeStr << link->getStrength() << " ";
        } else {
            //? for opponent
            if (link->isRevealed()) {
                string typeStr = (link->getType() == LinkType::Virus ? "V" : "D");
                out << id << ": " << typeStr << link->getStrength() << " ";
            } else {
                out << id << ": ? ";
            }
        }
        
        //print formatting
        linkCount++;
        if (linkCount == 4) {
            out << "\n";
        }
    }
    //add final newline if we didn't just add one
    if (linkCount % 4 != 0) {
        out << "\n";
    }
    out << "\n"; //add extra spacing before the board separator

    out << "========\n";

    //board print
    const auto& board = *game.getBoard();
    const auto& fogged = game.getFoggedCells();

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            const Cell& cell = board.at(r, c);
            std::pair<int, int> coord = {r, c};

            auto fogIt = fogged.find(coord);
            if (fogIt != fogged.end()) {
                //check if current player owns fog
                bool ownsAllFog = true;
                for (const auto& fog : fogIt->second.second) {
                    int fogOwnerId = fog.second;
                    if (fogOwnerId != current->getId()) {
                        ownsAllFog = false;
                        break;
                    }
                }
                //if fog is not owned, display ?
                if (!ownsAllFog) {
                    out << "?";
                    continue;
                }
                //display normally if not fogged
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
                //server ports
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

    
    //player 2 info
    out << "Player 2:\n";
    out << "Downloaded: " << p2->getDownloadedData() << "D, " << p2->getDownloadedVirus() << "V\n";
    out << "Abilities: " << p2->getNumUnusedAbilities() << "\n";

    int linkCount2 = 0;
    for (const auto& pair : p2->getLinks()) {
        char id = pair.first;
        auto link = pair.second.get();
        if (current == p2) {
            //sees own links
            string typeStr = (link->getType() == LinkType::Virus ? "V" : "D");
            out << id << ": " << typeStr << link->getStrength() << " ";
        } else {
            //see ?
            if (link->isRevealed()) {
                string typeStr = (link->getType() == LinkType::Virus ? "V" : "D");
                out << id << ": " << typeStr << link->getStrength() << " ";
            } else {
                out << id << ": ? ";
            }
        }
        
        //add new line for print formatting
        linkCount2++;
        if (linkCount2 == 4) {
            out << "\n";
        }
    }
    //add one if not added already
    if (linkCount2 % 4 != 0) {
        out << "\n";
    }
    out << "\n";
}


