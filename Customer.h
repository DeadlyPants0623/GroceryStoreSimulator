#include "Inventory.h"
#include "GroceryStore.h"
#include <string>
#include "Animation.h"
#include <functional>

class Customer : public std::enable_shared_from_this<Customer> {
public:
    Customer(const std::string& name);
	~Customer();

    std::string getName() const;
    void buyProduct(const std::string& productName, float productPrice, int quantity, Inventory& storeInventory);
    void displayInventory() const;
    void sendToCart(GroceryStore& groceryStore) const;

    void setPosition(const sf::Vector2f& pos);
    sf::Vector2f getPosition() const;
    void moveTo(const sf::Vector2f& target, float duration, std::function<void()> onArrival);
    void exitAndDestroy(std::function<void(std::shared_ptr<Customer>)> onDestroyCallback);
    void setWalking(bool walking);  // Switch between idle and walk animations
    void update(float deltaTime);  // Update the active animation
    void render(sf::RenderWindow& window);  // Render the active animation

    bool isActive();
    void markForDestruction();  // New method to mark the customer for removal

private:
    std::string name;
    Inventory inventory;
    Animation idleAnimation;
    Animation walkAnimation;
    bool isWalking = false;  // Current state of the customer
	bool destroyed = false;  // Flag to mark the customer for removal

    // Movement variables
    sf::Vector2f startPos;
    sf::Vector2f targetPos;
    float totalDuration = 0.0f;  // Total time for the movement
    float elapsedTime = 0.0f;    // Elapsed time since movement started
    bool moving = false;         // Whether the customer is currently moving

    std::function<void()> onArrivalCallback;  // Callback for arrival event

	void loadRandomAnimations();
};