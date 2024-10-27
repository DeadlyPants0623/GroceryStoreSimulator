#include "Inventory.h"
#include <iostream>

void Inventory::addProduct(const Product& product, int quantity) {
	std::cout << "Adding product: " << product.getName() << std::endl;
	for (auto& p : products) {
		if (p.getName() == product.getName()) {
			p.addQuantity(quantity);
			return;
		}
	}
	products.emplace_back(product.getName(), product.getPrice(), quantity);
}

bool Inventory::removeProduct(const std::string& productName, int quantity) {
	for (auto it = products.begin(); it != products.end(); ++it) {
		if (it->getName() == productName) {
			if (it->removeQuantity(quantity)) {
				if (it->getQuantity() == 0) {
					products.erase(it);
				}
				return true;
			}
			else {
				std::cout << "Not enough quantity of " << productName << " in stock." << std::endl;
				return false;
			}
		}
	}
	//std::cout << "Product not found: " << productName << std::endl;
	return false;
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

float Inventory::getTotalPrice(bool storePrice) const
{
	float localTotalPrice = 0;
	if (storePrice)
	{
		for (const auto& product : products)
		{
			localTotalPrice += product.getPrice() * 1.1 * product.getQuantity();
		}
		return localTotalPrice;
	}
	else
	{
		for (const auto& product : products)
		{
			localTotalPrice += product.getPrice() * product.getQuantity();
		}
		return localTotalPrice;
	}
}

const std::vector<Product>& Inventory::getProducts() const {
	return products;
}

void Inventory::clear()
{
	products.clear();
}
