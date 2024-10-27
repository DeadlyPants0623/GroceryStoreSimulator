#ifndef STOCKS_H
#define STOCKS_H

#include "Inventory.h"
#include <string>

class Stocks {
public:
    void loadConfig(const std::string& filename);
    void displayInventory() const;

    const Inventory& getInventory() const;

private:
    Inventory inventory;
};

#endif
