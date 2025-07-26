#include "abilityFactory.h"
#include "download.h"
#include "firewall.h"
#include "linkboost.h"
#include "polarize.h"
#include "scan.h"
#include <iostream>
#include <stdexcept>

Ability* AbilityFactory::createAbility(char abilityCode) {
    switch (abilityCode) {
        case 'L':
        case 'l':
            return new LinkBoost();
        case 'F':
        case 'f':
            return new FireWall();
        case 'D':
        case 'd':
            return new Download();
        case 'S':
        case 's':
            return new Scan();
        case 'P':
        case 'p':
            return new Polarize();
        default:
            throw std::invalid_argument("Invalid ability code: " + std::string(1, abilityCode));
    }
}

std::vector<Ability*> AbilityFactory::createAbilities(const std::string& abilityString) {
    if (!isValidAbilityString(abilityString)) {
        throw std::invalid_argument("Invalid ability string: " + abilityString);
    }
    
    std::vector<Ability*> abilities;
    for (char code : abilityString) {
        abilities.push_back(createAbility(code));
    }
    return abilities;
}

std::vector<Ability*> AbilityFactory::getDefaultAbilities() {
    return createAbilities("LFDSP"); // LinkBoost, Firewall, Download, Scan, Polarize
}

bool AbilityFactory::isValidAbilityString(const std::string& abilityString) {
    if (abilityString.length() != 5) {
        return false;
    }
    
    for (char code : abilityString) {
        if (!isValidAbilityCode(code)) {
            return false;
        }
    }
    return true;
}

std::string AbilityFactory::getValidAbilityCodes() {
    return "LFDSP"; // LinkBoost, Firewall, Download, Scan, Polarize
}

std::string AbilityFactory::getAbilityName(char abilityCode) {
    switch (abilityCode) {
        case 'L':
        case 'l':
            return "LinkBoost";
        case 'F':
        case 'f':
            return "Firewall";
        case 'D':
        case 'd':
            return "Download";
        case 'S':
        case 's':
            return "Scan";
        case 'P':
        case 'p':
            return "Polarize";
        default:
            return "Unknown";
    }
}

bool AbilityFactory::isValidAbilityCode(char abilityCode) {
    std::string validCodes = getValidAbilityCodes();
    return validCodes.find(toupper(abilityCode)) != std::string::npos;
} 