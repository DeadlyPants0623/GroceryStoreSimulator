#pragma once
#include <string>

class Product {
public:
    Product(std::string name = "", float price = 0.0, int quantity = 0);
    std::string getName() const;
    float getPrice() const;
    int getQuantity() const;

    void addQuantity(int quantityToAdd);
    bool removeQuantity(int quantityToRemove);
    void display() const;
    float getStorePrice() const;

    float storePrice;

    void setPrice(float newPrice);

private:
    std::string name;
    float price;
    int quantity;

};
