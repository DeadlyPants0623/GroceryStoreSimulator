#include "NavBar.h"
#include <stdexcept>

NavBar::NavBar() : buttonWidth(100), buttonHeight(40), spacing(10) {
    if (!font.loadFromFile("pixel.ttf")) {
        throw std::runtime_error("Failed to load font.");
    }
}

void NavBar::addButton(const std::string& label, std::function<void()> callback) {
    Button button;
    button.shape.setSize({ buttonWidth, buttonHeight });
    button.shape.setFillColor(sf::Color(100, 100, 255));
    button.shape.setPosition(buttons.size() * (buttonWidth + spacing), 0);

    button.label.setFont(font);
    button.label.setString(label);
    button.label.setCharacterSize(24);
    button.label.setFillColor(sf::Color::White);
    button.label.setPosition(button.shape.getPosition().x + 10, button.shape.getPosition().y + 5);

    button.callback = callback;
    buttons.push_back(button);
}

void NavBar::render(sf::RenderWindow& window) {
    for (const auto& button : buttons) {
        window.draw(button.shape);
        window.draw(button.label);
    }
}

void NavBar::handleClick(const sf::Vector2i& mousePos) {
    for (const auto& button : buttons) {
        if (button.shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            button.callback();  // Call the button's associated function
        }
    }
}
