#include "abilityFactory.h"
#include "download.h"
#include "firewall.h"
#include "linkboost.h"
#include "polarize.h"
#include "scan.h"
#include "jam.h"
#include "fog.h"
#include "areascan.h"
#include <iostream>
#include <stdexcept>
#include <map>
using namespace std;

unique_ptr<Ability> AbilityFactory::createAbility(char abilityCode) {
    switch (abilityCode) {
        case 'L':
        case 'l':
            return make_unique<LinkBoost>();
        case 'F':
        case 'f':
            return make_unique<FireWall>();
        case 'D':
        case 'd':
            return make_unique<Download>();
        case 'S':
        case 's':
            return make_unique<Scan>();
        case 'P':
        case 'p':
            return make_unique<Polarize>();
        case 'J':
        case 'j':
            return make_unique<Jam>();
        case 'E':
        case 'e':
            return make_unique<Fog>();
        case 'A':
        case 'a':
            return make_unique<AreaScan>();
        default:
            throw invalid_argument("Invalid ability code: " + string(1, abilityCode));
    }
}

vector<unique_ptr<Ability>> AbilityFactory::createAbilities(const string& abilityString) {
    if (!isValidAbilityString(abilityString)) {
        string errorMsg = "Invalid ability string: " + abilityString + ". ";
        
        if (abilityString.length() != 5) {
            errorMsg += "Must have exactly 5 abilities.";
        } else {
            // Count occurrences to provide specific feedback
            map<char, int> abilityCounts;
            for (char code : abilityString) {
                char upperCode = toupper(code);
                if (!isValidAbilityCode(upperCode)) {
                    errorMsg += "Invalid ability code: " + string(1, code) + ". ";
                    errorMsg += "Valid codes are: " + getValidAbilityCodes();
                    break;
                }
                abilityCounts[upperCode]++;
                if (abilityCounts[upperCode] > 2) {
                    errorMsg += "Cannot have more than 2 copies of ability: " + string(1, upperCode);
                    break;
                }
            }
        }
        
        throw invalid_argument(errorMsg);
    }
    
    vector<unique_ptr<Ability>> abilities;
    for (char code : abilityString) {
        abilities.push_back(createAbility(code));
    }
    return abilities;
}

vector<unique_ptr<Ability>> AbilityFactory::getDefaultAbilities() {
    return createAbilities("LFDSP"); // LinkBoost, Firewall, Download, Scan, Polarize
}

bool AbilityFactory::isValidAbilityString(const string& abilityString) {
    if (abilityString.length() != 5) {
        return false;
    }
    
    // Count occurrences of each ability type
    map<char, int> abilityCounts;
    
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

string AbilityFactory::getValidAbilityCodes() {
    return "LFDSPJEA"; // LinkBoost, Firewall, Download, Scan, Polarize, Jam, 
}

string AbilityFactory::getAbilityName(char abilityCode) {
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
        case 'J':
        case 'j':
            return "Jam";
        case 'E':
        case 'e':
            return "Fog";
        case 'A':
        case 'a':
            return "AreaScan";
        default:
            return "Unknown";
    }
}

bool AbilityFactory::isValidAbilityCode(char abilityCode) {
    string validCodes = getValidAbilityCodes();
    return validCodes.find(toupper(abilityCode)) != string::npos;
}

string AbilityFactory::getUsageInfo() {
    return "Ability Selection Rules:\n"
           "- Must have exactly 5 abilities\n"
           "- Maximum 2 copies of each ability type\n"
           "- Valid codes: " + getValidAbilityCodes() + "\n"
           "- L: LinkBoost, F: Firewall, D: Download, S: Scan, P: Polarize, J: Jam, E: Fog, A: AreaScan\n"
           "- Examples: LFDSP (one of each), FFDDL (two firewalls, two downloads, one linkboost)";
}
