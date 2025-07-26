#include "player.h"
#include "link.h"
#include "ability.h"
#include <iostream>

Player::Player(int id) : id(id) {}

int Player::getId() const {
    return id;
}

std::map<char, Link*>& Player::getLinks() {
    return links;
}

std::set<int>& Player::getUsedAbilities() {
    return usedAbilities;
}

int Player::getDownloadedVirus() const {
    return downloadedVirus;
}

int Player::getDownloadedData() const {
    return downloadedData;
}

int Player::getNumLinks() const {
    return links.size();
}

int Player::getStrengthSum() const {
    int total = 0;
    for (const auto& [id, link] : links) {
        total += link->getStrength();
    }
    return total;
}

void Player::addLink(Link* link) {
    links[link->getId()] = link;
}

Link* Player::getLink(char id) const {
    auto it = links.find(id);
    return it != links.end() ? it->second : nullptr;
}

void Player::removeLink(char id) {
    links.erase(id);
}

bool Player::ownsLink(char id) const {
    return links.find(id) != links.end();
}

std::vector<Link*> Player::getAllLinks() const {
    std::vector<Link*> all;
    for (const auto& [id, link] : links) {
        all.push_back(link);
    }
    return all;
}

void Player::revealLink(char id) {
    Link* link = getLink(id);
    if (link) link->reveal();
}

bool Player::isLinkRevealed(char id) const {
    Link* link = getLink(id);
    return link ? link->isRevealed() : false;
}

bool Player::canUseAbility(int abilityID) const {
    return abilityID >= 0 &&
           abilityID < (int)abilities.size() &&
           usedAbilities.find(abilityID) == usedAbilities.end();
}

int Player::getNumUnusedAbilities() const {
    return abilities.size() - usedAbilities.size();
}

void Player::incrementDownload(Link* link) {
    if (!link) return;
    if (link->getType() == LinkType::Data) downloadedData++;
    else downloadedVirus++;
}

void Player::resetDownloads() {
    downloadedData = 0;
    downloadedVirus = 0;
}

bool Player::hasWon() const {
    return downloadedData >= 4;
}

bool Player::hasLost() const {
    return downloadedVirus >= 4;
}

bool Player::hasLostOrWon() const {
    return hasWon() || hasLost();
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
