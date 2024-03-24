#pragma once
#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H

#include <SFML/Graphics.hpp>
#include <future>

extern const int HEIGHT;
extern const int WIDTH;

class SpriteManager {
public:
    SpriteManager(const std::string& texturePath, const sf::Vector2f& scale, const sf::Vector2f& initialPosition);
    void update(sf::RenderWindow& window, bool isExplorerMode);
    void draw(sf::RenderWindow& window);
    std::future<void> updateAsync(sf::RenderWindow& window, bool isExplorerMode);

private:
    sf::Texture texture;
    sf::Sprite sprite;
};

#endif // SPRITEMANAGER_H
