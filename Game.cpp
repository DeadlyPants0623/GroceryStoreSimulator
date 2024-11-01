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

	srand(static_cast<unsigned int>(time(0)));
	cashierPosition = { 50.0f, WINDOW_HEIGHT - 30.f };
	groceryStore.setPosition({0, WINDOW_HEIGHT - 65.0f});

	stocks.loadConfig("stock_config.txt");
	//groceryStore.buyStocks("Apple");
	//groceryStore.buyStocks("Banana");
	//groceryStore.buyStocks("Carrot");
	//groceryStore.buyStocks("Corn");
	//groceryStore.buyStocks("Grape");

	navBar.addButton("Stocks", [this]() { showStocks(); });
	navBar.addButton("Store", [this]() { showStore(); });
	navBar.addButton("Payment", [this]() { showPayment(); });

	initializeQueuePositions();

	loadFonts("pixel.ttf");
	randomCustomer();
	//randomCustomer();
	//randomCustomer();
	//randomCustomer();
	//randomCustomer();
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
	groceryStore.update(deltaTime);
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
	storeCreditUI.setPos(WINDOW_WIDTH - 400.f, 0);

	for (auto& customer : customers) {
		customer->update(deltaTime);
		switch (customer->getCustomerState()) {
		case CustomerState::WalkingToQueue: {
			if (customer->hasReachedQueue()) {
				customer->setCustomerState(CustomerState::IdleInQueue);
			}
			break;
		}
		case CustomerState::IdleInQueue: {
			if (isCashierAvailable()) {
				std::cout << "Cashier is available." << std::endl;
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
			int randomStock = rand() % 5;
			int randomQuantity = rand() % 5 + 1;
			if (customer->buyProduct(stockNames[randomStock], stockPrices[randomStock], randomQuantity, groceryStore.getInventory()))
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
	// After the loop, remove the marked customers
	customers.erase(
		std::remove_if(customers.begin(), customers.end(),
			[](const std::shared_ptr<Customer>& c) { return c->isMarkedForRemoval(); }),
		customers.end());

	spawnTimer += deltaTime;
	//RandomSpawnLoop();
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

	groceryStore.render(window);

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
	buttonClick.playSound("click");
}

void Game::showStore() {
	showStoreInfo = !showStoreInfo;
	showStockInfo = false;
	showPaymentInfo = false;
	storeUI.setPos(110, 50);
	buttonClick.playSound("click");
}

void Game::showPayment() {
	showPaymentInfo = !showPaymentInfo;
	showStockInfo = false;
	showStoreInfo = false;
	paymentUI.setPos(220, 50);
	buttonClick.playSound("click");
}

std::string Game::getStoreCreditString() const {
	std::ostringstream oss;
	oss << "Store Credit: $" << std::fixed << std::setprecision(2)
		<< groceryStore.getStoreCredit();
	return oss.str();
}

void Game::randomCustomer() {
	if (customerQueue.size() >= 9) {
		std::cout << "Queue is full." << std::endl;
		return;
	}

	auto customer = std::make_shared<Customer>("Customer " + std::to_string(customers.size()));
	std::cout << "Customer " << customers.size() << " has been added." << std::endl;

	customers.push_back(customer);
	customerQueue.push_back(customer);  // Store shared pointer

	for (int i = 0; i < customerQueue.size(); ++i) {
		if (customerQueue[i]->getName() == customer->getName())
		{
			std::cout << customer->getName() << " has entered the store." << std::endl;
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
		std::cout << "Payment received." << std::endl;
		currentCustomer->setTargetLeavePosition({ -50, WINDOW_HEIGHT - 30.f });
		currentCustomer->moveTo({ - 50, WINDOW_HEIGHT - 30.f });
		currentCustomer->setCustomerState(CustomerState::Leaving);
		currentCustomer = nullptr;
		customerGoingToCashier = nullptr;
	}
	else {
		std::cout << "No payment received." << std::endl;
	}
}

void Game::initializeQueuePositions()
{
	for (int i = 0; i < 10; ++i) {
		queuePositions.push_back({ 150.f + i * 48.f, WINDOW_HEIGHT - 30.f });
	}
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
		std::cout << "No customers in queue." << std::endl;
		return;
	}
	std::cout << "Processing customer queue." << std::endl;
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
			if (customerQueue.size() < 10) {
				randomCustomer();
			}
		}
		spawnTimer = 0;
	}
}