# SDL Joystick to WiFi Project

## Table of Contents
1. [Overview](#overview)
2. [Repository Structure](#repository-structure)
3. [Prerequisites](#prerequisites)
4. [Setup and Compilation](#setup-and-compilation)
5. [Usage](#usage)
6. [Key Components](#key-components)
7. [Joystick Axis Mapping](#joystick-axis-mapping)
8. [Configuration](#configuration)
9. [Contributing](#contributing)
10. [Acknowledgments](#acknowledgments)

## Overview
This project enables the use of an SDL-compatible joystick to send WiFi commands, potentially for controlling remote devices or robots. It uses the SDL2 library to read joystick input and sends the data over WiFi using a custom WifiSend implementation.

## Repository Structure
- `SDL2/` - Directory containing the SDL2 library files
- `README.md` - This file, providing project documentation
- `SDL2.dll` - SDL2 dynamic link library for Windows
- `WifiSend.h` - Header file containing WiFi sending functionality
- `main.cpp` - Main application code for joystick input and WiFi transmission

## Prerequisites
- C++ compiler with C++11 support
- SDL2 library (included in the repository)
- Windows OS (due to the use of Winsock2)

## Setup and Compilation
1. Ensure you have a C++ compiler installed on your system.
2. The SDL2 library is already included in the repository, so no additional download is necessary.
3. Compile the project using your preferred C++ compiler. For example, using g++:
   ```
   g++ main.cpp -o joystick_wifi -I./SDL2/include -L./SDL2/lib -lSDL2 -lws2_32
   ```
4. Make sure `SDL2.dll` is in the same directory as your compiled executable or in your system PATH.

## Usage
1. Connect an SDL-compatible joystick to your computer.
2. Run the compiled executable.
3. The program will initialize the joystick and begin sending data over WiFi.
4. Joystick movements will be translated into WiFi commands sent to the specified IP address (default: 192.168.4.1).

## Key Components

### main.cpp
Contains the main application logic:
- Initializes SDL and joystick
- Reads joystick input
- Processes and maps joystick values
- Calls WiFi sending function

### WifiSend.h
Implements the WiFi sending functionality:
- Sets up a Winsock connection
- Connects to the specified IP address
- Sends joystick data over the network

### SDL2 Library
Provides joystick input functionality:
- Joystick detection and initialization
- Reading axis values

## Joystick Axis Mapping

The program interprets joystick inputs based on the following axis mapping:

| Axis | Control               |
|------|----------------------|
| 0    | Left Joystick LEFT-RIGHT |
| 1    | Left Joystick UP-DOWN |
| 2    | Right Joystick LEFT-RIGHT |
| 3    | Right Joystick UP-DOWN |
| 4    | Left Trigger         |
| 5    | Right Trigger        |

### Understanding the Mapping:

- **Axes 0 and 1**: These correspond to the left analog stick. Axis 0 controls horizontal movement (left to right), while Axis 1 controls vertical movement (up and down).
- **Axes 2 and 3**: These map to the right analog stick, following the same pattern as the left stick.
- **Axes 4 and 5**: These are typically analog triggers, which can provide a range of values based on how far they are pressed.

### Value Ranges:

- For axes 0-3 (analog sticks), the values typically range from -32768 to 32767:
  - 0 represents the center position
  - -32768 is the leftmost or uppermost position
  - 32767 is the rightmost or lowermost position
- For axes 4-5 (triggers), the range is usually 0 to 32767:
  - 0 represents not pressed
  - 32767 represents fully pressed

### In the Code:

The `main.cpp` file currently uses axes 0 and 1 (left joystick) for control:

```cpp
int xAxis = SDL_JoystickGetAxis(joystick, 0);
int yAxis = SDL_JoystickGetAxis(joystick, 1);
```

These values are then mapped to a 0-255 range for WiFi transmission.

### Customization:

You can modify which axes are used by changing the axis numbers in the `SDL_JoystickGetAxis` calls. For example, to use the right stick instead, you would use:

```cpp
int xAxis = SDL_JoystickGetAxis(joystick, 2);
int yAxis = SDL_JoystickGetAxis(joystick, 3);
```

## Configuration
- To change the target IP address, modify the `ESP32_IP` constant in `WifiSend.h`.
- Joystick sensitivity and mapping can be adjusted in `main.cpp`.

## Usage Tips

- Ensure your joystick is properly calibrated for accurate input.
- The dead zone implemented in the code helps to ignore small, unintentional movements of the joystick.
- If you're using this to control a robot or other device, consider how the axis values map to your device's movement capabilities.

## Contributing
Contributions to improve the project are welcome. Please follow these steps:
1. Fork the repository
2. Create a new branch for your feature
3. Commit your changes
4. Push to your branch
5. Create a new Pull Request

## Acknowledgments
- SDL2 library developers
- My mental peace
