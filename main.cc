#include <iostream>
#include <sstream>
#include <cassert>

#include "game.h"
#include "controller.h"
#include "textdisplay.h"
#include "player.h"
#include "link.h"

// Run automated test with predefined commands
#include "abilityFactory.h"  // Add this include

int main(int argc, char* argv[]) {
    TextDisplay td(8);
    Game game;

    Player *p1 = new Player(1);
    Player *p2 = new Player(2);

    // Default ability strings
    std::string ability1Str = "LFDSP";
    std::string ability2Str = "LFDSP";
    std::string link1File = "";
    std::string link2File = "";
    bool graphics = false;

    // Simple command line parsing
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-ability1" && i + 1 < argc) {
            ability1Str = argv[++i];
        } else if (arg == "-ability2" && i + 1 < argc) {
            ability2Str = argv[++i];
        } else if (arg == "-link1" && i + 1 < argc) {
            link1File = argv[++i];
        } else if (arg == "-link2" && i + 1 < argc) {
            link2File = argv[++i];
        } else if (arg == "-graphics") {
            graphics = true;
        } else {
            std::cerr << "Unknown or invalid argument: " << arg << "\n";
        }
    }

    game.getPlayers().push_back(p1);
    game.getPlayers().push_back(p2);

    Controller controller(&td, &game);
    game.setController(&controller);

    try {
        auto p1Abilities = AbilityFactory::createAbilities(ability1Str);
        for (Ability* ability : p1Abilities) p1->addAbility(ability);

        auto p2Abilities = AbilityFactory::createAbilities(ability2Str);
        for (Ability* ability : p2Abilities) p2->addAbility(ability);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing abilities: " << e.what() << std::endl;
        return 1;
    }

    game.startGame();

    std::cout << "Welcome to RAIInet! Player 1 goes first.\n";
    controller.play(std::cin);

    delete p1;
    delete p2;

    return 0;
}
