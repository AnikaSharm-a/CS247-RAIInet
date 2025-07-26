#include "player.h"
#include "link.h"
#include "ability.h"
#include <iostream>

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

// Enhanced ability management methods
void Player::displayAbilities() const {
    std::cout << "Player " << id << " Abilities:\n";
    for (int i = 0; i < static_cast<int>(abilities.size()); ++i) {
        int abilityId = i + 1;
        std::string status = isAbilityUsed(abilityId) ? "[USED]" : "[AVAILABLE]";
        std::cout << "  " << abilityId << ". " << abilities[i]->getName() << " " << status << "\n";
    }
}

bool Player::hasUnusedAbilities() const {
    return usedAbilities.size() < abilities.size();
}

int Player::getUnusedAbilityCount() const {
    return abilities.size() - usedAbilities.size();
}

std::vector<std::string> Player::getAbilityNames() const {
    std::vector<std::string> names;
    for (const auto& ability : abilities) {
        names.push_back(ability->getName());
    }
    return names;
}

void Player::resetAbilities() {
    usedAbilities.clear();
}
