#include <sstream>
#include <cassert>
#include "controller.h"
#include "textdisplay.h"
#include "game.h"
#include "player.h"
#include "link.h"
#include <iostream>

void runTest() {
    TextDisplay td(8);
    Game game;
    game.startGame();

    Controller controller(&td, &game);
    game.setController(&controller);

    Player* p1 = new Player(1);
    Player* p2 = new Player(2);
    game.getPlayers().push_back(p1);
    game.getPlayers().push_back(p2);

    Link* l1 = new Link('a', LinkType::Data, 3, p1);
    p1->addLink(l1);
    game.getBoard()->at(6, 0).setLink(l1);

    Link* l2 = new Link('A', LinkType::Virus, 2, p2);
    p2->addLink(l2);
    game.getBoard()->at(1, 0).setLink(l2);

    // Prepare a sequence of commands as if typed by a user
    std::string commands = R"(
        board
        move a up
        move A down
        quit
    )";

    // Use istringstream to simulate user input
    std::istringstream input(commands);

    controller.play(input);

    // After moves, assert expected game state
    // For example: Link 'a' moved up from (6,0) to (5,0)
    // Make sure the board has the link in the expected position
    auto pos = game.getBoard()->findLinkPosition('a', p1);
    assert(pos.first == 5 && pos.second == 0);

    // Also check downloads, link states, etc. as needed
    assert(p1->getDownloadedData() >= 0);  // Just an example assertion

    // Cleanup
    delete l1;
    delete l2;
    delete p1;
    delete p2;

    std::cout << "Automated test passed!\n";
}

int main() {
    runTest();
    return 0;
}
