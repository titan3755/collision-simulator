#include <memory>
#include <SFML/Graphics.hpp>

#define WIDTH 1280
#define HEIGHT 720
#define BALL_RADIUS 50

std::unique_ptr<sf::RenderWindow> setup(std::string title, int width, int height, int frl);
std::unique_ptr<sf::CircleShape> createBall(int radius, sf::Color color, sf::Vector2f position);

int main()
{
    auto window = setup("SFML works!", WIDTH, HEIGHT, 60);
    auto shape = createBall(BALL_RADIUS, sf::Color::Red, sf::Vector2f(WIDTH / 2, HEIGHT / 2));
    sf::Vector2f shapeVelocity = sf::Vector2f(20, 20);
    while (window->isOpen())
    {
        // HANDLE EVENTS
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
        }

        // UPDATE FRAME
        // COLLISION DETECTION WITH BOUNDARY
        if (shape->getPosition().x + BALL_RADIUS >= WIDTH) {
            shapeVelocity.x = -shapeVelocity.x;
            shape->setPosition(WIDTH - BALL_RADIUS, shape->getPosition().y);
        }
        if (shape->getPosition().x - BALL_RADIUS <= 0) {
            shapeVelocity.x = -shapeVelocity.x;
            shape->setPosition(BALL_RADIUS, shape->getPosition().y);
        }
        if (shape->getPosition().y + BALL_RADIUS >= HEIGHT) {
            shapeVelocity.y = -shapeVelocity.y;
            shape->setPosition(shape->getPosition().x, HEIGHT - BALL_RADIUS);
        }
        if (shape->getPosition().y - BALL_RADIUS <= 0) {
            shapeVelocity.y = -shapeVelocity.y;
            shape->setPosition(shape->getPosition().x, BALL_RADIUS);
        }
        // MOVE BALL
        shape->move(shapeVelocity);
        // RENDER FRAME
        window->clear();
        window->draw(*shape);
        window->display();
    }
    window->close();
    return 0;
}

std::unique_ptr<sf::RenderWindow> setup(std::string title, int width, int height, int frl) {
    auto window = std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), title, sf::Style::Default, sf::ContextSettings(0, 0, 8));
    window->setFramerateLimit(frl);
    return window;
}

std::unique_ptr<sf::CircleShape> createBall(int radius, sf::Color color, sf::Vector2f position) {
    auto ball = std::make_unique<sf::CircleShape>(radius);
    ball->setFillColor(color);
    ball->setPosition(position);
    ball->setOrigin(radius / 2, radius / 2);
    return ball;
}