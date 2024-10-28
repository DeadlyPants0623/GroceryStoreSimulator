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

sf::Vector2f Customer::getPosition() const {
    return idleAnimation.getPosition();
}

void Customer::moveTo(const sf::Vector2f& target, float duration, std::function<void()> onArrival) {
    startPos = idleAnimation.getPosition();  // Get current position
    targetPos = target;
    totalDuration = duration;
    elapsedTime = 0.0f;
    moving = true;
    setWalking(true);  // Start walking animation
    onArrivalCallback = onArrival;  // Store the callback
}

void Customer::exitAndDestroy(std::function<void(std::shared_ptr<Customer>)> onDestroyCallback) {
    // Stop animations before exiting
    idleAnimation.stop();
    walkAnimation.stop();

    markForDestruction();  // Mark the customer as destroyed

    sf::Vector2f offScreenPosition(0, getPosition().y);

    moveTo(offScreenPosition, 2.0f, [this, onDestroyCallback]() {
        std::cout << name << " has exited the store and will be destroyed." << std::endl;

        // Call the destruction callback to remove the customer from the game
        onDestroyCallback(shared_from_this());
        });
}


void Customer::setWalking(bool walking) {
    if (isWalking != walking) {
        isWalking = walking;

        // Reset animation to the first frame when switching states
        if (isWalking) walkAnimation.reset();
        else idleAnimation.reset();
    }
}

void Customer::update(float deltaTime) {
    if (!moving) return;  // Skip update if the customer is not active

    elapsedTime += deltaTime;
    float progress = elapsedTime / totalDuration;

    if (progress >= 1.0f) {
        progress = 1.0f;
        moving = false;
        setWalking(false);  // Stop walking animation

        // Call the on-arrival callback if it exists
        if (onArrivalCallback) {
            std::cout << "Calling on-arrival callback" << std::endl;
            onArrivalCallback();
        }
    }

    // Interpolate between start and target positions
    sf::Vector2f newPos = startPos + (targetPos - startPos) * progress;
    idleAnimation.setPosition(newPos);
    walkAnimation.setPosition(newPos);

    if (isWalking) {
        walkAnimation.update(deltaTime);
    }
    else {
        idleAnimation.update(deltaTime);
    }
}

void Customer::render(sf::RenderWindow& window) {
    if (isWalking) {
        walkAnimation.render(window);
    }
    else {
        idleAnimation.render(window);
    }
}

bool Customer::isActive() {
    return !destroyed;  // Only active if not marked for destruction
}

void Customer::markForDestruction() {
    destroyed = true;  // Mark the customer as destroyed
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

Customer::~Customer() {
    std::cout << name << " is being destroyed." << std::endl;

    // Ensure the animation resources are cleared
    idleAnimation.clear();
    walkAnimation.clear();
}
