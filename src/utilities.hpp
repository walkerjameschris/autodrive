#pragma once

#include <cmath>
#include <string>
#include "agent.hpp"
#include <unordered_map>
#include <SFML/Graphics.hpp>

using Key = std::vector<int>;
using Vector = std::vector<float>;
using Points = std::map<int, Vector>;
using TrackMap = std::unordered_map<int, bool>;

unsigned int seed = 123456;

namespace numerics {

    float random() {
        unsigned int multiply = 16807;
        unsigned int divisor = 2147483647;
        seed = (multiply * seed) % divisor;
        return std::abs(float(seed) / float(divisor));
    }

    int randint(int low, int high) {
        return std::floor(low + random() * ((high - low) + 1));
    }
};

struct Element {

    sf::Image image;
    sf::Sprite sprite;
    sf::Texture texture;

    Element(std::string path) {
        texture.loadFromFile(path);
        image = texture.copyToImage();
        sprite.setTexture(texture);
    }
};
    
struct HUD {
    
    sf::Font font;
    sf::Text text;

    HUD(std::string font_path) {
        font.loadFromFile(font_path);
        text.setFont(font);
        text.setCharacterSize(16);
        text.setPosition(10, 10);
        text.setFillColor(sf::Color::White);
    }

    void render(
        int action,
        Vector state,
        sf::Clock& clock,
        sf::RenderWindow& window
    ) {

        int fps = 1 / clock.getElapsedTime().asSeconds();
        clock.restart();

        std::string message;

        for (int i = 0; i < state.size(); i++) {
            int distance = state[i];
            message += "Sensor " + std::to_string(i);
            message += ": " + std::to_string(distance);
            message += "\n";
        }

        message += "Action: " + std::to_string(action) + "\n";
        message += "FPS: " + std::to_string(fps);

        text.setString(message);
        window.draw(text);
    }
};
