#include "player.h"
#include "link.h"
#include "ability.h"

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

// Ability management methods
void Player::addAbility(Ability* ability) {
    abilities.push_back(ability);
}

Ability* Player::getAbility(int abilityId) {
    if (abilityId < 1 || abilityId > static_cast<int>(abilities.size())) {
        return nullptr;
    }
    return abilities[abilityId - 1]; // Convert to 0-based indexing
}

bool Player::isAbilityUsed(int abilityId) const {
    return usedAbilities.find(abilityId) != usedAbilities.end();
}

void Player::markAbilityUsed(int abilityId) {
    usedAbilities.insert(abilityId);
}

int Player::getNumAbilities() const {
    return abilities.size();
}
