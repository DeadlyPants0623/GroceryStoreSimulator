#include <string>
#include <functional>

class Upgrade {
private:
    std::string name;
    int level;
    float baseCost;
    float baseEffect;
    float currentCost;
    float currentEffect;
    float costMultiplier;
    float effectMultiplier;
    std::function<void(float)> callback;  // Callback to apply the effect

public:
    // Constructor with multipliers
    Upgrade(const std::string& upgradeName, float cost, float effect, float costMult, float effectMult, std::function<void(float)> callback);

    // Apply upgrade
    void applyUpgrade();

    // Getters
    std::string getName() const;
    int getLevel() const;
    float getCurrentCost() const;
    float getCurrentEffect() const;
};