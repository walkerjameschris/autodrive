#include <SFML/Graphics.hpp>
#include "src/simulate.hpp"
#include "src/brain.hpp"
#include "src/car.hpp"
#include <iostream>
#include <string>

int main() {

    const int frame_rate = 60;
    const int display_x = 1280;
    const int display_y = 720;
    const int train_cycles = 10000;
    
    sf::VideoMode video(display_x, display_y);
    sf::RenderWindow window(video, "");

    sf::Texture texture;
    sf::Sprite sprite;
    sf::Image image;
    sf::Clock clock;
    sf::Event event;

    texture.loadFromFile("../img/track.png");
    window.setFramerateLimit(frame_rate);
    sprite.setTexture(texture);
    
    image = texture.copyToImage();

    Car car;
    Brain brain;
    
    while (window.isOpen()) {

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        Simulate::render(car, brain, image, sprite, window);
    }

    return 0;
}
