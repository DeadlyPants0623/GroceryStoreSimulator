#ifndef GROCERYSTORE_H
#define GROCERYSTORE_H

#include "Product.h"
#include "Stocks.h"

class GroceryStore {
public:
    GroceryStore(const Stocks& stocks);

    void buyStocks(const std::string& productName);
    void displayInventory() const;
    void addToCart(const std::vector<Product>& products);
	void receivePayment();

    Inventory& getInventory();
    const Inventory& getCheckOutInventory() const;
	float getStoreCredit() const;

private:
    float storeCredit = 1000000;
    Inventory checkOut;
    Inventory inventory;
    const Stocks& stockManager;
};

#endif
