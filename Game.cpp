// Game.cpp
#include "Game.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <array>

constexpr std::array<const char*, 5> stockNames = {"Apple", "Banana", "Carrot", "Corn", "Grape"};
constexpr std::array<float, 5> stockPrices = { 1.0f, 0.9f, 1.5f, 2.0f, 4.0f };

enum class stockType {
    Apple,
    Banana,
    Carrot,
    Corn,
    Grape
};

Game::Game()
    : window(sf::VideoMode(1920, 1080), "Grocery Store Game"),
    groceryStore(stocks), showStockInfo(false), showStoreInfo(false), showPaymentInfo(false) {

    stocks.loadConfig("stock_config.txt");
    groceryStore.buyStocks("Apple");
	groceryStore.buyStocks("Banana");
	groceryStore.buyStocks("Carrot");
	groceryStore.buyStocks("Corn");
	groceryStore.buyStocks("Grape");

    navBar.addButton("Stocks", [this]() { showStocks(); });
    navBar.addButton("Store", [this]() { showStore(); });
    navBar.addButton("Payment", [this]() { showPayment(); });

    loadFonts("pixel.ttf");
	randomCustomer();
}

void Game::loadFonts(const std::string& fontPath) {
    stockUI.loadFont(fontPath);
    storeUI.loadFont(fontPath);
    paymentUI.loadFont(fontPath);
    storeCreditUI.loadFont(fontPath);
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
        } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            auto mousePos = sf::Mouse::getPosition(window);
            navBar.handleClick(mousePos);
            stockUI.handleClick(mousePos);
            paymentUI.handleClick(mousePos);
        }
    }
}

void Game::update() {
    if (showStockInfo) {
        stockUI.updateText(groceryStore, stocks.getInventory(), false, false, true, true, false, false);
    }
    if (showStoreInfo) {
        storeUI.updateText(groceryStore, groceryStore.getInventory(), false, false, true, false, true, false);
    }
    if (showPaymentInfo) {
        paymentUI.updateText(groceryStore, groceryStore.getCheckOutInventory(), true, true, true, false, true, true);
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
    showStockInfo = !showStockInfo;
    showStoreInfo = false;
    showPaymentInfo = false;
    stockUI.setPos(0, 50);
}

void Game::showStore() {
    showStoreInfo = !showStoreInfo;
    showStockInfo = false;
    showPaymentInfo = false;
    storeUI.setPos(110, 50);
}

void Game::showPayment() {
    showPaymentInfo = !showPaymentInfo;
    showStockInfo = false;
    showStoreInfo = false;
    paymentUI.setPos(220, 50);
}

std::string Game::getStoreCreditString() const {
    std::ostringstream oss;
    oss << "Store Credit: $" << std::fixed << std::setprecision(2)
        << groceryStore.getStoreCredit();
    return oss.str();
}

void Game::randomCustomer() {
	srand(time(0));
	int randomTimes = rand() % 5;
	std::cout << "Random times: " << randomTimes << std::endl;
	for (int i = 0; i < randomTimes; ++i) {
        int randomNum = rand() % stockNames.size();
        int randomAmount = rand() % 10;

        Customer customer("Customer");
        customer.buyProduct(stockNames[randomNum], stockPrices[randomNum], randomAmount, groceryStore.getInventory());
        customer.sendToCart(groceryStore);
	}
}
