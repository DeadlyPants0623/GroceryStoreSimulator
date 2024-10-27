#include "Game.h"
#include <iostream>
#include <iomanip>
#include <sstream>

Game::Game()
	: window(sf::VideoMode(1920, 1080), "Grocery Store Game"),
	groceryStore(stocks), showStockInfo(false), showStoreInfo(false), showPaymentInfo(false) {

	stocks.loadConfig("stock_config.txt");
	groceryStore.buyStocks("Apple");
	Customer customer1("Alice");
	customer1.buyProduct("Apple", 1.0f, 2, groceryStore.getInventory());
	customer1.sendToCart(groceryStore);

	navBar.addButton("Stocks", [this]() { showStocks(); });
	navBar.addButton("Store", [this]() { showStore(); });
	navBar.addButton("Payment", [this]() { showPayment(); });

	stockUI.loadFont("pixel.ttf");
	storeUI.loadFont("pixel.ttf");
	paymentUI.loadFont("pixel.ttf");
	storeCreditUI.loadFont("pixel.ttf");
}

void Game::run() {
	while (window.isOpen()) {
		processEvents();
		update();
		render();
	}
}

void Game::processEvents() {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}
		else if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				navBar.handleClick(sf::Mouse::getPosition(window));
				stockUI.handleClick(sf::Mouse::getPosition(window));
			}
		}
	}
}

void Game::update() {
	if (showStockInfo) {
		stockUI.updateText(groceryStore, stocks.getInventory(), false, false, true, true);
	}
	if (showStoreInfo) {
		storeUI.updateText(groceryStore, groceryStore.getInventory(), false, false, false, false);
	}
	if (showPaymentInfo) {
		paymentUI.updateText(groceryStore, groceryStore.getCheckOutInventory(), true, true, true, false);
	}
	storeCreditUI.setText(getStoreCreditString());
	storeCreditUI.setPos(1700, 0);
}


void Game::render() {
	window.clear(sf::Color::Black);
	navBar.render(window);

	if (showStockInfo) {
		stockUI.render(window);
	}
	if (showStoreInfo) {
		storeUI.render(window);
	}
	if (showPaymentInfo) {
		paymentUI.render(window);
	}
	storeCreditUI.render(window);

	window.display();
}

void Game::showStocks() {
	//std::cout << "Stocks" << std::endl;
	showStockInfo = !showStockInfo;
	showStoreInfo = false;
	showPaymentInfo = false;
	stockUI.setPos(0, 50);
}

void Game::showStore() {
	//std::cout << "Store" << std::endl;
	showStoreInfo = !showStoreInfo;
	showStockInfo = false;
	showPaymentInfo = false;
	storeUI.setPos(110, 50);
}

void Game::showPayment() {
	//std::cout << "Payment" << std::endl;
	showPaymentInfo = !showPaymentInfo;
	showStockInfo = false;
	showStoreInfo = false;
	paymentUI.setPos(220, 50);
}

std::string Game::getStoreCreditString() const
{
	// Format the store credit with two decimal places
	std::ostringstream oss;
	oss << "Store Credit: $" << std::fixed << std::setprecision(2)
		<< groceryStore.getStoreCredit();

	return oss.str();
}
