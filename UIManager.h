#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Inventory.h"
#include "GroceryStore.h"
#include "Button.h"

class UIManager {
public:
    UIManager();
    void loadFont(const std::string& fontPath);
    void updateText(GroceryStore& groceryStore, Inventory inventory, bool includeHeader, bool includeFooter, bool includePrice, bool includeButton);
    void render(sf::RenderWindow& window);
    void setPos(float x, float y);
	void setText(const std::string& text);
    void handleClick(const sf::Vector2i& mousePos);

    void createProductButtons(GroceryStore& groceryStore, Inventory& inventory);


private:
    sf::Font font;
    sf::Text text;
	sf::Vector2f position;
    std::vector<Button> productButtons;  // Store buttons for products
    std::string generateInventoryString(const Inventory& inventory, bool includeHeader, bool includeFooter, bool includePrice = false, bool includeButton = false);
};

#endif
