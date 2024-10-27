#include "GroceryStore.h"
#include <iostream>

GroceryStore::GroceryStore(const Stocks& stocks) : stockManager(stocks) {}

void GroceryStore::buyStocks(const std::string& productName) {
    const auto& products = stockManager.getInventory().getProducts();
    for (const auto& product : products) {
        if (product.getName() == productName) {
            std::cout << "Buying stock: " << productName << std::endl;
            inventory.addProduct(product, product.getQuantity());
            return;
        }
    }
    //std::cout << "Product not found in stock: " << productName << std::endl;
}

void GroceryStore::displayInventory() const {
    inventory.display();
}

void GroceryStore::addToCart(const std::vector<Product>& products)
{
    for (const auto& product : products)
    {
        checkOut.addProduct(product, product.getQuantity());
    }
}

Inventory& GroceryStore::getInventory() {
    return inventory;
}

const Inventory& GroceryStore::getCheckOutInventory() const
{
    return checkOut;
}

float GroceryStore::getStoreCredit() const
{
	return storeCredit;
}
