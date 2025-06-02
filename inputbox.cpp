#include "inputbox.h"

InputBox::InputBox(float x, float y, float width, float height, sf::Font& font) {
    box.setPosition(x, y);
    box.setSize({ width, height });
    box.setFillColor(sf::Color(240, 240, 240, 200));
    box.setOutlineColor(sf::Color::Black);
    box.setOutlineThickness(2);
    isActive = false;

    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Black);
    text.setPosition(x + 5, y + 5);
}


void InputBox::draw(sf::RenderWindow& window) {
    window.draw(box);
    window.draw(text);
}

void InputBox::handleEvent(sf::Event event, const sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mouse(sf::Mouse::getPosition(window));
        isActive = box.getGlobalBounds().contains(mouse);
    }

    if (isActive && event.type == sf::Event::TextEntered) {
        if (event.text.unicode == '\b' && !input.empty()) {
            input.pop_back();
        }
        else if (event.text.unicode < 128 && event.text.unicode != '\r') {
            input += static_cast<char>(event.text.unicode);
        }
        text.setString(input);
    }
}

std::string InputBox::getText() const {
    return input;
}

void InputBox::setText(const std::string& str) {
    input = str;
    text.setString(str);
}

void InputBox::clear() {
    input.clear();
    text.setString("");
}