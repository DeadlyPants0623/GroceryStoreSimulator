#include "Customer.h"
#include <iostream>
#include <random>
#include <functional>

Customer::Customer(const std::string& name)
	: name(name),
	idleAnimation(0.2f),  // 0.2s per frame for idle animation
	walkAnimation(0.1f)   // 0.1s per frame for walk animation
{
	loadRandomAnimations();
}

std::string Customer::getName() const { return name; }

void Customer::update(float deltaTime) {
	updateMovement(deltaTime);

	switch (state) {
	case CustomerState::Idle:
		idleAnimation.update(deltaTime);
		break;
	case CustomerState::WalkingToQueue:
		walkAnimation.update(deltaTime);
		break;
	case CustomerState::IdleInQueue:
		idleAnimation.update(deltaTime);
		break;
	case CustomerState::WalkingToCashier:
		walkAnimation.update(deltaTime);
		break;
	case CustomerState::SendToCart:
		break;
	case CustomerState::Leaving:
		walkAnimation.update(deltaTime);
		break;
	default:
		break;
	}
}

void Customer::render(sf::RenderWindow& window) {
	switch (state) {
	case CustomerState::Idle:
		idleAnimation.render(window);
		break;
	case CustomerState::WalkingToQueue:
		walkAnimation.render(window);
		break;
	case CustomerState::IdleInQueue:
		idleAnimation.render(window);
		break;
	case CustomerState::WalkingToCashier:
		walkAnimation.render(window);
		break;
	case CustomerState::SendToCart:
		break;
	case CustomerState::Leaving:
		walkAnimation.render(window);
		break;
	default:
		break;
	}
}

void Customer::buyProduct(const std::string& productName, float productPrice, int quantity, Inventory& storeInventory) {
	if (storeInventory.removeProduct(productName, quantity)) {
		std::cout << name << " bought " << quantity << " of " << productName << std::endl;
		Product product(productName, productPrice);
		inventory.addProduct(product, quantity);
	}
}

void Customer::displayInventory() const {
	//std::cout << name << "'s Inventory:" << std::endl;
	inventory.display();
}

void Customer::sendToCart(GroceryStore& groceryStore) const
{
	groceryStore.addToCart(inventory.getProducts());
}

void Customer::setPosition(const sf::Vector2f& pos) {
	startPos = targetPos = pos;
	idleAnimation.setPosition(pos);
	walkAnimation.setPosition(pos);
}

void Customer::setTargetCashierPosition(const sf::Vector2f& pos)
{
	targetCashierPos = pos;
}

void Customer::setTargetQueuePosition(const sf::Vector2f& pos)
{
	targetQueuePos = pos;
}

void Customer::setTargetLeavePosition(const sf::Vector2f& pos)
{
	targetLeavePos = pos;
}

sf::Vector2f Customer::getPosition() const {
	return idleAnimation.getPosition();
}

void Customer::moveTo(const sf::Vector2f& target) {
	std::cout << name << " is moving to " << target.x << ", " << target.y << std::endl;
	startPos = currentPos = idleAnimation.getPosition();  // Get current position
	targetPos = target;
}

void Customer::updateMovement(float deltaTime) {
	// Calculate the direction vector
	sf::Vector2f direction = targetPos - currentPos;
	float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	// Normalize the direction vector
	if (distance != 0) {
		direction /= distance;
	}

	// Calculate the distance to move this frame
	float moveDistance = movementSpeed * deltaTime;

	// Check if we reached the target or will reach it in this frame
	if (moveDistance >= distance) {
		currentPos = targetPos;  // Snap to target
	}
	else {
		// Move in the direction of the target
		currentPos += direction * moveDistance;
	}

	// Update animation position
	idleAnimation.setPosition(currentPos);
	walkAnimation.setPosition(currentPos);
}

void Customer::setCustomerState(CustomerState state)
{
	this->state = state;

	switch (state) {
	case CustomerState::Idle:
		idleAnimation.reset();
		break;
	case CustomerState::WalkingToQueue:
		walkAnimation.reset();
		break;
	case CustomerState::IdleInQueue:
		idleAnimation.reset();
		break;
	case CustomerState::WalkingToCashier:
		walkAnimation.reset();
		break;
	case CustomerState::SendToCart:
		break;
	case CustomerState::Leaving:
		walkAnimation.reset();
		break;
	default:
		break;
	}
}

CustomerState Customer::getCustomerState()
{
	return state;
}

void Customer::loadRandomAnimations()
{
	std::random_device rd;     // Random seed
	std::mt19937 rng(rd());    // Mersenne Twister RNG
	std::uniform_int_distribution<int> dist(1, 2);  // Random number from 1 to 2

	int animationType = dist(rng);  // Get random number (1 or 2)

	// Use switch to load the appropriate animation set
	switch (animationType) {
	case 1:
		// Load type 1 animations
		for (int i = 1; i <= 4; ++i) {
			idleAnimation.addFrame("assets/type1/Idle" + std::to_string(i) + ".png");
		}
		for (int i = 1; i <= 6; ++i) {
			walkAnimation.addFrame("assets/type1/Walk" + std::to_string(i) + ".png");
		}
		break;

	case 2:
		// Load type 2 animations
		for (int i = 1; i <= 4; ++i) {
			idleAnimation.addFrame("assets/type2/Idle" + std::to_string(i) + ".png");
		}
		for (int i = 1; i <= 6; ++i) {
			walkAnimation.addFrame("assets/type2/Walk" + std::to_string(i) + ".png");
		}
		break;

		// Optional: Handle unexpected cases if more sets are added in the future
	default:
		std::cerr << "Unknown animation type: " << animationType << std::endl;
		break;
	}

	std::cout << "Loaded animation type: " << animationType << std::endl;
}

bool Customer::hasReachedCashier()
{
	if (walkAnimation.getPosition() == targetCashierPos) {
		std::cout << name << " has reached cashier." << std::endl;
		return true;
	}
	return false;
}

bool Customer::hasReachedQueue()
{
	if (walkAnimation.getPosition() == targetQueuePos) {
		std::cout << name << " has reached queue." << std::endl;
		return true;
	}
	return false;
}

bool Customer::hasReachedLeave()
{
	if (walkAnimation.getPosition() == targetLeavePos) {
		std::cout << name << " has left the store." << std::endl;
		return true;
	}
	return false;
}

Customer::~Customer() {
	std::cout << name << " is being destroyed." << std::endl;

	// Ensure the animation resources are cleared
	idleAnimation.clear();
	walkAnimation.clear();
}
