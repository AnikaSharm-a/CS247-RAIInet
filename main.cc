#include <iostream>
#include <fstream> 
#include <sstream>
#include <cassert>
#include <algorithm>
#include <random>
#include <memory>

#include "game.h"
#include "controller.h"
#include "textdisplay.h"
#include "graphicdisplay.h"
#include "player.h"
#include "link.h"

#include "abilityFactory.h"

using namespace std;

int main(int argc, char* argv[]) {
    // Create unique_ptr objects that will be owned by the controller
    auto td = make_unique<TextDisplay>(8);
    unique_ptr<GraphicDisplay> gd = nullptr;
    auto game = make_unique<Game>();

    auto p1 = make_unique<Player>(1);
    auto p2 = make_unique<Player>(2);

    // Default ability strings
    string ability1Str = "LFDSP";
    string ability2Str = "LFDSP";
    string link1File = "";
    string link2File = "";
    bool graphics = false;

    // Simple command line parsing
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-ability1" && i + 1 < argc) {
            ability1Str = argv[++i];
            // Validate ability string immediately
            if (!AbilityFactory::isValidAbilityString(ability1Str)) {
                cerr << "Invalid ability string for player 1: " << ability1Str << "\n";
                cerr << "\n" << AbilityFactory::getUsageInfo() << "\n";
                return 1;
            }
        } else if (arg == "-ability2" && i + 1 < argc) {
            ability2Str = argv[++i];
            // Validate ability string immediately
            if (!AbilityFactory::isValidAbilityString(ability2Str)) {
                cerr << "Invalid ability string for player 2: " << ability2Str << "\n";
                cerr << "\n" << AbilityFactory::getUsageInfo() << "\n";
                return 1;
            }
        } else if (arg == "-link1" && i + 1 < argc) {
            link1File = argv[++i];
        } else if (arg == "-link2" && i + 1 < argc) {
            link2File = argv[++i];
        } else if (arg == "-graphics") {
            graphics = true;
        } else {
            cerr << "Unknown or invalid argument: " << arg << "\n";
            return 1;
        }
    }

    game->addPlayer(move(p1));
    game->addPlayer(move(p2));

    // Handle graphics mode
    unique_ptr<View> viewToUse;
    if (graphics) {
        gd = make_unique<GraphicDisplay>(8, 500, 700);
        viewToUse = move(gd);
    } else {
        viewToUse = move(td);
    }

    // Create controller with ownership of both view and game
    Controller controller(move(viewToUse), move(game));

    try {
        controller.getGame()->setupPlayers(controller.getGame()->getPlayers()[0], controller.getGame()->getPlayers()[1], ability1Str, ability2Str, link1File, link2File);
    } catch (const exception& e) {
        cerr << "Setup failed: " << e.what() << "\n";
        cerr << "\n" << AbilityFactory::getUsageInfo() << "\n";
        return 1;
    }

    controller.getGame()->startGame();

    cout << "Welcome to RAIInet! Player 1 goes first.\n";
    controller.play(cin);

    return 0;
}