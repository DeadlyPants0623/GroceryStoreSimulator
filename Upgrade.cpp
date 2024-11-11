// Upgrade.cpp
#include "Upgrade.h"
#include <iostream>

Upgrade::Upgrade(const std::string& upgradeName, float cost, float effect, float costMult, float effectMult, std::function<void(float)> callback)
    : name(upgradeName), level(1), baseCost(cost), baseEffect(effect),
    currentCost(cost), currentEffect(effect),
    costMultiplier(costMult), effectMultiplier(effectMult), callback(callback) {}

void Upgrade::applyUpgrade() {
    level++;
    currentCost *= costMultiplier;    // Increment cost by multiplier
    currentEffect *= effectMultiplier; // Increment effect by multiplier
	if (callback) callback(currentEffect);  // Apply the effect
    std::cout << "Upgraded " << name << " to level " << level << "\n";
}

std::string Upgrade::getName() const { return name; }
int Upgrade::getLevel() const { return level; }
float Upgrade::getCurrentCost() const { return currentCost; }
float Upgrade::getCurrentEffect() const { return currentEffect; }
