#include <iostream>
#include <SFML/Graphics.hpp>

constexpr auto WIDTH = 1280;
constexpr auto HEIGHT = 720;
constexpr auto TITLE = "SFML VS Edition";
constexpr auto FPS = 100;

class Ball;

int main(int argc, char* argv[]) {
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), TITLE, sf::Style::Default);
	window.setFramerateLimit(FPS);
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
	return 0;
}

class Ball {
	
};