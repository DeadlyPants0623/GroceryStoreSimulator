#include "GroceryStore.h"
#include <iostream>

GroceryStore::GroceryStore(const Stocks& stocks) : stockManager(stocks) {}

void GroceryStore::buyStocks(const std::string& productName) {
	float localPrice = 0.0f;
    const auto& products = stockManager.getInventory().getProducts();
    for (const auto& product : products) {
        if (product.getName() == productName) {
            //std::cout << "Buying stock: " << productName << std::endl;
			localPrice = product.getPrice() * product.getQuantity();
			if (storeCredit >= localPrice) {
				storeCredit -= localPrice;
				inventory.addProduct(product, product.getQuantity());
				return;
			}
			else {
				//std::cout << "Insufficient store credit to buy stock: " << productName << std::endl;
				return;
			}
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

void GroceryStore::receivePayment()
{
	for (const auto& product : checkOut.getProducts())
	{
		//std::cout << "Receiving payment for: " << product.getName() << std::endl;
		storeCredit += product.getPrice() * 1.1f * product.getQuantity();
		checkOut.clear();
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
