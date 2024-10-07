#include <iostream>
#include <stdexcept>
#include <SDL.h>
#include "WifiSend.h"

/*
Joystick Axis Mapping:
0 = Left Joystick LEFT-RIGHT
1 = Left Joystick UP-DOWN
2 = Right Joystick LEFT-RIGHT
3 = Right Joystick UP-DOWN
4 = Left Trigger
5 = Right Trigger
*/

// Constants
const int JOYSTICK_DEAD_ZONE = 8000;
const int DELAY_MS = 500;

// Function to map joystick values to a new range
int mapJoystickValue(int value, int oldMin, int oldMax, int newMin, int newMax) {
    return ((value - oldMin) * (newMax - newMin) / (oldMax - oldMin)) + newMin;
}

// Function to initialize SDL and Joystick
SDL_Joystick* initializeSDLAndJoystick() {
    if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
        throw std::runtime_error("SDL could not initialize! SDL Error: " + std::string(SDL_GetError()));
    }

    if (SDL_NumJoysticks() < 1) {
        throw std::runtime_error("No joysticks connected!");
    }

    SDL_Joystick* joystick = SDL_JoystickOpen(0);
    if (joystick == nullptr) {
        throw std::runtime_error("Unable to open joystick! SDL Error: " + std::string(SDL_GetError()));
    }

    return joystick;
}

// Function to print joystick information
void printJoystickInfo(SDL_Joystick* joystick) {
    std::cout << "Controller(s): " << SDL_NumJoysticks() << '\n';
    std::cout << "Controller Name: " << SDL_JoystickName(joystick) << '\n';
    std::cout << "Axes: " << SDL_JoystickNumAxes(joystick) << '\n';
    std::cout << "Buttons: " << SDL_JoystickNumButtons(joystick) << '\n';
}

int main(int argc, char* argv[]) {
    SDL_Joystick* joystick = nullptr;

    try {
        joystick = initializeSDLAndJoystick();
        printJoystickInfo(joystick);

        while (true) {
            SDL_JoystickUpdate();

            int xAxis = SDL_JoystickGetAxis(joystick, 0);
            int yAxis = SDL_JoystickGetAxis(joystick, 1);

            // Apply dead zone
            if (abs(xAxis) < JOYSTICK_DEAD_ZONE) xAxis = 0;
            if (abs(yAxis) < JOYSTICK_DEAD_ZONE) yAxis = 0;

            // Map joystick values to 0-255 range
            int x = mapJoystickValue(xAxis, -32768, 32767, 0, 255);
            int y = mapJoystickValue(yAxis, -32768, 32767, 0, 255);

            int data[3] = {0, y, x};
            std::cout << "X-Axis: " << x << "\n";
            std::cout << "Y-Axis: " << y << "\n";

            Send(data);
            SDL_Delay(DELAY_MS);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // Cleanup
    if (joystick) {
        SDL_JoystickClose(joystick);
    }
    SDL_Quit();

    return 0;
}
