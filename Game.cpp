// Game.cpp
#include "Game.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <array>
#include <ctime>

constexpr std::array<const char*, 5> stockNames = { "Apple", "Banana", "Carrot", "Corn", "Grape" };
constexpr std::array<float, 5> stockPrices = { 1.0f, 0.9f, 1.5f, 2.0f, 4.0f };

enum class stockType {
	Apple,
	Banana,
	Carrot,
	Corn,
	Grape
};

Game::Game()
	: window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Grocery Store Game"),
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
	setupQueuePositions();
	randomCustomer();
	randomCustomer();
	randomCustomer();
	randomCustomer();
	randomCustomer();
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
		float deltaTime = clock.restart().asSeconds();
		update(deltaTime);
		render();
	}
}

void Game::processEvents() {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}
		else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
			auto mousePos = sf::Mouse::getPosition(window);
			navBar.handleClick(mousePos);
			stockUI.handleClick(mousePos);
			paymentUI.handleClick(mousePos);

			// Check if the "Pay" button was clicked
			if (paymentUI.isPayButtonClicked(mousePos)) {
				handlePayment();  // Process payment manually
			}
		}
	}
}

void Game::update(float deltaTime) {
	if (showStockInfo) {
		stockUI.updateText(groceryStore, stocks.getInventory(), UpdateOptions::IncludePrice | UpdateOptions::IncludeButton);
	}
	if (showStoreInfo) {
		storeUI.updateText(groceryStore, groceryStore.getInventory(), UpdateOptions::IncludePrice | UpdateOptions::StorePrice);
	}
	if (showPaymentInfo) {
		paymentUI.updateText(groceryStore, groceryStore.getCheckOutInventory(), UpdateOptions::IncludeHeader | UpdateOptions::IncludeFooter | UpdateOptions::StorePrice | UpdateOptions::PaymentButton);
	}
	storeCreditUI.setText(getStoreCreditString());
	storeCreditUI.setPos(1600, 0);

	for (auto& customer : customers) {
		customer->update(deltaTime);
	}
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

	for (auto& customer : customers) {
		customer->render(window);
	}

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
	auto customer = std::make_shared<Customer>("Customer " + std::to_string(customers.size() + 1));

	// Start the customer off-screen at the right edge
	customer->setPosition(sf::Vector2f(WINDOW_WIDTH - 48, WINDOW_HEIGHT - 30));

	// Assign the next available queue position
	if (nextQueueIndex < queuePositions.size()) {
		sf::Vector2f targetPosition = queuePositions[nextQueueIndex++];
		std::cout << customer->getName() << " is assigned to queue position " << (nextQueueIndex - 1) << std::endl;

		// Move the customer to their assigned queue position
		customer->moveTo(targetPosition, 2.0f, [this, customer]() {
			std::cout << customer->getName() << " arrived at queue position." << std::endl;
			customerQueue.push(customer);  // Add to the queue

			// Start processing the queue if this is the first customer
			if (customerQueue.size() == 1) {
				processQueue();
			}
			});
	}
	else {
		std::cerr << "No available queue positions left!" << std::endl;
	}

	customers.push_back(customer);  // Keep track of the customer
}

void Game::processQueue() {
    if (isProcessing || customerQueue.empty()) {
        return;  // Don't proceed if a payment is in progress or queue is empty
    }

    isProcessing = true;  // Set the flag indicating a customer is being processed

    currentCustomer = customerQueue.front();  // Get the first customer in the queue
    std::cout << currentCustomer->getName() << " is moving to the cashier." << std::endl;

    // Move the customer to the cashier position
    currentCustomer->moveTo(cashierPosition, 1.0f, [this]() {
        std::cout << currentCustomer->getName() << " arrived at the cashier." << std::endl;

        // Customer buys products and sends them to the cart upon arrival
        int randomNum = rand() % stockNames.size();
        int randomAmount = rand() % 10 + 1;

        currentCustomer->buyProduct(stockNames[randomNum], stockPrices[randomNum], randomAmount, groceryStore.getInventory());
        std::cout << currentCustomer->getName() << " bought " << randomAmount << " of " << stockNames[randomNum] << "." << std::endl;

        currentCustomer->sendToCart(groceryStore);
        std::cout << currentCustomer->getName() << " sent products to the cart." << std::endl;

        // Now wait for the "Pay" button to be clicked
        isProcessing = false;  // Allow the payment to proceed
    });
}


void Game::setupQueuePositions() {
	int startX = 100;  // Starting X position of the queue
	int startY = WINDOW_HEIGHT - 30;  // Aligned horizontally

	// Create 10 queue positions spaced 60 pixels apart
	for (int i = 0; i < 10; ++i) {
		queuePositions.emplace_back(sf::Vector2f(startX + i * 60, startY));
	}
}

void Game::handlePayment() {
	if (!currentCustomer) {
		std::cout << "No customer at the cashier to pay." << std::endl;
		return;
	}

	std::cout << "Processing payment for " << currentCustomer->getName() << std::endl;

	// Simulate payment logic
	std::cout << currentCustomer->getName() << " has completed payment." << std::endl;

	// Remove the current customer from the queue
	customerQueue.pop();
	std::cout << currentCustomer->getName() << " has finished and left the queue." << std::endl;

	// Clear the current customer and reset the processing flag
	currentCustomer = nullptr;
	isProcessing = false;

	// Shift the remaining customers forward in the queue
	shiftQueueForward();

	// Start processing the next customer if available
	if (!customerQueue.empty()) {
		processQueue();
	}
}

void Game::shiftQueueForward() {
	if (customerQueue.empty()) return;  // Nothing to shift if the queue is empty

	std::queue<std::shared_ptr<Customer>> tempQueue = customerQueue;  // Copy the queue
	int index = 0;

	// Iterate over the temporary queue and move each customer to the correct position
	while (!tempQueue.empty() && index < queuePositions.size()) {
		auto customer = tempQueue.front();
		tempQueue.pop();  // Remove the processed customer from the temp queue

		sf::Vector2f targetPosition = queuePositions[index++];
		customer->moveTo(targetPosition, 1.0f, []() {
			std::cout << "Customer shifted forward in the queue." << std::endl;
			});
	}
}
