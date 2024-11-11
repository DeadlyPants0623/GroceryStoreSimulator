#pragma once
#include "Product.h"
#include <vector>

class Inventory {
public:
    void addProduct(const Product& product, int quantity);
    bool hasProduct(const std::string& productName, int quantity);
	void removeProduct(const std::string& productName, int quantity);
    void display() const;
    float calculateTotalPrice(const Product& product) const;
	float getTotalPrice() const;

    std::vector<Product>& getProducts();
	Product& getProduct(const std::string& productName);
	void clear();

private:
    std::vector<Product> products;
};
