#include "ability.h"
using namespace std;

Ability::Ability(const string& name): name(name){}

// returns the name of the ability
const string& Ability::getName() const {
    return name;
}
