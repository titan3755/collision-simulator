#include <circle.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <SDL.h>
#undef main // SDL defines main, so we need to undefine it
#include <SDL_image.h>
#include <SDL_ttf.h>

constexpr auto SCREEN_WIDTH = 1280;
constexpr auto SCREEN_HEIGHT = 720;

bool checkIfMouseIsWithinWindowAndLeftMouseButtonIsPressed(SDL_Window* window) {
	int x, y;
	Uint32 mouseState = SDL_GetMouseState(&x, &y);
	Uint32 windowFlags = SDL_GetWindowFlags(window);
	return (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) && (windowFlags & SDL_WINDOW_MOUSE_FOCUS) && (windowFlags & SDL_WINDOW_INPUT_FOCUS);
}

static void setWindowTitleAccordingToFramerateAndTimeElapsed(SDL_Window* window) {
	static int frameCount = 0;
	static Uint32 lastFrameTime = 0;
	static char title[100];

	// Increment frame count every loop
	frameCount++;

	Uint32 currentTime = SDL_GetTicks();
	if (currentTime - lastFrameTime >= 1000) {  // Every second
		// Calculate FPS by dividing frames by elapsed time in seconds
		int fps = frameCount;  // FPS = frames per second
		int timeElapsedInSeconds = (currentTime - lastFrameTime) / 1000;

		// Set window title with the calculated FPS
		int succ = sprintf_s(title, "Ball_Physics | FPS: %d", fps);
		if (succ < 0) {
			std::cerr << "sprintf_s Error: " << succ << std::endl;
		}
		SDL_SetWindowTitle(window, title);

		// Reset for the next frame
		lastFrameTime = currentTime;
		frameCount = 0;
	}
}

void handleCircleCollision(Circle* circle1, Circle* circle2) {
	// Get the positions and radii of the circles
	glm::vec2 pos1 = circle1->getPos();
	glm::vec2 pos2 = circle2->getPos();
	float radius1 = circle1->getRadius();
	float radius2 = circle2->getRadius();

	// Calculate the distance vector between the two circle centers
	glm::vec2 diff = pos1 - pos2;
	float distance = glm::length(diff);

	// If the circles are colliding
	if (distance < radius1 + radius2) {
		// Normalize the vector to get the direction of collision
		glm::vec2 normal = glm::normalize(diff);

		// Calculate the relative velocity of the circles
		glm::vec2 velocity1 = circle1->getVelocity();
		glm::vec2 velocity2 = circle2->getVelocity();
		glm::vec2 relativeVelocity = velocity1 - velocity2;

		// Calculate the velocity along the normal direction (dot product)
		float velocityAlongNormal = glm::dot(relativeVelocity, normal);

		// If the circles are moving apart, no need to resolve
		if (velocityAlongNormal > 0) return;

		// Coefficient of restitution (elastic collision)
		float e = 0.9f; // Use 1.0 for perfectly elastic collisions

		// Calculate the impulse scalar (j)
		float j = -(1 + e) * velocityAlongNormal;
		j /= (1 / radius1 + 1 / radius2);

		// Apply the impulse to the velocities
		glm::vec2 impulse = j * normal;
		circle1->setVelocity(velocity1 + impulse / radius1);
		circle2->setVelocity(velocity2 - impulse / radius2);

		// Separate the circles to avoid overlap
		float overlap = radius1 + radius2 - distance;
		glm::vec2 correction = normal * (overlap / 2.0f);
		circle1->setPos(pos1.x + correction.x, pos1.y + correction.y);
		circle2->setPos(pos2.x - correction.x, pos2.y - correction.y);
	}
}


