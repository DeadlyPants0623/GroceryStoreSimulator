#include "Button.h"

Button::Button(const std::string& inLabel, const sf::Vector2f& position, const sf::Vector2f& size, std::function<void()> callback)
    : label(inLabel), callback(callback) {  // Assign inLabel to label first
    shape.setSize(size);
    shape.setPosition(position);
    shape.setFillColor(sf::Color(100, 100, 255));

    text.setString(label);  // Now this will have the correct label value
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(20);  // Default size; can be adjusted later
    text.setPosition(position.x + 10, position.y + 5);  // Center the text slightly
}

void Button::setFont(const sf::Font& font) {
    text.setFont(font);
}

void Button::render(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

bool Button::isClicked(const sf::Vector2i& mousePos) const {
    return shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

void Button::handleClick() {
    callback();  // Execute the button's action
}

std::string Button::getLabel() const {
    return label;
}

void Button::setLabel(const std::string& inLabel)
{
	label = inLabel;
	text.setString(label);
}
