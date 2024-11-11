#pragma once
#include "Upgrade.h"
#include <vector>
#include <string>

class Game;
class UIManager;

class UpgradeManager {
private:
    std::vector<Upgrade> upgrades;

public:
    bool loadUpgradesFromFile(const std::string& filename, Game& game);
    void attemptUpgrade(size_t index, Game& game, UIManager& uiManager);

    // Accessor
    Upgrade& getUpgrade(size_t index);
    const std::vector<Upgrade>& getUpgrades() const;
};