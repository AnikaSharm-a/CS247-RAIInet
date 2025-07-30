#pragma once
#include <vector>
#include <string>
#include <memory>
#include "ability.h"

using namespace std;

class AbilityFactory {
public:
    static unique_ptr<Ability> createAbility(char abilityCode);
    static vector<unique_ptr<Ability>> createAbilities(const string& abilityString);
    static vector<unique_ptr<Ability>> getDefaultAbilities();
    
    static bool isValidAbilityString(const string& abilityString);
    static string getValidAbilityCodes();
    static string getAbilityName(char abilityCode);
    static bool isValidAbilityCode(char abilityCode);
    static string getUsageInfo();
}; 