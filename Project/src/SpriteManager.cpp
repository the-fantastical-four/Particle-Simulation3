#include "SpriteManager.h"
#include <iostream>

SpriteManager::SpriteManager(const std::string& texturePath, const sf::Vector2f& scale, const sf::Vector2f& initialPosition) {
    if (!texture.loadFromFile(texturePath)) {
        std::cout << "Load failed" << std::endl;
        system("pause");
    }

    sprite.setTexture(texture);
    sprite.setScale(scale);
    sprite.setPosition(initialPosition);
}

void SpriteManager::update(sf::RenderWindow& window, bool isExplorerMode) {
    if (isExplorerMode) {
        const float moveSpeed = 5.0f;
        sf::Vector2f movement(0.f, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= moveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += moveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= moveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += moveSpeed;

        sf::Vector2f newPosition = sprite.getPosition() + movement;
        sf::FloatRect spriteBounds = sprite.getGlobalBounds();
        // changed to width and height to avoid particles and sprite going out of bounds in explorer mode
        // when particles are spawned
        float minX = 0, maxX = WIDTH - spriteBounds.width; 
        float minY = 0, maxY = HEIGHT - spriteBounds.height;

        newPosition.x = std::min(std::max(newPosition.x, minX), maxX);
        newPosition.y = std::min(std::max(newPosition.y, minY), maxY);

        sprite.setPosition(newPosition);
    }
}

std::future<void> SpriteManager::updateAsync(sf::RenderWindow& window, bool isExplorerMode) {
    return std::async(std::launch::async, [this, &window, isExplorerMode]() {
        update(window, isExplorerMode);
        });
}

void SpriteManager::draw(sf::RenderWindow& window) {
    // If not in explorer mode, reset to the default view
    window.setView(window.getDefaultView());
    window.draw(sprite);
    
}
