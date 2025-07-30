#ifndef LINKTYPE_H
#define LINKTYPE_H

// defines the two types of links that can exist in the game
enum class LinkType {
    Virus,  // harmful links that count against the player when downloaded
    Data    // beneficial links that count toward victory when downloaded
};

#endif // LINKTYPE_H
