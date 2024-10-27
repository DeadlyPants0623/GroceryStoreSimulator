#ifndef INVENTORY_H
#define INVENTORY_H

#include "Product.h"
#include <vector>

class Inventory {
public:
    void addProduct(const Product& product, int quantity);
    bool removeProduct(const std::string& productName, int quantity);
    void display() const;
	float getTotalPrice() const;

    const std::vector<Product>& getProducts() const;

private:
    std::vector<Product> products;
};

#endif