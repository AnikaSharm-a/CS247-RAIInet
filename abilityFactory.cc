#include "abilityFactory.h"
#include "download.h"
#include "firewall.h"
#include "linkboost.h"
#include "polarize.h"
#include "scan.h"
#include <iostream>
#include <stdexcept>
#include <map>

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
        std::string errorMsg = "Invalid ability string: " + abilityString + ". ";
        
        if (abilityString.length() != 5) {
            errorMsg += "Must have exactly 5 abilities.";
        } else {
            // Count occurrences to provide specific feedback
            std::map<char, int> abilityCounts;
            for (char code : abilityString) {
                char upperCode = toupper(code);
                if (!isValidAbilityCode(upperCode)) {
                    errorMsg += "Invalid ability code: " + std::string(1, code) + ". ";
                    errorMsg += "Valid codes are: " + getValidAbilityCodes();
                    break;
                }
                abilityCounts[upperCode]++;
                if (abilityCounts[upperCode] > 2) {
                    errorMsg += "Cannot have more than 2 copies of ability: " + std::string(1, upperCode);
                    break;
                }
            }
        }
        
        throw std::invalid_argument(errorMsg);
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
    
    // Count occurrences of each ability type
    std::map<char, int> abilityCounts;
    
    for (char code : abilityString) {
        char upperCode = toupper(code);
        
        // Check if it's a valid ability code
        if (!isValidAbilityCode(upperCode)) {
            return false;
        }
        
        // Count occurrences
        abilityCounts[upperCode]++;
        
        // Check if any ability appears more than 2 times
        if (abilityCounts[upperCode] > 2) {
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

std::string AbilityFactory::getUsageInfo() {
    return "Ability Selection Rules:\n"
           "- Must have exactly 5 abilities\n"
           "- Maximum 2 copies of each ability type\n"
           "- Valid codes: " + getValidAbilityCodes() + "\n"
           "- L: LinkBoost, F: Firewall, D: Download, S: Scan, P: Polarize\n"
           "- Examples: LFDSP (one of each), FFDDL (two firewalls, two downloads, one linkboost)";
} 