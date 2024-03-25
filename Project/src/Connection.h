#pragma once

#include <iostream>
#include <vector>
#include <SFML/Network.hpp>
#include <thread>
#include <mutex>
#include "SpriteManager.h"

sf::TcpListener listener; 
std::vector<std::unique_ptr<sf::TcpSocket>> clients;
extern std::vector<SpriteManager> sprites; 

std::mutex clientsMutex; 

int connectionCounter = 0; 

std::string imgPaths[2] = { "include/pikachu.png", "include/snorlax.png" };

unsigned short port = 6250; 

void receiveFromClient(sf::TcpSocket* clientSocket, SpriteManager* sprite) {
    while (true) {
        std::lock_guard<std::mutex> lock(clientsMutex); 
        sf::Packet packet; 
        if (clientSocket->receive(packet) == sf::Socket::Done) {
            float x, y; 
            packet >> x >> y; 
            sprite->update(sf::Vector2f(x, y)); 
        }
    }
}

void acceptClients() {
    if (listener.listen(port) != sf::Socket::Done) {
        std::cerr << "Failed to bind to port " << port << std::endl;
    }

    std::cout << "Server is listening on port " << port << std::endl;

	while (true) {
        auto client = std::make_unique<sf::TcpSocket>(); // Use std::make_unique to create a new sf::TcpSocket
        client->setBlocking(false);

;       if (listener.accept(*client) == sf::Socket::Done) {
            std::lock_guard<std::mutex> lock(clientsMutex); // Lock clients vector for thread safety

            sf::TcpSocket* clientPtr = client.get();
            clients.emplace_back(std::move(client));

            sprites.emplace_back(imgPaths[connectionCounter], sf::Vector2f(0.5f, 0.5f), sf::Vector2f(0, 0));
            SpriteManager* spritePtr = &sprites.back(); 

            std::thread clientThread(receiveFromClient, clientPtr, spritePtr); 
            clientThread.detach(); 
            connectionCounter++; 
        }
	}
}