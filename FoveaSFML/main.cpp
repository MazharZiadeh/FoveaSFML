#include <SFML/Graphics.hpp>
// Define constants for window size and grid size
const int windowSize = 1080;
const int gridSize = 64;

// Function to calculate pixel intensity based on distance and maximum distance
sf::Uint8 calculateIntensity(float distance, float maxDistance) {
    // Calculate intensity based on the distance from the mouse position
    float intensity = 1.0f - (distance / maxDistance);
    // Clamp intensity to the range [0, 1]
    intensity = std::max(0.0f, std::min(1.0f, intensity));
    // Convert intensity to sf::Uint8 (8-bit unsigned integer) for color representation
    return static_cast<sf::Uint8>(intensity * 255);
}

int main() {

    // Create a window with specified size and title
    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "Foveated Rendering");

    // Hide the mouse cursor
    window.setMouseCursorVisible(false);

    // Set the maximum distance from the center for intensity calculation
    float maxDistance = windowSize / 4.0f;
    // Calculate the size of each square in the grid
    int squareSize = windowSize / gridSize;

    // Define radii for central and mid circles
    int CenterCircleRadius = windowSize / 4;
    int MidCircleRadius = windowSize / 2;

    // Boolean flag to determine if 'P' key is pressed
    bool pKeyPressed = false;

    // Main game loop
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            // If the window is closed, exit the loop and close the window
            if (event.type == sf::Event::Closed)
                window.close();

            // Check if the 'P' key is pressed
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
                pKeyPressed = !pKeyPressed; // Toggle the flag
            }
        }

        // Get the current mouse position
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

        // Clear the window
        window.clear();

        // Iterate through the grid
        for (int i = 0; i < gridSize; ++i) {
            for (int j = 0; j < gridSize; ++j) {
                // Create a square with the calculated size
                sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
                square.setPosition(j * squareSize, i * squareSize);

                // Check if 'P' key is pressed, rendering the entire grid without foveated rendering
                if (pKeyPressed) {
                    square.setFillColor(sf::Color(255, 255, 255)); // Set the fill color to white
                }
                else {
                    // Calculate the distance from the center of the square to the mouse position
                    float centerX = j * squareSize + squareSize / 2.0f;
                    float centerY = i * squareSize + squareSize / 2.0f;
                    float distance = std::sqrt((centerX - mousePosition.x) * (centerX - mousePosition.x) +
                        (centerY - mousePosition.y) * (centerY - mousePosition.y));

                    // Set default resolution factor
                    float resolutionFactor = 1.0f;

                    // Adjust resolution factor based on the distance from the center
                    if (distance > CenterCircleRadius) {
                        // If the distance is within the mid circle, change the resolution factor
                        if (distance <= MidCircleRadius) {
                            // Setting to 0.6f results in less impact on intensity for mid distances
                            resolutionFactor = 0.6f;
                        }
                        else {
                            // Setting to 0.25f results in a significant reduction in intensity for far distances
                            resolutionFactor = 0.25f;
                        }
                    }

                    // Calculate intensity based on the distance and adjust by resolution factor
                    sf::Uint8 intensity = calculateIntensity(distance, maxDistance);
                    intensity = static_cast<sf::Uint8>(intensity * resolutionFactor);

                    // Set the fill color of the square based on the calculated intensity
                    square.setFillColor(sf::Color(intensity, intensity, intensity));
                }

                // Draw the square on the window
                window.draw(square);
            }
        }

        // Display the contents of the window
        window.display();
    }

    // Exit the program
    return 0;
}
