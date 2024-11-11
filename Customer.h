#pragma once
#include <string>
#include <iostream>
#include <random>
#include <vector>
#include "Inventory.h"
#include "GroceryStore.h"
#include "UIManager.h"
#include "Animation.h"

enum class CustomerState {
    Idle,
    WalkingToQueue,
	IdleInQueue,
	WalkingToCashier,
	SendToCart,
	Leaving,
    Last
};

enum class EMood {
	Happy,
	Angry,
	Neutral,
    Last
};

enum class EMessages {
    Hello,
	NoStock,
	ThankYou,
    Last
};

class Customer {
public:
    Customer(const std::string& name);
	~Customer();

    std::string getName() const;

    void update(float deltaTime);  // Update the active animation
    void render(sf::RenderWindow& window);  // Render the active animation

    bool buyProduct(const std::string& productName, int quantity, Inventory& storeInventory);
    void displayInventory() const;
    void sendToCart(GroceryStore& groceryStore);

	void setMovementSpeed(float speed);
    void setPosition(const sf::Vector2f& pos);
	void setTargetCashierPosition(const sf::Vector2f& pos);
	void setTargetQueuePosition(const sf::Vector2f& pos);
	void setTargetLeavePosition(const sf::Vector2f& pos);
    sf::Vector2f getPosition() const;
    void moveTo(const sf::Vector2f& target);
	void updateMovement(float deltaTime);
	void setCustomerState(CustomerState state);
	CustomerState getCustomerState();
    bool hasReachedCashier();
    bool hasReachedQueue();
	bool hasReachedLeave();
    bool isMarkedForRemoval() const { return markedForRemoval; }
    void markForRemoval() { markedForRemoval = true; }

	float getPatience() const;
	void updatePatience(float deltaTime);


private:
    std::string name;
    Inventory inventory;
    Animation idleAnimation;
    Animation walkAnimation;

    // Movement variables
    sf::Vector2f startPos;
    sf::Vector2f targetPos;
	sf::Vector2f targetCashierPos;
	sf::Vector2f targetQueuePos;
	sf::Vector2f targetLeavePos;
    sf::Vector2f currentPos;
	float movementSpeed = 100.0f;
	CustomerState state;
	EMood mood;
    bool markedForRemoval = false;

	void loadRandomAnimations();
    UIManager message;
    float messageTimer = 0.f;
	bool shouldShowMessage;
	bool checkMessageTimer();

    void setCustomerMessage(EMessages message, std::string parameter);
	EMood setRandomCustomerMood();

	// Patience
	float patience = 0.0f;

    // Difficulty Settings
	float patienceMultiplier = 1.0f;
};