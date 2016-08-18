#include <iostream>

#include "testgame.h"

using namespace std;

int initialize()
{
	// Initialize SDL
	string windowTitle = "Jens Game ";
	windowTitle += GAME_VERSION;

	cout << "[INFO] Initializing SDL... ";
	int returnCode = game_initializeSDL(windowTitle);
	if (!returnCode)
		cout << "[OK]" << endl;
	else
	{
		switch (returnCode)
		{
			case -2:
				cout << "[CRIT]" << endl;
				cout << "[CRIT] Failed to initialize SDL! Error: " << SDL_GetError() << endl;
				break;
			case -3:
				cout << "[CRIT]" << endl;
				cout << "[CRIT] Failed to initialize SDL_image! Error: " << IMG_GetError() << endl;
				break;
			case -4:
				cout << "[CRIT]" << endl;
				cout << "[CRIT] Failed to initialize SDL_ttf! Error: " << TTF_GetError() << endl;
				break;
		}

		game_destroySDL();
		return returnCode;
	}

	return 0;
}

void displayHelp(string programName)
{
	cout << "Usage: " << programName << " [options]" << endl;
	cout << "--assets <path>\t\tSpecify a different assets directory." << endl;
	cout << "This software was developed by Jens Coenders" << endl;
}

bool processArguments(int argc, char** argv)
{
	for (int i = 1; i < argc; i++)
	{
		string argument = argv[i];
		for (unsigned int j = 0; j < argument.size(); j++)
			argument[j] = tolower(argument[j]);

		if (argument == "--assets" && (i + 1) < argc)
		{
			Game_SharedMemory::m_assetsFolder = argv[++i];
			cout << "[INFO] Setting assets folder to '" << Game_SharedMemory::m_assetsFolder << "'" << endl;
		}
		else if (argument == "--help")
		{
			displayHelp(argv[0]);
			return false;
		}
		else
		{
			cout << "Unknown argument '" << argument << "'!" << endl;
			displayHelp(argv[0]);
			return false;
		}
	}

	return true;
}

int main(int argc, char** argv)
{
	// TODO: Fix position glitch when window resizes
	// TODO: Fix random window crash when window resizes
	// TODO: Create parent-child system

	// Process arguments
	if (!processArguments(argc, argv))
		return -1;

	// Initialize
	int result = initialize();
	if (result)
		return result;

	// Run game
	runTestGame();
	cout << "[INFO] Stopped main thread" << endl;

	// Cleanup
	game_destroySDL();
	delete[] Game_SharedMemory::r_renderLayers;

	cout << "[INFO] Cleanup complete";
	return 0;
}
