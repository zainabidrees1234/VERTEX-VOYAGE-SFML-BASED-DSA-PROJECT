#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <SFML/Graphics.hpp>
#include <string>

class InputBox {
private:
    sf::RectangleShape box;
    sf::Text text;
    bool isActive;
    std::string input;

public:
    InputBox(float x, float y, float width, float height, sf::Font& font);

    void draw(sf::RenderWindow& window);
    void handleEvent(sf::Event event, const sf::RenderWindow& window);
    std::string getText() const;
    void setText(const std::string& str);
    void clear();
};

#endif // INPUTBOX_H