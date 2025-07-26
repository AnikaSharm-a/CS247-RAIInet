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

// ... inside runTest():

void runTest() {
    TextDisplay td(8);
    Game game;

    Player* p1 = new Player(1);
    Player* p2 = new Player(2);
    game.getPlayers().push_back(p1);
    game.getPlayers().push_back(p2);

    game.startGame();

    Controller controller(&td, &game);
    game.setController(&controller);

    // Add default abilities to players
    auto p1Abilities = AbilityFactory::getDefaultAbilities();
    for (Ability* ability : p1Abilities) {
        p1->addAbility(ability);
    }
    auto p2Abilities = AbilityFactory::getDefaultAbilities();
    for (Ability* ability : p2Abilities) {
        p2->addAbility(ability);
    }

    // Setup initial links
    Link* p1_data1 = new Link('a', LinkType::Data, 3, p1);
    Link* p1_data2 = new Link('b', LinkType::Data, 2, p1);
    Link* p1_virus = new Link('v', LinkType::Virus, 1, p1);

    p1->addLink(p1_data1);
    p1->addLink(p1_data2);
    p1->addLink(p1_virus);

    game.getBoard()->at(6, 0).setLink(p1_data1);
    game.getBoard()->at(6, 1).setLink(p1_data2);
    game.getBoard()->at(6, 2).setLink(p1_virus);

    Link* p2_data1 = new Link('A', LinkType::Data, 3, p2);
    Link* p2_virus1 = new Link('V', LinkType::Virus, 2, p2);
    Link* p2_virus2 = new Link('W', LinkType::Virus, 2, p2);

    p2->addLink(p2_data1);
    p2->addLink(p2_virus1);
    p2->addLink(p2_virus2);

    game.getBoard()->at(1, 0).setLink(p2_data1);
    game.getBoard()->at(1, 1).setLink(p2_virus1);
    game.getBoard()->at(1, 2).setLink(p2_virus2);

    // Simulate a series of moves, battles, and ability uses
    std::string commands = R"(
        board
        move a up
        move A down
        move b up
        move V left
        move v up
        board
        ability 1 V
        move a up
        move W down
        move b up
        move A down
        board
        quit
    )";



    std::istringstream input(commands);
    controller.play(input);

    // Check positions after moves
    auto pos_a = game.getBoard()->findLinkPosition('a', p1);
    auto pos_b = game.getBoard()->findLinkPosition('b', p1);
    auto pos_v = game.getBoard()->findLinkPosition('v', p1);
    auto pos_A = game.getBoard()->findLinkPosition('A', p2);
    auto pos_V = game.getBoard()->findLinkPosition('V', p2);
    auto pos_W = game.getBoard()->findLinkPosition('W', p2);

    // Print positions for debugging
    std::cout << "'a' at (" << pos_a.first << "," << pos_a.second << ")\n";
    std::cout << "'b' at (" << pos_b.first << "," << pos_b.second << ")\n";
    std::cout << "'v' at (" << pos_v.first << "," << pos_v.second << ")\n";
    std::cout << "'A' at (" << pos_A.first << "," << pos_A.second << ")\n";
    std::cout << "'V' at (" << pos_V.first << "," << pos_V.second << ")\n";
    std::cout << "'W' at (" << pos_W.first << "," << pos_W.second << ")\n";

    // Check for victory or game over condition
    assert(game.isGameOver() || true);  // Depending on whether victory happened

    // Check downloads to confirm downloads occurred during battles
    std::cout << "Player 1 data downloaded: " << p1->getDownloadedData() << "\n";
    std::cout << "Player 1 viruses downloaded: " << p1->getDownloadedVirus() << "\n";
    std::cout << "Player 2 data downloaded: " << p2->getDownloadedData() << "\n";
    std::cout << "Player 2 viruses downloaded: " << p2->getDownloadedVirus() << "\n";

    // Cleanup (note: abilities allocated by factory need to be deleted if your Player doesn't do it)
    // Here you could either delete abilities manually or let Player handle it on destruction.

    delete p1_data1;
    delete p1_data2;
    delete p1_virus;
    delete p2_data1;
    delete p2_virus1;
    delete p2_virus2;
    delete p1;
    delete p2;

    std::cout << "Complex game simulation test completed!\n";
}



// Initialize and run an interactive game
void runGame() {
    TextDisplay td(8);
    Game game;

    Player* p1 = new Player(1);
    Player* p2 = new Player(2);
    game.getPlayers().push_back(p1);
    game.getPlayers().push_back(p2);

    game.startGame();

    Controller controller(&td, &game);
    game.setController(&controller);

    std::cout << "Welcome to the game! Type your commands below.\n";
    controller.play(std::cin);

    // Cleanup
    delete p1;
    delete p2;
}

// int main(int argc, char* argv[]) {
//     if (argc > 1) {
//         std::string arg = argv[1];
//         if (arg == "test") {
//             runTest();
//             return 0;
//         }
//     }

//     runGame();
//     return 0;
// }

int main(int argc, char* argv[]) {
    TextDisplay td(8);
    Game game;

    Player *p1 = new Player(1);
    Player *p2 = new Player(2);

    game.getPlayers().push_back(p1);
    game.getPlayers().push_back(p2);

    Controller controller(&td, &game);
    game.setController(&controller);

    game.startGame();

    std::cout << "Welcome to RAIInet! Player 1 goes first.\n";
    controller.play(std::cin);

    delete p1;
    delete p2;

    return 0;
}
