#ifndef GAME_TOOLS_H
#define GAME_TOOLS_H

#include <string>
#include <SDL2/SDL_ttf.h>
#include "game_global.h"
#include "game_object.h"

/* General */
bool game_isInside(Game_Point parentPos, Game_Rect parentSize, Game_Point childPos, Game_Rect childSize, bool forcedInside);
bool game_isRenderPosInside(Game_Object& parent, Game_Object& child, bool forceFullyInside);

/* Assets */
bool game_loadAsset(std::string assetPath);
SDL_Surface* game_getAsset(std::string assetPath, bool loadAsset = false);
void game_freeAssets();

/* Object */
bool game_addGameObject(Game_Object* object, int layerID);
bool game_removeGameObject(Game_Object* object);

Game_Point game_getObjectRenderPos(Game_Object& object);
Game_Rect game_getObjectRenderSize(Game_Object& object);

Game_Object* game_findObjectByID(unsigned int objectID);

/* Rendering */
Game_RenderEquipment* game_createRenderEquipment(int surfaceWidth, int surfaceHeight);

/* Misc */
Game_Rect game_getTextSize(std::string text, TTF_Font* font = gameVar_guiFont);

SDL_Surface* colorBackgroundTU(Game_Object& object, Game_RenderEquipment& equipment);
SDL_Surface* imageTextureObjectTU(Game_Object& object, Game_RenderEquipment& equipment);
SDL_Surface* textObjectTU(Game_Object& object, Game_RenderEquipment& equipment);

/* Function definitions */

template <typename First, typename ... Rest>
std::string game_getAssetPath(First name, Rest&... subDirs)
{
	return gameVar_assetDir + "\\" + std::string(name) + "\\" + combineStringPath(subDirs...);
}

template <typename First, typename ... Rest>
Game_ModuleType game_combineModules(First firstModule, Rest ... restModules)
{
	return (Game_ModuleType) ((int) firstModule | combineModuleTypes(restModules...));
}

#endif
