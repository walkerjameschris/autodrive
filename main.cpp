#include <string>
#include <iostream>
#include "src/car.hpp"
#include "src/agent.hpp"
#include "src/simulate.hpp"
#include "src/utilities.hpp"
#include <SFML/Graphics.hpp>

int main() {

    int fps = 60;
    int max_speed = 10;
    int epochs = 250000;
    int sensor_read = 200;
    float sensor_chunk = 25;
    float learn_rate = 0.05;
    float discount_rate = 0.90;

    sf::Texture track_texture;
    sf::Sprite track_sprite;
    sf::Image track_image;

    track_texture.loadFromFile("../img/track.png");
    track_sprite.setTexture(track_texture);
    track_image = track_texture.copyToImage();

    sf::Texture car_texture;
    sf::Sprite car_sprite;

    car_texture.loadFromFile("../img/car.png");
    car_sprite.setTexture(car_texture);

    sf::Vector2u display_dims = track_image.getSize();
    int display_x = display_dims.x;
    int display_y = display_dims.y;

    Car car(display_x, display_y, sensor_read, max_speed, track_image);
    Agent agent(sensor_chunk, learn_rate, discount_rate);
    HUD hud("../font/jetbrains.ttf");

    simulate::train(epochs, car, agent);

    sf::VideoMode video(display_x, display_y);
    sf::RenderWindow window(video, "autodrive");
    window.setFramerateLimit(fps);
    sf::Clock clock;
    sf::Event event;

    Vector state = car.reset();

    while (window.isOpen()) {

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        state = simulate::render(
            hud,
            car,
            agent,
            state,
            clock,
            track_sprite,
            window,
            car_sprite
        );
    }

    return 0;
}
