#include <iostream>
#include <memory>

#include "game.h"
#include "controller.h"
#include "textdisplay.h"
#include "graphicdisplay.h"
#include "abilityFactory.h"

using namespace std;

int main(int argc, char* argv[]) {
    // unique_ptr objects that will be owned by the controller
    auto td = make_unique<TextDisplay>(8);
    unique_ptr<GraphicDisplay> gd = nullptr;
    auto game = make_unique<Game>();

    auto p1 = make_unique<Player>(1);
    auto p2 = make_unique<Player>(2);

    // default ability strings
    string ability1Str = "LFDSP";
    string ability2Str = "LFDSP";
    string link1File = "";
    string link2File = "";
    bool graphics = false;

    // COmmand line parsing
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-ability1" && i + 1 < argc) {
            ability1Str = argv[++i];
            if (!AbilityFactory::isValidAbilityString(ability1Str)) {
                cerr << "Invalid ability string for player 1: " << ability1Str << endl;
                cerr << "\n" << AbilityFactory::getUsageInfo() << endl;
                return 1;
            }
        } else if (arg == "-ability2" && i + 1 < argc) {
            ability2Str = argv[++i];
            if (!AbilityFactory::isValidAbilityString(ability2Str)) {
                cerr << "Invalid ability string for player 2: " << ability2Str << endl;
                cerr << "\n" << AbilityFactory::getUsageInfo() << endl;
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

    // FOr graphics mode
    unique_ptr<View> viewToUse;
    if (graphics) {
        gd = make_unique<GraphicDisplay>(8, 500, 700);
        viewToUse = move(gd);
    } else {
        viewToUse = move(td);
    }

    // create controller that owns both view and game
    Controller controller(move(viewToUse), move(game));

    try {
        controller.getGame()->setupPlayers(controller.getGame()->getPlayers()[0], controller.getGame()->getPlayers()[1], ability1Str, ability2Str, link1File, link2File);
    } catch (const exception& e) {
        cerr << "Setup failed: " << e.what() << endl;
        cerr << "\n" << AbilityFactory::getUsageInfo() << endl;
        return 1;
    }

    controller.getGame()->startGame();

    cout << "Welcome to RAIInet! Player 1 goes first.\n";
    controller.play(cin);

    return 0;
}
