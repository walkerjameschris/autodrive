#include <SFML/Graphics.hpp>
#include "src/simulate.hpp"
#include "src/agent.hpp"
#include "src/car.hpp"
#include <iostream>
#include <string>

int main() {

    const int frame_rate = 60;
    const int display_x = 1280;
    const int display_y = 720;
    const int train_cycles = 500000;

    sf::Texture texture;
    sf::Sprite sprite;
    sf::Image image;

    texture.loadFromFile("../img/track.png");
    sprite.setTexture(texture);
    image = texture.copyToImage();

    Car car(display_x, display_y, 200);
    Agent agent;

    Simulate::train(train_cycles, car, agent, image);

    sf::VideoMode video(display_x, display_y);
    sf::RenderWindow window(video, "autodrive");
    sf::Clock clock;
    sf::Event event;

    window.setFramerateLimit(frame_rate);
    
    while (window.isOpen()) {

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        Simulate::render(car, agent, image, sprite, window);
    }

    return 0;
}
