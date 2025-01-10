#pragma once

#include <string>
#include "agent.hpp"
#include <SFML/Graphics.hpp>

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

    static std::string remap_action(int action) {
        if (action == 0) {
            return "Left";
        } else if (action == 1) {
            return "Right";
        } else if (action == 2) {
            return "Faster";
        } else if (action == 3) {
            return "Slower";
        }

        return "";
    }

    float render(
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

        message += "Action: " + remap_action(action) + "\n";
        message += "FPS: " + std::to_string(fps);

        text.setString(message);
        window.draw(text);
        return fps;
    }
};