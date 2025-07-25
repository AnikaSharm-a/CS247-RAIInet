#include "player.h"
#include "link.h"

Player::Player(int id) : id(id) {}

void Player::addLink(Link* link) {
    links[link->getId()] = link;
}

bool Player::hasWon() const {
    return downloadedData >= 4;
}

bool Player::hasLost() const {
    return downloadedVirus >= 4;
}

void Player::incrementDownload(Link* link) {
    if (link->getType() == LinkType::Data) downloadedData++;
    else downloadedVirus++;
}
