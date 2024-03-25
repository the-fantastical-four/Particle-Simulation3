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

void SpriteManager::update(sf::Vector2f newPosition) {
    sprite.setPosition(newPosition);
}

std::future<void> SpriteManager::updateAsync(sf::Vector2f newPosition) {
    return std::async(std::launch::async, [this, newPosition]() {
        update(newPosition);
        });
}

void SpriteManager::draw(sf::RenderWindow& window) {
    // If not in explorer mode, reset to the default view
    window.setView(window.getDefaultView());
    window.draw(sprite);
    
}
