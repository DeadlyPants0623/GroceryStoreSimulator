#include <SFML/Graphics.hpp>
#include "Stocks.h"
#include "GroceryStore.h"
#include "UIManager.h"
#include "Customer.h"
#include "NavBar.h"
#include <vector>

class Game {
public:
    Game();
    void loadFonts(const std::string& fontPath);
    void run();
    void processEvents();
    void update(float deltaTime);
    void render();
    void showStocks();
    void showStore();
    void showPayment();
    std::string getStoreCreditString() const;
    void randomCustomer();
	void afterPayment();

private:
    const int WINDOW_WIDTH = 1024;
    const int WINDOW_HEIGHT = 768;
    sf::RenderWindow window;
    Stocks stocks;
    GroceryStore groceryStore;

    //std::vector<sf::Vector2f> queuePositions;
    sf::Vector2f cashierPosition = { 48.0f, 738.0f };
    bool isProcessing = false;  // Track if payment is in progress

    std::vector<std::shared_ptr<Customer>> customers;
    std::vector<std::shared_ptr<Customer>> customerQueue;

    std::shared_ptr<Customer> currentCustomer;
    std::shared_ptr<Customer> leavingCustomer;
    std::shared_ptr<Customer> customerGoingToCashier;

    UIManager stockUI, storeUI, paymentUI, storeCreditUI;
    NavBar navBar;
    sf::Clock clock;
    bool showStockInfo, showStoreInfo, showPaymentInfo;

	std::vector<sf::Vector2f> queuePositions;
	void initializeQueuePositions();

	bool isCashierAvailable() const;

	void processCustomerQueue();
    void shiftCustomerQueuePosition();

    float spawnTimer;
    float spawnInterval = 1;
};



