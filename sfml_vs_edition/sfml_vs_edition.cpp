#include <iostream>
#include <ctime>
#include <SFML/Graphics.hpp>

constexpr auto WIDTH = 1280;
constexpr auto HEIGHT = 720;
constexpr auto TITLE = "SFML VS Edition";
constexpr auto FPS = 80;

float getSystemTime() {
	return static_cast<float>(clock()) / CLOCKS_PER_SEC;
}

class Ball {
private:
	sf::CircleShape shape;
	sf::Vector2f velocity;
	sf::Vector2f position;
	float radius;
	sf::Color color;

public:
	Ball(float radius, sf::Color color, sf::Vector2f position, sf::Vector2f velocity) : radius(radius), color(color), position(position), velocity(velocity) {
		shape.setRadius(radius);
		shape.setFillColor(color);
		shape.setPosition(position);
	}

	~Ball() {}

	void update(float del) {
		position += velocity * del;
		shape.setPosition(position);
	}

	void draw(sf::RenderWindow& window) {
		window.draw(shape);
	}

	void move(sf::Vector2f offset) {
		position += offset;
		shape.setPosition(position);
	}

	void move(float offsetX, float offsetY) {
		position += sf::Vector2f(offsetX, offsetY);
		shape.setPosition(position);
	}

	void boundaryCheck(float width, float height) {
		if (position.x - this->radius < 0 || position.x + this->radius > width) {
			shape.setPosition(position.x < 0 ? 0 : width, position.y);
			velocity.x = -velocity.x;
		}
		if (position.y - this->radius < 0 || position.y + this->radius > height) {
			shape.setPosition(position.x, position.y < 0 ? 0 : height);
			velocity.y = -velocity.y;
		}
	}

	sf::Vector2f getPosition() {
		return position;
	}

	sf::Vector2f getVelocity() {
		return velocity;
	}

	float getRadius() {
		return radius;
	}

	sf::Color getColor() {
		return color;
	}

	sf::CircleShape getShape() {
		return shape;
	}

	void setPosition(sf::Vector2f position) {
		this->position = position;
		shape.setPosition(position);
	}

	void setVelocity(sf::Vector2f velocity) {
		this->velocity = velocity;
	}

	void setRadius(float radius) {
		this->radius = radius;
		shape.setRadius(radius);
	}

	void setColor(sf::Color color) {
		this->color = color;
		shape.setFillColor(color);
	}

	void setShape(sf::CircleShape shape) {
		this->shape = shape;
	}

	void setFillColor(sf::Color color) {
		shape.setFillColor(color);
	}

	void setOutlineColor(sf::Color color) {
		shape.setOutlineColor(color);
	}

	void setOutlineThickness(float thickness) {
		shape.setOutlineThickness(thickness);
	}

};

int main(int argc, char* argv[]) {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.majorVersion = 4;
	settings.minorVersion = 6;
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), TITLE, sf::Style::Default, settings);
	// declare ball
	Ball* ball = new Ball(30, sf::Color::Red, sf::Vector2f(WIDTH / 2, HEIGHT / 2), sf::Vector2f(600, 600));
	const float TARGET_TIMESTEP = 0.01f; // 75 FPS 
	float lastFrameTime = 0.0f;
	float accumulator = 0.0;
    while (window.isOpen())
    {
		float currTime = getSystemTime();
		float deltaTime = currTime - lastFrameTime;
		lastFrameTime = currTime;
		accumulator += deltaTime;
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		while (accumulator >= TARGET_TIMESTEP) {
			ball->update(TARGET_TIMESTEP);
			ball->boundaryCheck(WIDTH, HEIGHT);
			accumulator -= TARGET_TIMESTEP;
		}


        window.clear();
		ball->draw(window);
        window.display();
    }
	ball->~Ball();
	return 0;
}