// UpgradeManager.cpp
#include "UpgradeManager.h"
#include "Game.h"
#include <iostream>
#include <fstream>
#include "json.hpp"  // Include JSON library (nlohmann/json)

using json = nlohmann::json;

bool UpgradeManager::loadUpgradesFromFile(const std::string& filename, Game& game) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Could not open config file.\n";
		return false;
	}

	json j;
	file >> j;

	for (const auto& item : j) {
		//std::cout << "Loading upgrade: " << item["name"] << "\n";
		std::string name = item["name"];
		float cost = item["cost"];
		float effect = item["effect"];
		float costMultiplier = item["costMultiplier"];
		float effectMultiplier = item["effectMultiplier"];

		if (name == "Customer Speed Boost") {
			//std::cout << "Loaded upgrade: " << name << "\n";
			upgrades.emplace_back(name, cost, effect, costMultiplier, effectMultiplier,
				[&game](float effect) {
					//std::cout << "Applying upgrade: " << effect << "\n";
					//std::cout << "Customers Get" << game.getCustomers().size() << "\n";
					game.setUpgradedMovementSpeed(effect);
				});
		}
		if (name == "Cheaper Stock Cost") {
			//std::cout << "Loaded upgrade: " << name << "\n";
			upgrades.emplace_back(name, cost, effect, costMultiplier, effectMultiplier,
				[&game](float effect) {
					//std::cout << "Applying upgrade: " << effect << "\n";
					game.getGroceryStore().setUpgradeStockCost(effect);
				});
		}
		if (name == "Increase Resale Price") {
			//std::cout << "Loaded upgrade: " << name << "\n";
			upgrades.emplace_back(name, cost, effect, costMultiplier, effectMultiplier,
				[&game](float effect) {
					//std::cout << "Applying upgrade: " << effect << "\n";
					game.getGroceryStore().setUpgradeResaleCost(effect);
				});
		}
		if (name == "Increase Queue Size") {
			//std::cout << "Loaded upgrade: " << name << "\n";
			upgrades.emplace_back(name, cost, effect, costMultiplier, effectMultiplier,
				[&game](float effect) {
					game.setUpgradeQueueLimit();
				});
		}
		if (name == "Auto Checkout") {
			//std::cout << "Loaded upgrade: " << name << "\n";
			upgrades.emplace_back(name, cost, effect, costMultiplier, effectMultiplier,
				[&game, this](float effect) {
					game.setUpgradeAutoCheckout();
				});
		}
	}
	return true;
}

void UpgradeManager::attemptUpgrade(size_t index, Game& game, UIManager& uiManager) {
	if (index >= upgrades.size()) {
		std::cerr << "Invalid upgrade index\n";
		return;
	}

	Upgrade& upgrade = upgrades[index];
	float cost = upgrade.getCurrentCost();

	if (game.getGroceryStore().getStoreCredit() >= cost) {
		// Deduct store credit and apply the upgrade if affordable
		game.getGroceryStore().deductStoreCredit(cost);
		upgrade.applyUpgrade();  // Apply upgrade effect

		std::cout << "Applied upgrade: " << upgrade.getName()
			<< " | Remaining store credit: $" << game.getGroceryStore().getStoreCredit() << "\n";

		// Refresh the UI to show updated cost
		uiManager.refreshUpgradeButtons(*this, game);
	}
	else {
		std::cout << "Not enough store credit for upgrade: " << upgrade.getName() << "\n";
	}
}



Upgrade& UpgradeManager::getUpgrade(size_t index) {
	return upgrades.at(index);
}

const std::vector<Upgrade>& UpgradeManager::getUpgrades() const {
	return upgrades;
}
