#include "Inventory.h"
#include <iostream>

void Inventory::addProduct(const Product& product, int quantity) {
	//std::cout << "Adding product: " << product.getName() << std::endl;
	for (auto& p : products) {
		if (p.getName() == product.getName()) {
			p.addQuantity(quantity);
			return;
		}
	}
	products.emplace_back(product.getName(), product.getPrice(), quantity);
}

bool Inventory::hasProduct(const std::string& productName, int quantity) {
	for (auto it = products.begin(); it != products.end(); ++it) {
		if (it->getName() == productName) {
			std::cout << "Product found: " << productName << std::endl;
			return true;
		}
	}
	std::cout << "Product does not exist " << productName << std::endl;
	return false;
}

void Inventory::removeProduct(const std::string& productName, int quantity)
{
	for (auto it = products.begin(); it != products.end(); ++it) {
		if (it->getName() == productName) {
			if (it->removeQuantity(quantity)) {
				//std::cout << "Removed " << quantity << " of " << productName << std::endl;
				return;
			}
			else {
				//std::cout << "Insufficient quantity of " << productName << " in inventory." << std::endl;
				return;
			}
		}
	}
	//std::cout << "Product not found in inventory: " << productName << std::endl;
}

void Inventory::display() const {
	//std::cout << "Products in the inventory:" << std::endl;
	if (products.empty()) {
		//std::cout << "Inventory is empty." << std::endl;
	}
	else {
		for (const auto& product : products) {
			product.display();
		}
	}
}

float Inventory::calculateTotalPrice(const Product& product) const {
	return product.getPrice() * product.getQuantity();
}

float Inventory::getTotalPrice() const {
	float totalPrice = 0.0f;
	for (const auto& product : products) {
		totalPrice += calculateTotalPrice(product);
	}
	return totalPrice;
}


std::vector<Product>& Inventory::getProducts() {
	return products;
}

Product& Inventory::getProduct(const std::string& productName)
{
	for (auto& product : products)
	{
		if (product.getName() == productName)
		{
			return product;
		}
	}
	throw std::runtime_error("Product not found: " + productName);
}

void Inventory::clear()
{
	products.clear();
}
