#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

class Product {
public:
    Product(std::string name = "", double price = 0.0, int quantity = 0);
    std::string getName() const;
    float getPrice() const;
    int getQuantity() const;

    void addQuantity(int quantityToAdd);
    bool removeQuantity(int quantityToRemove);
    void display() const;
    float getStorePrice() const;

    float storePrice;

protected:
    std::string name;
    float price;
    int quantity;

};

#endif
