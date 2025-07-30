#ifndef ABILITYFACTORY_H
#define ABILITYFACTORY_H

#include "ability.h"
#include <vector>
#include <string>
#include <memory>
using namespace std;

class AbilityFactory {
public:
    // Create a single ability from its code
    static unique_ptr<Ability> createAbility(char abilityCode);
    
    // Create a set of abilities from a string of codes (e.g., "LFDSP")
    static vector<unique_ptr<Ability>> createAbilities(const string& abilityString);
    
    // Get the default ability set (LinkBoost, Firewall, Download, Scan, Polarize)
    static vector<unique_ptr<Ability>> getDefaultAbilities();
    
    // Validate an ability string
    static bool isValidAbilityString(const string& abilityString);
    
    // Get all valid ability codes
    static string getValidAbilityCodes();
    
    // Get ability name from code
    static string getAbilityName(char abilityCode);
    
    // Get usage information and rules
    static string getUsageInfo();

private:
    // Helper method to validate single ability code
    static bool isValidAbilityCode(char abilityCode);
};

#endif // ABILITYFACTORY_H 
