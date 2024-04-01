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

sf::Vector2f SpriteManager::getPosition() {
    return sprite.getPosition(); 
}

sf::FloatRect SpriteManager::getViewBounds() {
    // setting periphery 
    const float peripheryWidth = 33 * 10; // 33 columns, each 10 pixels wide
    const float peripheryHeight = 19 * 10; // 19 rows, each 10 pixels tall

    // Center the view on the sprite's current position
    sf::Vector2f center(sprite.getPosition().x + sprite.getGlobalBounds().width / 2, 
        sprite.getPosition().y + sprite.getGlobalBounds().height / 2);
    sf::Vector2f peripherySize(peripheryWidth, peripheryHeight); 

    return sf::FloatRect(center - peripherySize / 2.f, peripherySize);
}

sf::FloatRect SpriteManager::getGlobalBounds() {
    return sprite.getGlobalBounds(); 
}