int main(int argc, char* argv[]) {
	// program start
	std::cout << "Hello, World!" << std::endl;

	// SDL2 initialization
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	else {
		std::cout << "SDL_Init Success!" << std::endl;
	}
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
		return 1;
	}
	else {
		std::cout << "IMG_Init Success!" << std::endl;
	}
	if (TTF_Init() != 0) {
		std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
		return 1;
	}
	else {
		std::cout << "TTF_Init Success!" << std::endl;
	}

	// SDL2 window creation
	SDL_Window* window = SDL_CreateWindow("Ball_Physics", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr || !window) {
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	else {
		std::cout << "SDL_CreateWindow Success!" << std::endl;
	}

	// SDL2 renderer creation
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr || !renderer) {
		std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	else {
		std::cout << "SDL_CreateRenderer Success!" << std::endl;
	}

	std::vector<Circle*> circlesVec;

	// SDL2 event loop
	int prevmouseX = 0, prevmouseY = 0;
	int mouseX = 0, mouseY = 0;
	SDL_Event event;
	bool running = true;
	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_SPACE) {
					// clear all circles
					for (int i = 0; i < circlesVec.size(); ++i) {
						delete circlesVec[i];
					}
					circlesVec.clear();
				}
			}
		}

		// set window title according to framerate and time elapsed
		setWindowTitleAccordingToFramerateAndTimeElapsed(window);

		// SDL2 render
		if (checkIfMouseIsWithinWindowAndLeftMouseButtonIsPressed(window)) {
			// spawn only one circle per mouse click
			SDL_GetMouseState(&mouseX, &mouseY);
			if (mouseX != prevmouseX || mouseY != prevmouseY) {
				circlesVec.push_back(new Circle(mouseX, mouseY, 10.0f, { (Uint8)(rand() % 256), (Uint8)(rand() % 256), (Uint8)(rand() % 256), 255 }, renderer));
				circlesVec.back()->setAcceleration(0.0f, 9.8f);
				prevmouseX = mouseX;
				prevmouseY = mouseY;
			}
		}

		// clear the screen
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		// Handle collisions between circles
		for (int i = 0; i < circlesVec.size(); ++i) {
			// Check for collision with every other circle in the list
			for (int j = i + 1; j < circlesVec.size(); ++j) {
				// Handle collision between circle i and circle j
				handleCircleCollision(circlesVec[i], circlesVec[j]);
			}

			// Update circle positions and draw them
			circlesVec[i]->update(1.0f / 60.0f);

			glm::vec2 pos = circlesVec[i]->getPos();
			float radius = circlesVec[i]->getRadius();

			// Check if the circle has passed the bottom boundary
			if (pos.y + radius > SCREEN_HEIGHT) {
				// Correct the position (stop at the bottom)
				circlesVec[i]->setPos(pos.x, SCREEN_HEIGHT - radius);
				// Invert the vertical velocity to simulate a bounce
				circlesVec[i]->setVelocity(circlesVec[i]->getVelocity().x, -circlesVec[i]->getVelocity().y);
			}

			// Check if the circle has passed the top boundary
			if (pos.y - radius < 0) {
				// Correct the position (stop at the top)
				circlesVec[i]->setPos(pos.x, radius);
				// Invert the vertical velocity to simulate a bounce
				circlesVec[i]->setVelocity(circlesVec[i]->getVelocity().x, -circlesVec[i]->getVelocity().y);
			}

			// Check if the circle has passed the left boundary
			if (pos.x - radius < 0) {
				// Correct the position (stop at the left)
				circlesVec[i]->setPos(radius, pos.y);
				// Invert the horizontal velocity to simulate a bounce
				circlesVec[i]->setVelocity(-circlesVec[i]->getVelocity().x, circlesVec[i]->getVelocity().y);
			}

			// Check if the circle has passed the right boundary
			if (pos.x + radius > SCREEN_WIDTH) {
				// Correct the position (stop at the right)
				circlesVec[i]->setPos(SCREEN_WIDTH - radius, pos.y);
				// Invert the horizontal velocity to simulate a bounce
				circlesVec[i]->setVelocity(-circlesVec[i]->getVelocity().x, circlesVec[i]->getVelocity().y);
			}

			// Draw the circle
			circlesVec[i]->draw();
		}

		// present the screen
		SDL_RenderPresent(renderer);

		// errors
		if (SDL_GetError() != nullptr && SDL_GetError()[0] != '\0') {
			std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
		}
	}

	// SDL2 cleanup
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	// program end
	std::cout << "Goodbye, World!" << std::endl;

	return 0;
}