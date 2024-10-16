#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

#define FRAMERATE_LIMIT 75
#define WIDTH 1280
#define HEIGHT 720
#define BALL_RADIUS 50

std::unique_ptr<sf::RenderWindow> setup(std::string title, int width, int height, int frl);
std::unique_ptr<sf::CircleShape> createBall(int radius, sf::Color color, sf::Vector2f position);
std::vector<std::unique_ptr<sf::RectangleShape>> createBoundaryRectangles(int width, int height);

int main()
{
    sf::Clock clock;
    float dt;
    float mp = 5.f;
    // positions
    sf::Vector2f p1 = sf::Vector2f(100, 100);
    auto window = setup("Collision Simulator", WIDTH, HEIGHT, FRAMERATE_LIMIT);
    auto shape = createBall(BALL_RADIUS, sf::Color::Red, sf::Vector2f(WIDTH / 2, HEIGHT / 2));
    auto boundaries = createBoundaryRectangles(WIDTH, HEIGHT);
    sf::Vector2f shapeVelocity = sf::Vector2f(10, -10);
    while (window->isOpen())
    {
        // HANDLE EVENTS
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window->close();
                }
                if (event.key.code == sf::Keyboard::Up) {
                    shapeVelocity.y -= 10;
                }
                if (event.key.code == sf::Keyboard::Down) {
                    shapeVelocity.y += 10;
                }
                if (event.key.code == sf::Keyboard::Left) {
                    shapeVelocity.x -= 10;
                }
                if (event.key.code == sf::Keyboard::Right) {
                    shapeVelocity.x += 10;
                }
                if (event.key.code == sf::Keyboard::Space) {
                    shapeVelocity = sf::Vector2f(10, -10);
                    shape->setPosition(WIDTH / 2, HEIGHT / 2);
                }
            }
        }
        // UPDATE TIME
        dt = clock.restart().asSeconds();
        // UPDATE FRAME
        // COLLISION DETECTION WITH BOUNDARY WITH TIMESTEP
        for (auto& boundary : boundaries) {
            if (shape->getGlobalBounds().intersects(boundary->getGlobalBounds())) {
                if (boundary->getPosition().x == 0 || boundary->getPosition().x == WIDTH) {
                    if (boundary->getPosition().x == 0) {
                        shape->setPosition(shape->getPosition().x + 3, shape->getPosition().y);
                    }
                    else {
                        shape->setPosition(shape->getPosition().x - 3, shape->getPosition().y);
                    }
                    shapeVelocity.x *= -1;
                }
                if (boundary->getPosition().y == 0 || boundary->getPosition().y == HEIGHT) {
                    if (boundary->getPosition().y == 0) {
                        shape->setPosition(shape->getPosition().x, shape->getPosition().y + 3);
                    }
                    else {
                        shape->setPosition(shape->getPosition().x, shape->getPosition().y - 3);
                    }
                    shapeVelocity.y *= -1;
                }
            }
        }
        p1 += shapeVelocity * dt;
        // MOVE BALL
        shape->setPosition(p1);
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
    return window;
}

std::unique_ptr<sf::CircleShape> createBall(int radius, sf::Color color, sf::Vector2f position) {
    auto ball = std::make_unique<sf::CircleShape>(radius);
    ball->setFillColor(color);
    ball->setPosition(position);
    ball->setOrigin(radius / 2, radius / 2);
    return ball;
}

std::vector<std::unique_ptr<sf::RectangleShape>> createBoundaryRectangles(int width, int height) {
    std::vector<std::unique_ptr<sf::RectangleShape>> boundaries;
    auto top = std::make_unique<sf::RectangleShape>(sf::Vector2f(width, 1));
    top->setPosition(0, 0);
    boundaries.push_back(std::move(top));
    auto bottom = std::make_unique<sf::RectangleShape>(sf::Vector2f(width, 1));
    bottom->setPosition(0, height);
    boundaries.push_back(std::move(bottom));
    auto left = std::make_unique<sf::RectangleShape>(sf::Vector2f(1, height));
    left->setPosition(0, 0);
    boundaries.push_back(std::move(left));
    auto right = std::make_unique<sf::RectangleShape>(sf::Vector2f(1, height));
    right->setPosition(width, 0);
    boundaries.push_back(std::move(right));
    return boundaries;
}