#include <circle.h>

// Constructor: Initialize position, radius, color, and create the circle texture
Circle::Circle(float x, float y, float radius, SDL_Color color, SDL_Renderer* renderer)
    : pos(x, y), radius(radius), color(color), velocity(0.0f, 0.0f), acceleration(0.0f, 0.0f), renderer(renderer), circleTexture(nullptr) {
    createCircleTexture();
}

// Destructor: Clean up the texture when done
Circle::~Circle() {
    if (circleTexture) {
        SDL_DestroyTexture(circleTexture);
    }
}

// Helper function to create the circle texture
void Circle::createCircleTexture() {
    // Clean up the old texture if it exists
    if (circleTexture) {
        SDL_DestroyTexture(circleTexture);
    }

    // Create an SDL_Surface with a size of 2 * radius (square)
    SDL_Surface* surface = SDL_CreateRGBSurface(0, 2 * radius, 2 * radius, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

    // Fill the surface with transparency
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 0, 0)); // Fill with transparent background

    // Draw the filled circle onto the surface
    for (int y = 0; y < 2 * radius; ++y) {
        for (int x = 0; x < 2 * radius; ++x) {
            int dx = x - radius;
            int dy = y - radius;
            if (dx * dx + dy * dy <= radius * radius) {
                Uint32 colorVal = SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a);
                ((Uint32*)surface->pixels)[y * surface->w + x] = colorVal;
            }
        }
    }

    // Create a texture from the surface
    circleTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // Free the surface as we no longer need it
}

// Draw the circle using the cached texture
void Circle::draw() {
    if (!circleTexture) {
        createCircleTexture();  // Ensure texture is created if not done yet
    }

    // Define destination rectangle for positioning and scaling the circle texture
    SDL_Rect dstRect = { (int)(pos.x - radius), (int)(pos.y - radius), (int)(2 * radius), (int)(2 * radius) };

    // Draw the circle texture at the specified position and scale
    SDL_RenderCopyEx(renderer, circleTexture, NULL, &dstRect, 0.0, NULL, SDL_FLIP_NONE);
}

// Move the circle by a certain amount (used for position updates)
void Circle::move(float x, float y) {
    pos.x += x;
    pos.y += y;
}

// Set the position of the circle directly
void Circle::setPos(float x, float y) {
    pos.x = x;
    pos.y = y;
}

// Set the radius of the circle
void Circle::setRadius(float radius) {
    this->radius = radius;
    createCircleTexture(); // Recreate the texture with the new radius
}

// Set the color of the circle
void Circle::setColor(SDL_Color color) {
    this->color = color;
    createCircleTexture(); // Recreate the texture with the new color
}

// Set the velocity of the circle using individual components
void Circle::setVelocity(float x, float y) {
    velocity = glm::vec2(x, y);
}

// Set the velocity of the circle using a glm::vec2
void Circle::setVelocity(glm::vec2 velocity) {
    this->velocity = velocity;
}

// Set the acceleration of the circle using individual components
void Circle::setAcceleration(float x, float y) {
    acceleration = glm::vec2(x, y);
}

// Set the acceleration of the circle using a glm::vec2
void Circle::setAcceleration(glm::vec2 acceleration) {
    this->acceleration = acceleration;
}

// Update the position of the circle based on velocity and acceleration
void Circle::update(float dt) {
    // Update velocity based on acceleration
    velocity += acceleration * dt;

    // Update position based on velocity
    pos += velocity * dt;
}

// Getter functions
glm::vec2 Circle::getPos() const {
    return pos;
}

float Circle::getRadius() const {
    return radius;
}

SDL_Color Circle::getColor() const {
    return color;
}

glm::vec2 Circle::getVelocity() const {
    return velocity;
}

glm::vec2 Circle::getAcceleration() const {
    return acceleration;
}