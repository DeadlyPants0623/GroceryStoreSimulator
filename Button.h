#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

class Button {
public:
    Button(const std::string& label, const sf::Vector2f& position, const sf::Vector2f& size, std::function<void()> callback);

    void setFont(const sf::Font& font);
    void render(sf::RenderWindow& window);
    bool isClicked(const sf::Vector2i& mousePos) const;

private:
    sf::RectangleShape shape;
    sf::Text text;
    std::function<void()> callback;

public:
    void handleClick();  // Exposed as public so UIManager can trigger it
};

#endif
