#include "player.h"
#include "link.h"
#include "ability.h"
#include <iostream>
using namespace std;

Player::Player(int id) : id(id) {}

int Player::getId() const {
    return id;
}

const map<char, unique_ptr<Link>>& Player::getLinks() const {
    return links;
}

set<int>& Player::getUsedAbilities() {
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
    for (const auto& entry : links) {
        auto link = entry.second.get();
        total += link->getStrength();
    }
    return total;
}

void Player::addLink(unique_ptr<Link> link) {
    char id = link->getId();
    links[id] = move(link);
}

shared_ptr<Link> Player::getLink(char id) const {
    auto it = links.find(id);
    return it != links.end() ? shared_ptr<Link>(it->second.get(), [](Link*){}) : nullptr;
}

void Player::removeLink(char id) {
    links.erase(id);
}

bool Player::ownsLink(char id) const {
    return links.find(id) != links.end();
}

vector<shared_ptr<Link>> Player::getAllLinks() const {
    vector<shared_ptr<Link>> all;
    for (const auto& entry : links) {
        all.push_back(shared_ptr<Link>(entry.second.get(), [](Link*){}));
    }
    return all;
}

void Player::revealLink(char id) {
    auto link = getLink(id);
    if (link) link->reveal();
}

bool Player::isLinkRevealed(char id) const {
    auto link = getLink(id);
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
    if (link->getType() == LinkType::Data) { 
        downloadedData++;
    }
    else { 
        downloadedVirus++;
    }
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
void Player::addAbility(unique_ptr<Ability> ability) {
    abilities.push_back(move(ability));
}

Ability* Player::getAbility(int abilityId) {
    if (abilityId < 1 || abilityId > static_cast<int>(abilities.size())) {
        return nullptr;
    }
    return abilities[abilityId - 1].get(); //convert
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
    cout << "Player " << id << " Abilities:\n";
    for (int i = 0; i < static_cast<int>(abilities.size()); ++i) {
        int abilityId = i + 1;
        if (!abilities[i]) {
            continue;
        }
        string status = isAbilityUsed(abilityId) ? "[USED]" : "[AVAILABLE]";
        cout << "  " << abilityId << ". " << abilities[i]->getName() << " " << status << "\n";
    }
}

bool Player::hasUnusedAbilities() const {
    return usedAbilities.size() < abilities.size();
}

int Player::getUnusedAbilityCount() const {
    return abilities.size() - usedAbilities.size();
}

vector<string> Player::getAbilityNames() const {
    vector<string> names;
    for (const auto& ability : abilities) {
        names.push_back(ability->getName());
    }
    return names;
}

void Player::resetAbilities() {
    usedAbilities.clear();
}
