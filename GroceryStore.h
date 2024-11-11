#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Product.h"
#include "Stocks.h"
#include "Sound.h"

class GroceryStore {
public:
    GroceryStore(Stocks& stocks);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    void buyStocks(const std::string& productName);
    void displayInventory() const;
    void addToCart(std::vector<Product>& products);
	void receivePayment();
    bool deductStoreCredit(float amount); // New method to handle deductions

    Inventory& getInventory();
    Inventory& getCheckOutInventory();
	float getStoreCredit() const;
	void setPosition(const sf::Vector2f& pos);

    // Upgrades
    void setUpgradeStockCost(float multiplier);
	float getStockCostMultiplier() const;

	void setUpgradeResaleCost(float multiplier);
	float getResaleCostMultiplier() const;

private:
    float storeCredit = 10000000;
    Inventory checkOut;
    Inventory inventory;
    Stocks& stockManager;
	sf::Sprite storeSprite;
	sf::Texture storeTexture;

	Animation coinAnimation;

    void loadAnimation();
    bool showCoin;
	float showCoinTimer;
    Sound coinSound;

	float stockCostMultiplier = 0.0f;
	float resaleCostMultiplier = 1.0f;
};