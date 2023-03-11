#pragma once

#include "SDL/include/SDL.h"
#include "SDL/include/SDL.h"
#include <vector>
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )
#include "SDL_image/include/SDL_image.h"
#pragma comment(lib, "SDL_image/libx86/SDL2_image.lib")
#include "Entity.h"




#define WINDOW_WIDTH	1249
#define WINDOW_HEIGHT	512
#define MAX_KEYS		256
#define MAX_SHOTS		200
#define MAX_ENEMIES		40

class Game
{
public:
	Game();
	~Game();

	bool Init();
	void Release();
	
	bool Input();
	bool Update();
	void Draw();
	

private:
	SDL_Window *Window;
	SDL_Window* gameOverWindow;
	SDL_Renderer *Renderer;
	SDL_Texture* spriteTexture;
	SDL_Texture* shooting;
	SDL_Texture* background;
	SDL_Texture* robot;
	SDL_Texture* laser;

	Entity Player, Shots[MAX_SHOTS], scene, Enemy[40], enemyShots[MAX_SHOTS];
	/*std::vector<Entity> enemigos;*/
	int idx_shot;
	int fxRob = 1;

	bool god_mode;

	enum KEY_STATE { KEY_IDLE, KEY_DOWN, KEY_REPEAT, KEY_UP	};
	KEY_STATE keys[MAX_KEYS]; 
};
