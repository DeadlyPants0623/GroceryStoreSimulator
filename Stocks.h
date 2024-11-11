#pragma once
#include "Inventory.h"
#include <string>

class Stocks {
public:
    void loadConfig(const std::string& filename);
    void displayInventory() const;

    Inventory& getInventory();

private:
    Inventory inventory;
};
