// Game.cpp
#include "Game.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <array>
#include <ctime>

constexpr std::array<const char*, 13> stockNames = { "Apple", "Banana", "Carrot", "Corn", "Grape", "Kiwi", "Orange", "Peach", "Pear", "Pineapple", "Strawberry", "Watermelon", "(HD)Grade"};

//enum class stockType {
//	Apple,
//	Banana,
//	Carrot,
//	Corn,
//	Grape,
//	Kiwi,
//	Orange,
//	Peach,
//	Pear,
//	Pineapple,
//	Strawberry,
//	Watermelon
//};

Game::Game()
	: window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Grocery Store Game"),
	groceryStore(stocks), showStockInfo(false), showStoreInfo(false), showPaymentInfo(false) {

	srand(static_cast<unsigned int>(time(0)));
	cashierPosition = { 50.0f, WINDOW_HEIGHT - 30.f };
	groceryStore.setPosition({0, WINDOW_HEIGHT - 65.0f});

	stocks.loadConfig("stock_config.txt");
	groceryStore.buyStocks("Apple");
	//groceryStore.buyStocks("Banana");
	//groceryStore.buyStocks("Carrot");
	//groceryStore.buyStocks("Corn");
	//groceryStore.buyStocks("Grape");

	navBar.addButton("Store", [this]() { showStocks(); });
	navBar.addButton("Stocks", [this]() { showStore(); });
	navBar.addButton("Payment", [this]() { showPayment(); });
	navBar.addButton("Upgrades", [this]() { showUpgrades(); });

	// Upgrades
	if (!upgradeManager.loadUpgradesFromFile("upgrades.json", *this)) {
		std::cerr << "Failed to load upgrades from file.\n";
	}
	else {
		//std::cout << "Loaded upgrades from file.\n";
	}

	upgradesUI.createUpgradeButtons(upgradeManager, *this);

	// Set up progress bar background
	progressBarBackground.setSize(sf::Vector2f(300, 40));   // Example size
	progressBarBackground.setFillColor(sf::Color(100, 100, 100)); // Gray background
	progressBarBackground.setPosition(progressBarXLocation, 0); // Position at bottom of the screen

	// Set up progress bar itself
	progressBar.setSize(sf::Vector2f(0, 40));  // Initial size with 0 progress
	progressBar.setFillColor(sf::Color::Green); // Green progress color
	progressBar.setPosition(progressBarXLocation, 0); // Same position as background

	initializeQueuePositions();

	loadFonts("pixel.ttf");
	//RandomSpawnLoop();
}

void Game::loadFonts(const std::string& fontPath) {
	stockUI.loadFont(fontPath);
	storeUI.loadFont(fontPath);
	paymentUI.loadFont(fontPath);
	storeCreditUI.loadFont(fontPath);
	upgradesUI.loadFont(fontPath);
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
			if (showStockInfo) {
				stockUI.handleClick(mousePos);
			}
			if (showPaymentInfo) {
				paymentUI.handleClick(mousePos);
			}
			if (showUpgradesInfo) {
				upgradesUI.handleClick(mousePos);
			}
			if (paymentUI.isPayButtonClicked(mousePos)) {
				afterPayment();
			}
		}
		else if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Space) {
				randomCustomer();
			}
		}
	}
}

