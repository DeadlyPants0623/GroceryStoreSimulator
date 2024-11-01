#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Product.h"
#include "Stocks.h"
#include "Sound.h"

class GroceryStore {
public:
    GroceryStore(const Stocks& stocks);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    void buyStocks(const std::string& productName);
    void displayInventory() const;
    void addToCart(const std::vector<Product>& products);
	void receivePayment();


    Inventory& getInventory();
    const Inventory& getCheckOutInventory() const;
	float getStoreCredit() const;
	void setPosition(const sf::Vector2f& pos);

private:
    float storeCredit = 1000000;
    Inventory checkOut;
    Inventory inventory;
    const Stocks& stockManager;
	sf::Sprite storeSprite;
	sf::Texture storeTexture;

	Animation coinAnimation;

    void loadAnimation();
    bool showCoin;
	float showCoinTimer;
    Sound coinSound;
};