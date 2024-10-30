#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

constexpr auto SCREEN_WIDTH = 1280;
constexpr auto SCREEN_HEIGHT = 720;
constexpr auto WINDOW_TITLE = "SDL2 Starter";

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Surface* optimizedSurface = NULL;
SDL_Surface* pngDemoImage = NULL;
SDL_Surface* demoImage = NULL;

bool initEverythingSDL();
bool loadMedia();
SDL_Surface* loadSurface(std::string path);
void close();

int main(int argc, char* args[])
{
    std::cout << "Initializing SDL...\n";
	bool success = initEverythingSDL();
	if (!success) {
		std::cout << "Failed to initialize SDL.\n";
		return -1;
	}
	std::cout << "SDL Initialized.\n";
	std::cout << "Loading media...\n";
	success = loadMedia();
	if (!success) {
		std::cout << "Failed to load media.\n";
		close();
		return -1;
	}
	optimizedSurface = loadSurface("assets/demo.bmp");
	if (!optimizedSurface || optimizedSurface == NULL) {
		std::cout << "Failed to load optimized surface.\n";
		close();
		return -1;
	}
	pngDemoImage = loadSurface("assets/demo_png.png");
	if (!pngDemoImage || pngDemoImage == NULL) {
		std::cout << "Failed to load PNG image.\n";
		close();
		return -1;
	}
	SDL_Rect stretchRect;
	stretchRect.x = 0;
	stretchRect.y = 0;
	stretchRect.w = SCREEN_WIDTH;
	stretchRect.h = SCREEN_HEIGHT;
	std::cout << "Media and Surfaces loaded.\n";
	bool quit = false;
	SDL_Event e;
	while (!quit) {
		// event loop
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}
		SDL_BlitScaled(pngDemoImage, NULL, screenSurface, &stretchRect);
		SDL_UpdateWindowSurface(window);
	}
	close();
	return 0;
}

bool initEverythingSDL() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Could not initialize SDL2. Err: " << SDL_GetError() << std::endl;
		return false;
	}
	window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window) {
		std::cout << "Could not create window. Err: " << SDL_GetError() << std::endl;
		return false;
	}
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}
	screenSurface = SDL_GetWindowSurface(window);
	if (!screenSurface) {
		std::cout << "Could not get window surface. Err: " << SDL_GetError() << std::endl;
		return false;
	}
	return true;
}

bool loadMedia() {
	demoImage = SDL_LoadBMP("assets/demo.bmp");
	if (!demoImage) {
		std::cout << "Could not load image. Err: " << SDL_GetError() << std::endl;
		return false;
	}
	return true;
}

SDL_Surface* loadSurface(std::string path) {
	SDL_Surface* optimizedSurface = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (!loadedSurface) {
		std::cout << "Could not load image. Err: " << SDL_GetError() << std::endl;
		return NULL;
	}
	optimizedSurface = SDL_ConvertSurface(loadedSurface, screenSurface->format, 0);
	if (!optimizedSurface) {
		std::cout << "Could not optimize image. Err: " << IMG_GetError() << std::endl;
		return loadedSurface;
	}
	SDL_FreeSurface(loadedSurface);
	return optimizedSurface;
}

void close() {
	SDL_FreeSurface(demoImage);
	demoImage = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
}
