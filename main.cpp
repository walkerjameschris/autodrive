#include <SFML/Graphics.hpp>
#include "src/simulation.hpp"
#include <iostream>
#include <string>

int main() {

    const int frame_rate = 60;
    const int display_x = 1280;
    const int display_y = 720;
    
    Simulation simulation;
    
    sf::VideoMode window_scale(display_x, display_y);
    sf::RenderWindow window(window_scale, "autodrive");
    sf::Texture texture;
    sf::Clock clock;
    sf::Event event;

    window.setFramerateLimit(frame_rate);
    texture.loadFromFile("../img/track.png");
    sf::Sprite sprite(texture);
    sf::Image image = texture.copyToImage();

    while (window.isOpen()) {

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        simulation.render(
            window,
            sprite,
            image,
            sf::Keyboard::isKeyPressed(sf::Keyboard::A),
            sf::Keyboard::isKeyPressed(sf::Keyboard::D),
            sf::Keyboard::isKeyPressed(sf::Keyboard::W),
            sf::Keyboard::isKeyPressed(sf::Keyboard::S)
        );
    }

    return 0;
}
