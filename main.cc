#include "controller.h"
#include "textdisplay.h"
#include "game.h"
#include "player.h"
#include "link.h"
#include <iostream>

int main() {
    // Create text display
    TextDisplay td(8);

    // Create game
    Game game;
    game.startGame();

    // Setup controller
    Controller controller(&td, &game);
    game.setController(&controller);

    // Create players
    Player* p1 = new Player(1);
    Player* p2 = new Player(2);
    game.getPlayers().push_back(p1);
    game.getPlayers().push_back(p2);

    // Add links manually
    Link* l1 = new Link('a', LinkType::Data, 3, p1);
    p1->addLink(l1);
    game.getBoard()->at(6, 0).setLink(l1);

    Link* l2 = new Link('A', LinkType::Virus, 2, p2);
    p2->addLink(l2);
    game.getBoard()->at(1, 0).setLink(l2);

    // Test loop
    std::cout << "Type commands: board | move <id> <up/down/left/right> | quit\n";
    controller.play(std::cin);

    return 0;
}
