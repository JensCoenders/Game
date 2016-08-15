#include <SDL_image.h>
#include <iostream>

#include "shared.h"

/* Shared Memory */

bool Game_SharedMemory::p_running = false;
int Game_SharedMemory::p_targetFPS = 60;
bool Game_SharedMemory::p_useFPSCounter = true;

Game_Camera Game_SharedMemory::w_mainCamera = {{0, 0}, {1024, 576}, 0, 2};
bool Game_SharedMemory::w_keyboardMovesCamera = true;
double Game_SharedMemory::w_zoomScale = 1.0;

Game_RenderLayer* Game_SharedMemory::r_renderLayers = new Game_RenderLayer[GAME_LAYER_AMOUNT];
int Game_SharedMemory::r_renderThreadID = -1;

bool Game_SharedMemory::s_SDLInitialized = false;
SDL_Renderer* Game_SharedMemory::s_mainRenderer = NULL;
SDL_Window* Game_SharedMemory::s_window = NULL;

string Game_SharedMemory::m_assetsFolder = "assets";
Game_AdvancedObject* Game_SharedMemory::m_keyboardInputObject = NULL;	// TODO: Replace input object by input handler
TTF_Font* Game_SharedMemory::m_guiFont = NULL;

bool Game_Tools::addGameObject(Game_Object* object, unsigned int layerID)
{
	if (layerID >= GAME_LAYER_AMOUNT)
	{
		cout << "[WARN] Invalid layer ID provided: " << layerID << endl;
		return false;
	}

	Game_RenderLayer* layer = &Game_SharedMemory::r_renderLayers[layerID];
	LinkedListNode<Game_Object>* newNode = new LinkedListNode<Game_Object>();
	newNode->value = object;
	newNode->nextNode = layer->objectList;

	layer->objectList = newNode;
	layer->objectCount++;

	return true;
}

bool Game_Tools::removeGameObject(Game_Object* object)
{
	for (int i = 0; i < GAME_LAYER_AMOUNT; i++)
	{
		Game_RenderLayer* currentLayer = &Game_SharedMemory::r_renderLayers[i];
		LinkedListNode<Game_Object>* currentNode = currentLayer->objectList;
		while (currentNode)
		{
			if (currentNode->value->getID() == object->getID())
			{
				if (currentNode->prevNode)
					currentNode->prevNode->nextNode = currentNode->nextNode;

				if (currentNode->nextNode)
					currentNode->nextNode->prevNode = currentNode->prevNode;

				delete currentNode;
				currentLayer->objectCount--;

				return true;
			}

			currentNode = currentNode->nextNode;
		}
	}

	return false;
}

Game_Rect Game_Tools::getTextSize(string text)
{
	TTF_Font* font = Game_SharedMemory::m_guiFont;
	Game_Rect dest = {0, 0};

	if (TTF_SizeText(font, text.c_str(), &dest.width, &dest.height))
		cout << "[ERR] Couldn't get size of text '" << text << "'!" << endl;

	return dest;
}

Game_RenderEquipment* Game_Tools::createRenderEquipment(int surfaceWidth, int surfaceHeight)
{
	// Create surface
	SDL_Surface* surface = SDL_CreateRGBSurface(0, surfaceWidth, surfaceHeight, 32, GAME_SURFACE_RMASK, GAME_SURFACE_GMASK,
	GAME_SURFACE_BMASK, GAME_SURFACE_AMASK);

	if (!surface)
	{
		cout << "[ERR] Couldn't create SDL_Surface for render equipment: " << SDL_GetError() << endl;
		return NULL;
	}

	// Create equipment
	SDL_Renderer* renderer = SDL_CreateSoftwareRenderer(surface);
	if (!renderer)
	{
		cout << "[ERR] Couldn't create SDL_Renderer for render equipment: " << SDL_GetError() << endl;
		return NULL;
	}

	return new Game_RenderEquipment(renderer, surface);
}

SDL_Surface* Game_Tools::imageTextureObjectTU(Game_Object& object, Game_RenderEquipment* equipment)
{
	SDL_Surface* imageTextureSurface = IMG_Load(object.getImageTexturePath().c_str());
	if (!imageTextureSurface)
	{
		cout << "[ERR]: " << IMG_GetError() << endl;
		return NULL;
	}

	return imageTextureSurface;
}
