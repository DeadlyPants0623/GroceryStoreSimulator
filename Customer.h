#include "Inventory.h"
#include "GroceryStore.h"
#include <string>
#include "Animation.h"

enum class CustomerState {
    Idle,
    WalkingToQueue,
	IdleInQueue,
	WalkingToCashier,
	SendToCart,
	Leaving
};

class Customer {
public:
    Customer(const std::string& name);
	~Customer();

    std::string getName() const;

    void update(float deltaTime);  // Update the active animation
    void render(sf::RenderWindow& window);  // Render the active animation

    void buyProduct(const std::string& productName, float productPrice, int quantity, Inventory& storeInventory);
    void displayInventory() const;
    void sendToCart(GroceryStore& groceryStore) const;

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
    bool markedForRemoval = false;

	void loadRandomAnimations();
};