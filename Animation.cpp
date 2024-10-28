#include "Animation.h"
#include <stdexcept>
#include <iostream>

Animation::Animation(float frameDuration) : frameDuration(frameDuration) {}

void Animation::addFrame(const std::string& filepath) {
    sf::Texture texture;
    if (!texture.loadFromFile(filepath)) {
        throw std::runtime_error("Failed to load texture: " + filepath);
    }
    frames.push_back(texture);  // Store the loaded texture
}

void Animation::update(float deltaTime) {
    if (!isRunning || frames.empty()) return;  // Don't update if stopped

    elapsedTime += deltaTime;

    if (elapsedTime >= frameDuration) {
        currentFrame = (currentFrame + 1) % frames.size();  // Loop through frames
        sprite.setTexture(frames[currentFrame]);  // Update the texture
        elapsedTime = 0.0f;
    }
}

void Animation::setPosition(const sf::Vector2f& pos) {
    sprite.setPosition(pos);
}

sf::Vector2f Animation::getPosition() const {
    return sprite.getPosition();  // **Retrieve the sprite's current position**
}

void Animation::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Animation::reset() {
    currentFrame = 0;
    elapsedTime = 0.0f;
    sprite.setTexture(frames[0]);  // Reset to the first frame
}

void Animation::clear() {
    sprite = sf::Sprite();  // Reset the sprite
    frames.clear();          // Clear the frames
    std::cout << "Animation resources cleared." << std::endl;
}

void Animation::stop() {
    isRunning = false;
    std::cout << "Animation stopped." << std::endl;
}
