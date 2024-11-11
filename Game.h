#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Stocks.h"
#include "GroceryStore.h"
#include "UIManager.h"
#include "Customer.h"
#include "NavBar.h"
#include <vector>
#include "Sound.h"

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
    void showUpgrades();
    std::string getStoreCreditString() const;
    void randomCustomer();
	void afterPayment();

	std::vector<std::shared_ptr<Customer>> getCustomers() const;

    // Upgrades
	void setUpgradedMovementSpeed(float speed);
    void setUpgradeQueueLimit();
	void setUpgradeAutoCheckout();
	bool getAutoCheckout() const;

	GroceryStore& getGroceryStore();

private:
    const int WINDOW_WIDTH = 1920;
    const int WINDOW_HEIGHT = 1080;
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

    UIManager stockUI, storeUI, paymentUI, storeCreditUI, upgradesUI;
    NavBar navBar;
	UpgradeManager upgradeManager;
    sf::Clock clock;
    bool showStockInfo, showStoreInfo, showPaymentInfo, showUpgradesInfo;

    //Queue
	std::vector<sf::Vector2f> queuePositions;
	void initializeQueuePositions();
	int QueueLimit = 10;

	bool isCashierAvailable() const;

	void processCustomerQueue();
    void shiftCustomerQueuePosition();

    void RandomSpawnLoop();
    void setProgress(float progress);
    float spawnTimer;
    float spawnInterval = .1f;
	Sound buttonClick;

	// Patience Progress Bar
    sf::RectangleShape progressBarBackground;  // Background of the progress bar
    sf::RectangleShape progressBar;            // Foreground (progress) of the bar
	float progressBarXLocation = 700.0f ;          // Location of the progress bar
    float progressValue = 0.0f;                // Current progress value (0-100%)

	float totalCustomerPatience = 0.0f;

	float upgradedCustomerMovementSpeed = 100.0f;
	bool autoCheckout = false;

};