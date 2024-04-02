#pragma once

#include <iostream>
#include <vector>
#include <SFML/Network.hpp>
#include <thread>
#include <mutex>
#include "SpriteManager.h"

#define SPRITE 0
#define PARTICLE 1

sf::TcpListener listener; 
sf::SocketSelector selector; 

std::vector<sf::TcpSocket*> clients;
extern std::vector<SpriteManager*> sprites; 
extern std::vector<Particle> particles; 

extern std::mutex clientsMutex; 

int connectionCounter = 0; 

std::string imgPaths[2] = { "include/pikachu.png", "include/snorlax.png" };

unsigned short port = 6250; 

bool isWithinPeriphery(sf::FloatRect objectBounds, sf::FloatRect periphery) {
    if (periphery.intersects(objectBounds)) {
        return true; 
    }

    return false; 
}

void sendSpriteAndParticlePositions() {
    for (int i = 0; i < sprites.size(); i++) { // might need to lock because access sprites.size()

        sf::Packet packet;

        for (int j = 0; j < sprites.size(); j++) {

            if (i != j) { // to avoid sending the sprite its own 
                sf::Uint8 messageType = SPRITE;
                packet << messageType << sprites[j]->getPosition().x << sprites[j]->getPosition().y;
            }
        }

        for (auto& particle : particles) {
            sf::Vector2f position = particle.shape.getPosition();
            sf::Uint8 messageType = PARTICLE;
            packet << messageType << position.x << position.y;
        }

        if (clients[i]->send(packet) != sf::Socket::Done) {
            std::cout << "Error sending packet to client " << i << std::endl;
        }
    }
}

void acceptClients() {
    if (listener.listen(port) != sf::Socket::Done) {
        std::cerr << "Failed to bind to port " << port << std::endl;
    }
    std::cout << "Server is listening on port " << port << std::endl;

    selector.add(listener);  

	while (true) {

        // selector waits for data on any socket
        if (selector.wait()) {
            if (selector.isReady(listener)) {

                sf::TcpSocket* client = new sf::TcpSocket; 
                client->setBlocking(true);
                if (listener.accept(*client) == sf::Socket::Done) {
                    clients.push_back(client); 
                    selector.add(*client); 

                    SpriteManager* spriteManager = new SpriteManager("include/pikachu.png", sf::Vector2f(0.5f, 0.5f), sf::Vector2f(0, 0));
                    sprites.push_back(spriteManager);
                }
                else {
                    delete client; 
                }
            }
            else {
                // The listener socket is not ready, test all other sockets (the clients)
                for (std::vector<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it) {
                    sf::TcpSocket& client = **it;

                    if (selector.isReady(client)) {
                        // The client has sent some data, we can receive it

                        sf::Packet packet;
                        if (client.receive(packet) == sf::Socket::Done) {
                            int index = std::distance(clients.begin(), it);
                            float x, y;
                            packet >> x >> y;
                            sprites[index]->update(sf::Vector2f(x, y));
                        }

                    }
                }
            }
        }
	}
}