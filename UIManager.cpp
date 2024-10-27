#include "UIManager.h"
#include <iostream>
#include <iomanip>
#include <sstream>

UIManager::UIManager() {
	text.setCharacterSize(30);
	text.setFillColor(sf::Color::White);
}

void UIManager::loadFont(const std::string& fontPath) {
	if (!font.loadFromFile(fontPath)) {
		// Handle error
	}
	text.setFont(font);
}

void UIManager::updateText(GroceryStore& groceryStore, Inventory inventory,
	bool includeHeader, bool includeFooter, bool includePrice, bool includeButton, bool storePrice, bool paymentButton) {
	text.setString(generateInventoryString(groceryStore, inventory, includeHeader, includeFooter, includePrice, includeButton, storePrice, paymentButton));

	if (includeButton) {
		createProductButtons(groceryStore, inventory);
	}
}

void UIManager::render(sf::RenderWindow& window) {
	text.setPosition(position);
	window.draw(text);

	// Render all buttons
	for (auto& button : productButtons) {
		button.render(window);
	}	
}

void UIManager::setPos(float x, float y) {
	position = sf::Vector2f(x, y);
}

void UIManager::setText(const std::string& text)
{
	this->text.setString(text);
}

void UIManager::handleClick(const sf::Vector2i& mousePos) {
	// Check if any product button was clicked
	for (auto& button : productButtons) {
		if (button.isClicked(mousePos)) {
			std::cout << "Button clicked!\n";
			button.handleClick();  // Trigger button's callback
		}
	}
}

void UIManager::createProductButtons(GroceryStore& groceryStore, Inventory& inventory) {
	productButtons.clear();  // Clear previous buttons

	float buttonY = position.y + 7.5f;  // Initial Y position for buttons

	for (size_t i = 0; i < inventory.getProducts().size(); ++i) {
		std::string productName = inventory.getProducts()[i].getName();  // Capture by value

		Button button(
			"Add Stock",
			sf::Vector2f(position.x + 300, buttonY),
			sf::Vector2f(25, 25),
			[&groceryStore, productName]() {  // Capture by reference
				groceryStore.buyStocks(productName);  // Modify stock
				//std::cout << "Added stock for product: " << productName << std::endl;
			}
		);

		productButtons.push_back(button);
		buttonY += 35;  // Update Y position for the next button
	}
}

void UIManager::createPaymentButton(GroceryStore& groceryStore)
{
	productButtons.clear();  // Clear previous buttons
	Button button(
		"Receive Payment",
		sf::Vector2f(position.x + 300.f, position.y + 300.f),
		sf::Vector2f(25, 25),
		[&groceryStore]() {  // Capture by reference
			groceryStore.receivePayment();  // Modify stock
			//std::cout << "Added stock for product: " << productName << std::endl;
		}
	);
	productButtons.push_back(button);
}


std::string UIManager::generateInventoryString(GroceryStore& groceryStore, const Inventory& inventory, bool includeHeader, bool includeFooter, bool includePrice, bool includeButton, bool storePrice, bool paymentButton) {
	std::ostringstream oss;  // Use string stream to control the formatting

	if (includeHeader) {
		oss << "Products and Quantity and Total Price\n";
	}

	for (const auto& product : inventory.getProducts()) {
		oss << product.getName() << "| |"
			<< product.getQuantity();

		if (includePrice) {
			oss << "| |$" << std::fixed << std::setprecision(2);
			if (storePrice)
			{
				oss << product.getPrice() * 1.1 * product.getQuantity();  // Ensure 2 decimal places
			}
			else
			{
				oss << product.getPrice() * product.getQuantity();  // Ensure 2 decimal places
			}
		}

		oss << "\n";  // Newline after each product entry
	}

	if (includeFooter) {
		oss << "Total: $" << std::fixed << std::setprecision(2)
			<< inventory.getTotalPrice(true) << "\n";  // Ensure 2 decimal places
		if (paymentButton) {
			createPaymentButton(groceryStore);
		}
	}

	std::string inventoryText = oss.str();
	//std::cout << inventoryText;  // Output the inventory text
	return inventoryText;
}
