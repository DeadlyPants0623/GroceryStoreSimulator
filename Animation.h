#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Animation {
private:
    std::vector<sf::Texture> frames;  // Textures for each frame
    sf::Sprite sprite;                // Sprite to display the current frame
    float frameDuration;              // Duration per frame (in seconds)
    int currentFrame = 0;             // Current frame index
    float elapsedTime = 0.0f;         // Time since last frame update
    bool isRunning = true;            // Track if the animation is running

public:
    Animation(float frameDuration = 0.1f);  // Default frame duration

    void addFrame(const std::string& filepath);  // Load a frame from a PNG
    void update(float deltaTime);  // Update the animation
    void setPosition(const sf::Vector2f& pos);  // Set sprite position
    sf::Vector2f getPosition() const;  // Get the current sprite position
    void render(sf::RenderWindow& window);  // Render the sprite
    void reset();  // Reset to the first frame
	void clear();  // Clear all frames (for reuse
	void stop();  // Stop the animation
};
