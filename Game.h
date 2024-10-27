#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Stocks.h"
#include "GroceryStore.h"
#include "UIManager.h"
#include "Customer.h"
#include "NavBar.h"

class Game {
public:
    Game();
    void run();
    void processEvents();
    void update();
    void render();
    void showStocks();
    void showStore();
    void showPayment();
	std::string getStoreCreditString() const;

private:
    sf::RenderWindow window;
    Stocks stocks;
    GroceryStore groceryStore;
	//Customer customer1;
	//Customer customer2;
    UIManager stockUI;
    UIManager storeUI;
    UIManager paymentUI;
	UIManager storeCreditUI;
    NavBar navBar;
    bool showStockInfo;
    bool showStoreInfo;
    bool showPaymentInfo;
};

#endif