void Game::update(float deltaTime) {
	// Update the grocery store
	groceryStore.update(deltaTime);
	if (showStockInfo) {
		stockUI.updateText(groceryStore, stocks.getInventory(), UpdateOptions::IncludePrice | UpdateOptions::IncludeButton);
	}
	// Update the store UI
	if (showStoreInfo) {
		storeUI.updateText(groceryStore, groceryStore.getInventory(), UpdateOptions::IncludePrice | UpdateOptions::StorePrice);
	}
	// Update the payment UI
	if (showPaymentInfo) {
		paymentUI.updateText(groceryStore, groceryStore.getCheckOutInventory(), UpdateOptions::IncludeHeader | UpdateOptions::IncludeFooter | UpdateOptions::StorePrice | UpdateOptions::PaymentButton);
	}
	// Update the store credit UI
	storeCreditUI.setText(getStoreCreditString());
	storeCreditUI.setPos(WINDOW_WIDTH - 300.f, 0);

	// Update the customers
	for (auto& customer : customers) {
		customer->update(deltaTime);
		totalCustomerPatience += customer->getPatience();
		switch (customer->getCustomerState()) {
		case CustomerState::WalkingToQueue: {
			if (customer->hasReachedQueue()) {
				customer->setCustomerState(CustomerState::IdleInQueue);
			}
			break;
		}
		case CustomerState::IdleInQueue: {
			if (isCashierAvailable()) {
				//std::cout << "Cashier is available." << std::endl;
				processCustomerQueue();
			}
			break;
		}
		case CustomerState::WalkingToCashier: {
			if (customer->hasReachedCashier()) {
				currentCustomer = customer;
				customer->setCustomerState(CustomerState::SendToCart);
			}
			break;
		}
		case CustomerState::SendToCart: {
			srand(static_cast<unsigned int>(time(0)));
			int randomStock = rand() % 13;
			int randomQuantity = rand() % 10 + 1;
			if (customer->buyProduct(stockNames[randomStock], randomQuantity, groceryStore.getInventory()))
			{
				customer->sendToCart(groceryStore);
				customer->setCustomerState(CustomerState::Idle);
			}
			else {
				customer->setTargetLeavePosition({ -50, WINDOW_HEIGHT - 30.f });
				customer->moveTo({ -50, WINDOW_HEIGHT - 30.f });
				currentCustomer = nullptr;
				customerGoingToCashier = nullptr;
				customer->setCustomerState(CustomerState::Leaving);
			}
			break;
		}
		case CustomerState::Leaving: {
			if (customer->getCustomerState() == CustomerState::Leaving && customer->hasReachedLeave()) {
				customer->markForRemoval();
			}
			break;
		}
		}
	}
	// After the loop
	// Set patience
	//std::cout << "Total Customer Patience: " << totalCustomerPatience << std::endl;
	setProgress(totalCustomerPatience / customers.size());

	// Reset Customer Patience
	totalCustomerPatience = 0;

	// Remove the marked customers
	customers.erase(
		std::remove_if(customers.begin(), customers.end(),
			[](const std::shared_ptr<Customer>& c) { return c->isMarkedForRemoval(); }),
		customers.end());

	spawnTimer += deltaTime;
	RandomSpawnLoop();
	if (currentCustomer && autoCheckout && currentCustomer->getCustomerState() == CustomerState::Idle) {
		groceryStore.receivePayment();
		afterPayment();
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
	if (showUpgradesInfo) {
		upgradesUI.render(window);
	}
	storeCreditUI.render(window);

	groceryStore.render(window);

	for (auto& customer : customers) {
		customer->render(window);
	}

	// Draw the progress bar
	window.draw(progressBarBackground);
	window.draw(progressBar);   

	window.display();
}

void Game::showStocks() {
	showStockInfo = !showStockInfo;
	showStoreInfo = false;
	showPaymentInfo = false;
	showUpgradesInfo = false;
	stockUI.setPos(0, 50);
	buttonClick.playSound("click");
}

void Game::showStore() {
	showStoreInfo = !showStoreInfo;
	showStockInfo = false;
	showPaymentInfo = false;
	showUpgradesInfo = false;
	storeUI.setPos(110, 50);
	buttonClick.playSound("click");
}

void Game::showPayment() {
	showPaymentInfo = !showPaymentInfo;
	showStockInfo = false;
	showStoreInfo = false;
	showUpgradesInfo = false;
	paymentUI.setPos(220, 50);
	buttonClick.playSound("click");
}

void Game::showUpgrades()
{
	showUpgradesInfo = !showUpgradesInfo;
	showPaymentInfo = false;
	showStockInfo = false;
	showStoreInfo = false;
	if (showUpgradesInfo) {
		upgradesUI.createUpgradeButtons(upgradeManager, *this);
	}
	buttonClick.playSound("click");
}

std::string Game::getStoreCreditString() const {
	std::ostringstream oss;
	oss << "Store Credit: $" << std::fixed << std::setprecision(2)
		<< groceryStore.getStoreCredit();
	return oss.str();
}

void Game::randomCustomer() {
	if (customerQueue.size() >= QueueLimit) {
		//std::cout << "Queue is full." << std::endl;
		return;
	}

	auto customer = std::make_shared<Customer>("Customer " + std::to_string(customers.size()));
	//std::cout << "Customer " << customers.size() << " has been added." << std::endl;

	customers.push_back(customer);
	customerQueue.push_back(customer);  // Store shared pointer

	for (int i = 0; i < customerQueue.size(); ++i) {
		if (customerQueue[i]->getName() == customer->getName())
		{
			//std::cout << customer->getName() << " has entered the store." << std::endl;
			customer->setMovementSpeed(upgradedCustomerMovementSpeed);
			customer->setPosition(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT - 30));
			customer->setTargetQueuePosition(queuePositions[i]);
			customer->moveTo(queuePositions[i]);
			customer->setCustomerState(CustomerState::WalkingToQueue);
		}
	}
}

