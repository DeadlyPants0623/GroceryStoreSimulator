#include "Product.h"
#include <iostream>

Product::Product(std::string name, float price, int quantity)
    : name(name), price(price), quantity(quantity) {}

std::string Product::getName() const { return name; }
float Product::getPrice() const { return price; }
int Product::getQuantity() const { return quantity; }

void Product::addQuantity(int quantityToAdd) {
    quantity += quantityToAdd;
}

bool Product::removeQuantity(int quantityToRemove) {
    if (quantity >= quantityToRemove) {
        quantity -= quantityToRemove;
        return true;
    }
    return false;
}

void Product::display() const {
    std::cout << "Name: " << name << ", Price: " << price
        << ", Quantity: " << quantity << std::endl;
}

float Product::getStorePrice() const
{
	return storePrice;
}

void Product::setPrice(float newPrice)
{
	//std::cout << "Setting price to: " << newPrice << std::endl;
	price = newPrice;
	//std::cout << "Price: " << price << std::endl;
}
