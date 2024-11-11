#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Inventory.h"
#include "GroceryStore.h"
#include "Button.h"
#include "UpgradeManager.h"

class Game; // Forward declaration

enum class UpdateOptions : uint8_t {
    None = 0,      // Default, no flags
    IncludeHeader = 1 << 0, // 0000 0001
    IncludeFooter = 1 << 1, // 0000 0010
    IncludePrice = 1 << 2, // 0000 0100
    IncludeButton = 1 << 3, // 0000 1000
    StorePrice = 1 << 4, // 0001 0000
    PaymentButton = 1 << 5,  // 0010 0000
	UpgradeButton = 1 << 6  // 0100 0000
};

// Enable bitwise operators for the enum
inline UpdateOptions operator|(UpdateOptions a, UpdateOptions b) {
    return static_cast<UpdateOptions>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline bool hasFlag(UpdateOptions value, UpdateOptions flag) {
    return (static_cast<uint8_t>(value) & static_cast<uint8_t>(flag)) != 0;
}

class UIManager {
public:
    UIManager();
    void loadFont(const std::string& fontPath);
    void updateText(GroceryStore& groceryStore, Inventory& inventory, UpdateOptions options);
    void render(sf::RenderWindow& window);
    void setPos(float x, float y);
	void setText(const std::string& text);
	void setFontSize(int size);
    void handleClick(const sf::Vector2i& mousePos);

    void createProductButtons(GroceryStore& groceryStore, Inventory& inventory);
	void createPaymentButton(GroceryStore& groceryStore);

    bool isPayButtonClicked(const sf::Vector2i& mousePos) const;

	void createUpgradeButtons(UpgradeManager& upgradeManager, Game& game);
    void refreshUpgradeButtons(UpgradeManager& upgradeManager, Game& game);

private:
    sf::Font font;
    sf::Text text;
	sf::Vector2f position;
    std::vector<Button> productButtons;  // Store buttons for products
	std::vector<Button> upgradeButtons;  // Store buttons for upgrades
    std::string generateInventoryString(GroceryStore& groceryStore, Inventory& inventory, bool includeHeader, bool includeFooter, bool includePrice = false, bool includeButton = false, bool storePrice = false, bool payemntButton = false);
};

