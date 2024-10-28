#include <SFML/Graphics.hpp>
#include "Stocks.h"
#include "GroceryStore.h"
#include "UIManager.h"
#include "Customer.h"
#include "NavBar.h"
#include <memory>  
#include <queue>
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

private:
    const int WINDOW_WIDTH = 1024;
    const int WINDOW_HEIGHT = 768;
    sf::RenderWindow window;
    Stocks stocks;
    GroceryStore groceryStore;

    std::vector<sf::Vector2f> queuePositions;
    sf::Vector2f cashierPosition = { 48.0f, 738.0f };
    bool isProcessing = false;  // Track if payment is in progress

    std::vector<std::shared_ptr<Customer>> customers;
    std::queue<std::shared_ptr<Customer>> customerQueue;

    std::shared_ptr<Customer> currentCustomer = nullptr;  // Track the customer at the cashier

    UIManager stockUI, storeUI, paymentUI, storeCreditUI;
    NavBar navBar;
    sf::Clock clock;
    bool showStockInfo, showStoreInfo, showPaymentInfo;

    void processQueue();  // Process the queue
    void setupQueuePositions();
    void handlePayment();  // New function to handle payment manually

    void shiftQueueForward();

    int nextQueueIndex = 0;  // Track the next available queue position
};



