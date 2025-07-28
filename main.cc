#include <iostream>
#include <fstream> 
#include <sstream>
#include <cassert>
#include <algorithm>
#include <random>

#include "game.h"
#include "controller.h"
#include "textdisplay.h"
#include "graphicdisplay.h"
#include "player.h"
#include "link.h"

// Run automated test with predefined commands
#include "abilityFactory.h"  // Add this include

LinkType parseLinkType(const std::string& s) {
    if (s.empty()) throw std::invalid_argument("Empty link type.");
    if (s[0] == 'D') return LinkType::Data;
    if (s[0] == 'V') return LinkType::Virus;
    throw std::invalid_argument("Invalid link type: " + s);
}

int parseStrength(const std::string& s) {
    if (s.length() < 2) throw std::invalid_argument("Link string too short.");
    int strength = s[1] - '0';
    if (strength < 1 || strength > 4) throw std::invalid_argument("Invalid strength: " + s);
    return strength;
}

void loadLinksFromFile(const std::string& filename, Player* player, bool isPlayer1) {
    std::ifstream in(filename);
    if (!in) throw std::runtime_error("Failed to open link file: " + filename);

    std::vector<std::string> tokens;
    std::string s;
    while (in >> s) {
        tokens.push_back(s);
    }

    if (tokens.size() != 8) {
        throw std::runtime_error("Expected 8 links in file: " + filename);
    }

    for (int i = 0; i < 8; ++i) {
        std::string linkStr = tokens[i];
        LinkType type = parseLinkType(linkStr);
        int strength = parseStrength(linkStr);

        char id = isPlayer1 ? ('a' + i) : ('A' + i);
        Link* link = new Link(id, type, strength, player);
        player->addLink(link);
    }
}

// Generate default links (same as your original config) for player
void generateDefaultLinks(Player* player, bool isPlayer1) {
    std::vector<std::pair<LinkType,int>> configs = {
        {LinkType::Virus, 1}, {LinkType::Data, 4},
        {LinkType::Virus, 3}, {LinkType::Virus, 2},
        {LinkType::Data, 3}, {LinkType::Virus, 4},
        {LinkType::Data, 2}, {LinkType::Data, 1}
    };

    // Shuffle configs randomly
    static std::random_device rd;
    static std::mt19937 g(rd());
    std::shuffle(configs.begin(), configs.end(), g);

    char id = isPlayer1 ? 'a' : 'A';
    for (const auto& cfg : configs) {
        Link* link = new Link(id, cfg.first, cfg.second, player);
        player->addLink(link);
        ++id;
    }
}

int main(int argc, char* argv[]) {
    TextDisplay td(8);
    GraphicDisplay* gd = nullptr;
    View* viewToUse = &td;
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

    if (graphics) {
        gd = new GraphicDisplay(8, 500, 500);
        viewToUse = gd;
    }

    Controller controller(viewToUse, &game);
    game.setController(&controller);

    try {
        auto p1Abilities = AbilityFactory::createAbilities(ability1Str);
        for (Ability* ability : p1Abilities) p1->addAbility(ability);

        auto p2Abilities = AbilityFactory::createAbilities(ability2Str);
        for (Ability* ability : p2Abilities) p2->addAbility(ability);

        // Load links from file if specified, else generate defaults
        if (!link1File.empty()) {
            loadLinksFromFile(link1File, p1, true);
        } else {
            generateDefaultLinks(p1, true);
        }

        if (!link2File.empty()) {
            loadLinksFromFile(link2File, p2, false);
        } else {
            generateDefaultLinks(p2, false);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error parsing abilities: " << e.what() << std::endl;
        return 1;
    }

    game.startGame();

    std::cout << "Welcome to RAIInet! Player 1 goes first.\n";
    controller.play(std::cin);

    delete p1;
    delete p2;
    delete gd;

    return 0;
}
