#include <iostream>
#include <SDL.h>
#include "WifiSend.h"


int main(int argc, char* argv[])
{

	SDL_Init(SDL_INIT_JOYSTICK);

	if (SDL_NumJoysticks() < 1)
		std::cout << "Disconnected" << '\n';
	else
	{
		SDL_Joystick* joystick = SDL_JoystickOpen(0);
	}


	//SDL Declarations
	SDL_Joystick* joystick = SDL_JoystickOpen(0);

	//Joystick Info
	std::cout << "Controller(s): " << SDL_NumJoysticks() << '\n';
	std::cout << "Controller Name: " << SDL_JoystickName(joystick) << '\n';
	std::cout << "Axes: " << SDL_JoystickNumAxes(joystick) << '\n';
	std::cout << "Buttons: " << SDL_JoystickNumButtons(joystick) << '\n';

	//Actual Stuff
	while (true)
	{
		SDL_JoystickUpdate();

		int y = (((SDL_JoystickGetAxis(joystick, 1) + 32767) * (255)) / 65534);
		int x = (((SDL_JoystickGetAxis(joystick, 0) + 32767) * (255)) / 65534);

		int data[3] = { 0,y,x };

		std::cout << "Y-Axis: " << y << "\n";
		std::cout << "X-Axis: " << x << "\n";

		Send(data);
		SDL_Delay(500);
	}

	return 0;
}


/*
0 = Left Joystick LEFT-RIGHT
1 = Left Joystick UP-DOWN
2 = Right Joystick LEFT-RIGHT
3 = Right Joystick UP-DOWN
4 = Left Trigger
5 = Right Trigger


new_value = ((old_value - old_min) * (new_max - new_min) / (old_max - old_min)) + new_min
*/
