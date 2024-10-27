#ifndef NAVBAR_H
#define NAVBAR_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <string>

class NavBar {
public:
    NavBar();
    void addButton(const std::string& label, std::function<void()> callback);
    void render(sf::RenderWindow& window);
    void handleClick(const sf::Vector2i& mousePos);

private:
    struct Button {
        sf::RectangleShape shape;
        sf::Text label;
        std::function<void()> callback;
    };

    sf::Font font;
    std::vector<Button> buttons;
    float buttonWidth;
    float buttonHeight;
    float spacing;
};

#endif
