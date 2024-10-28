#include "UIManager.h"
#include <iostream>
#include <iomanip>
#include <sstream>

UIManager::UIManager() {
	text.setCharacterSize(35);
	text.setFillColor(sf::Color::White);
}

void UIManager::loadFont(const std::string& fontPath) {
	if (!font.loadFromFile(fontPath)) {
		// Handle error
	}
	text.setFont(font);
}

void UIManager::updateText(GroceryStore& groceryStore, const Inventory& inventory, UpdateOptions options) {
	// Generate the inventory string based on the options provided
	text.setString(generateInventoryString(
		groceryStore, inventory,
		hasFlag(options, UpdateOptions::IncludeHeader),
		hasFlag(options, UpdateOptions::IncludeFooter),
		hasFlag(options, UpdateOptions::IncludePrice),
		hasFlag(options, UpdateOptions::IncludeButton),
		hasFlag(options, UpdateOptions::StorePrice),
		hasFlag(options, UpdateOptions::PaymentButton)
	));

	// Create product buttons if the option is enabled
	if (hasFlag(options, UpdateOptions::IncludeButton)) {
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

void UIManager::createProductButtons(GroceryStore& groceryStore, const Inventory& inventory) {
	productButtons.clear();  // Clear previous buttons

	float buttonY = position.y + 7.5f;  // Initial Y position for buttons

	for (size_t i = 0; i < inventory.getProducts().size(); ++i) {
		std::string productName = inventory.getProducts()[i].getName();  // Capture by value

		Button button(
			"Add " + productName,
			sf::Vector2f(position.x + 350, buttonY),
			sf::Vector2f(100, 30),
			[&groceryStore, productName]() {  // Capture by reference
				groceryStore.buyStocks(productName);  // Modify stock
				//std::cout << "Added stock for product: " << productName << std::endl;
			}
		);
		button.setFont(font);
		productButtons.push_back(button);
		buttonY += 40;  // Update Y position for the next button
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

bool UIManager::isPayButtonClicked(const sf::Vector2i& mousePos) const {
	for (const auto& button : productButtons) {
		if (button.isClicked(mousePos) && button.getLabel() == "Receive Payment") {
			return true;  // Pay button was clicked
		}
	}
	return false;
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
