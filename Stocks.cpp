#include "Stocks.h"
#include <fstream>
#include <sstream>
#include <iostream>

void Stocks::loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file " << filename << std::endl;
        return;
    }

    std::string line;
    while (getline(file, line)) {
        std::istringstream iss(line);
        std::string name;
        float price;
        int quantity;

        if (iss >> name >> price >> quantity) {
            Product product(name, price);
            inventory.addProduct(product, quantity);
        }
    }
    file.close();
}

void Stocks::displayInventory() const {
    inventory.display();
}

Inventory& Stocks::getInventory() {
    return inventory;
}
