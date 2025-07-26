#include "ability.h"
using namespace std;

Ability::Ability(const string& name): name(name){}

Ability::~Ability() = default;

const string& Ability::getName() const {
    return name;
}
