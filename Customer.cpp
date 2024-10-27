#include "Customer.h"
#include <iostream>

Customer::Customer(std::string name) : name(name) {}

std::string Customer::getName() const { return name; }

void Customer::buyProduct(const std::string& productName, float productPrice, int quantity, Inventory& storeInventory) {
    if (storeInventory.removeProduct(productName, quantity)) {
        //std::cout << name << " bought " << quantity << " of " << productName << std::endl;
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
