#include "GroceryStore.h"
#include <iostream>

GroceryStore::GroceryStore(Stocks& stocks) : stockManager(stocks), coinAnimation(0.1f)
{
	//Load the store sprite
	if (!storeTexture.loadFromFile("assets/cashier/cashier.png")) {
		throw std::runtime_error("Failed to load store texture.");
	}
	storeSprite.setTexture(storeTexture);
	loadAnimation();
	coinAnimation.setPosition(sf::Vector2f({ 50, 730 }));
}

void GroceryStore::update(float deltaTime)
{
	if (showCoin)
	{
		coinAnimation.update(deltaTime);
		showCoinTimer += deltaTime;
		coinAnimation.setPosition({ coinAnimation.getPosition().x, coinAnimation.getPosition().y - 0.1f });
		if (showCoinTimer >= .5f)
		{
			showCoin = false;
			showCoinTimer = 0.0f;
			coinAnimation.setPosition(sf::Vector2f({ 50, 730 }));
		}
	}
}

void GroceryStore::render(sf::RenderWindow& window)
{
	window.draw(storeSprite);
	if (showCoin)
	{
		coinAnimation.render(window);
	}
}

void GroceryStore::buyStocks(const std::string& productName) {
	float localPrice = 0.0f;
	auto& products = stockManager.getInventory().getProducts();
	for (const auto& product : products) {
		if (product.getName() == productName) {
			//std::cout << "Buying stock: " << productName << std::endl;
			localPrice = (product.getPrice() - (product.getPrice() * stockCostMultiplier)) * product.getQuantity();
			//std::cout << "Price: " << localPrice << "Product Price: " << product.getPrice() << "Stock Cost Multiplier: " << stockCostMultiplier << std::endl;
			if (deductStoreCredit(localPrice)) {
				inventory.addProduct(product, product.getQuantity());
				return;
			}
			else {
				//std::cout << "Insufficient store credit to buy stock: " << productName << std::endl;
				return;
			}
		}
	}
	//std::cout << "Product not found in stock: " << productName << std::endl;
}

void GroceryStore::displayInventory() const {
	inventory.display();
}

void GroceryStore::addToCart(std::vector<Product>& products)
{
	for (const auto& product : products)
	{
		checkOut.addProduct(product, product.getQuantity());
	}
}

void GroceryStore::receivePayment()
{
	for (const auto& product : checkOut.getProducts())
	{
		//std::cout << "Receiving payment for: " << product.getName() << std::endl;
		storeCredit += product.getPrice() * resaleCostMultiplier * product.getQuantity();
		checkOut.clear();
		showCoin = true;
		coinSound.playSound("kaching");
	}
}

bool GroceryStore::deductStoreCredit(float amount)
{
	if (storeCredit >= amount) {
		storeCredit -= amount;
		return true;
	}
	else {
		std::cout << "Insufficient store credit.\n";
		return false;
	}
}

Inventory& GroceryStore::getInventory() {
	return inventory;
}

Inventory& GroceryStore::getCheckOutInventory()
{
	return checkOut;
}

float GroceryStore::getStoreCredit() const
{
	return storeCredit;
}

void GroceryStore::setPosition(const sf::Vector2f& pos)
{
	storeSprite.setPosition(pos);
}

void GroceryStore::setUpgradeStockCost(float multiplier)
{
	stockCostMultiplier = multiplier;
}

float GroceryStore::getStockCostMultiplier() const
{
	return stockCostMultiplier;
}

void GroceryStore::setUpgradeResaleCost(float multiplier)
{
	resaleCostMultiplier = multiplier;
}

float GroceryStore::getResaleCostMultiplier() const
{
	return resaleCostMultiplier;
}

void GroceryStore::loadAnimation()
{
	for (int i = 1; i <= 6; ++i) {
		std::string filename = "assets/coin/coin" + std::to_string(i) + ".png";
		coinAnimation.addFrame(filename);
	}
}