void Game::afterPayment()
{
	if (currentCustomer) {
		//std::cout << "Payment received." << std::endl;
		currentCustomer->setTargetLeavePosition({ -50, WINDOW_HEIGHT - 30.f });
		currentCustomer->moveTo({ - 50, WINDOW_HEIGHT - 30.f });
		currentCustomer->setCustomerState(CustomerState::Leaving);
		currentCustomer = nullptr;
		customerGoingToCashier = nullptr;
	}
	else {
		//std::cout << "No payment received." << std::endl;
	}
}

std::vector<std::shared_ptr<Customer>> Game::getCustomers() const
{
	return customers;
}

void Game::setUpgradedMovementSpeed(float speed)
{
	upgradedCustomerMovementSpeed = speed;
	for (auto& customer : customers) {
		customer->setMovementSpeed(speed);
	}
}

GroceryStore& Game::getGroceryStore()
{
	return groceryStore;
}

void Game::initializeQueuePositions()
{
	queuePositions.clear();
	for (int i = 0; i < QueueLimit; ++i) {
		queuePositions.push_back({ 150.f + i * 48.f, WINDOW_HEIGHT - 30.f });
	}
}

void Game::setUpgradeQueueLimit()
{
	QueueLimit++;
	initializeQueuePositions();
}

void Game::setUpgradeAutoCheckout()
{
	autoCheckout = true;
}

bool Game::getAutoCheckout() const
{
	return autoCheckout;
}

bool Game::isCashierAvailable() const
{
	if (!customerGoingToCashier) {
		return true;
	}
	return false;
}

void Game::processCustomerQueue()
{
	if (customerQueue.empty()) {
		//std::cout << "No customers in queue." << std::endl;
		return;
	}
	//std::cout << "Processing customer queue." << std::endl;
	auto customer = customerQueue.front();
	customerQueue.erase(customerQueue.begin());
	customerGoingToCashier = customer;
	customer->setCustomerState(CustomerState::WalkingToCashier);
	customer->setTargetCashierPosition(cashierPosition);
	customer->moveTo(cashierPosition);
	shiftCustomerQueuePosition();
}

void Game::shiftCustomerQueuePosition()
{
	for (int i = 0; i < customerQueue.size(); ++i) {
		customerQueue[i]->setTargetQueuePosition(queuePositions[i]);
		customerQueue[i]->moveTo(queuePositions[i]);
		customerQueue[i]->setCustomerState(CustomerState::WalkingToQueue);
	}
}

void Game::RandomSpawnLoop()
{
	if (spawnTimer >= spawnInterval)
	{
		int randomChance = rand() % 100;
		if (randomChance <= 50) {
			if (customerQueue.size() < QueueLimit) {
				randomCustomer();
			}
		}
		spawnTimer = 0;
	}
}

void Game::setProgress(float progress) {
	// Clamp the progress value between 0 and 100
	progressValue = std::max(0.0f, std::min(progress, 100.0f));
	// Update the width of the progress bar based on progress percentage
	float width = progressBarBackground.getSize().x * (progressValue / 100.0f);
	progressBar.setSize(sf::Vector2f(width, progressBarBackground.getSize().y));
}