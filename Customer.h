#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "Inventory.h"
#include "GroceryStore.h"
#include <string>

class Customer {
public:
    Customer(std::string name);

    std::string getName() const;
    void buyProduct(const std::string& productName, float productPrice, int quantity, Inventory& storeInventory);
    void displayInventory() const;
    void sendToCart(GroceryStore& groceryStore) const;

private:
    std::string name;
    Inventory inventory;
};

#endif
