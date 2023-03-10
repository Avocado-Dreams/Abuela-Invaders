#include "Game.h"
#include <math.h>

Game::Game() {}
Game::~Game(){}

bool Game::Init()

{
	//Initialize SDL with all subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	//Create our window: title, x, y, w, h, flags
	Window = SDL_CreateWindow("Spaceship: arrow keys + space", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (Window == NULL)
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
		return false;
	}
	//Create a 2D rendering context for a window: window, device index, flags
	Renderer = SDL_CreateRenderer(Window, -1, 0);
	if (Renderer == NULL)
	{
		SDL_Log("Unable to create rendering context: %s", SDL_GetError());
		return false;
	}
	//Initialize keys array
	for (int i = 0; i < MAX_KEYS; ++i)
		keys[i] = KEY_IDLE;

	//Init variables
	Player.Init(600, WINDOW_HEIGHT - 60, 65, 65, 5);
	idx_shot = 0;
	int w;
	SDL_QueryTexture(background, NULL, NULL, &w, NULL);
	scene.Init(0, 0, w, WINDOW_HEIGHT, 4);
	god_mode = false;	
	
	
	/*------------|--------|-----------------/
	/-------------|-IMAGES-|-----------------/
	/-------------|--------|----------------*/
	
	IMG_Init(IMG_INIT_PNG);
	// Cargar el archivo PNG como superficie
	SDL_Surface* spriteSurface = IMG_Load("assets/abuela3.png");
	
	// Crear una textura a partir de la superficie
	spriteTexture = SDL_CreateTextureFromSurface(Renderer, spriteSurface);
	// Liberar la superficie del sprite
	SDL_FreeSurface(spriteSurface);

	//DISPAROS
	SDL_Surface* shootSurface = IMG_Load("assets/aguacate.png");
	shooting = SDL_CreateTextureFromSurface(Renderer, shootSurface);
	SDL_FreeSurface(shootSurface);

	//BACKGROUNG
	SDL_Surface* backgSurface = IMG_Load("assets/background.png");
	background = SDL_CreateTextureFromSurface(Renderer, backgSurface);
	SDL_FreeSurface(backgSurface);

	return true;

}
void Game::Release()
{
	//Clean up all SDL initialized subsystems
	SDL_DestroyTexture(spriteTexture);
	SDL_DestroyTexture(shooting);
	SDL_DestroyTexture(background);
	IMG_Quit();
	SDL_Quit();

}
bool Game::Input()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)	return false;
	}

	SDL_PumpEvents();
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keyboard[i])
			keys[i] = (keys[i] == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;
		else
			keys[i] = (keys[i] == KEY_REPEAT || keys[i] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
	}

	return true;
	
}
bool Game::Update()
{
	//Read Input
	if (!Input())	return true;

	//Process Input
	int fx = 0, fy = 0;
	if (keys[SDL_SCANCODE_ESCAPE] == KEY_DOWN)	return true;
	if (keys[SDL_SCANCODE_F1] == KEY_DOWN)		god_mode = !god_mode;
	if (keys[SDL_SCANCODE_LEFT] == KEY_REPEAT) {
		if (Player.GetX() > 260)
		{
			fx = -1;
		}
	}
	if (keys[SDL_SCANCODE_RIGHT] == KEY_REPEAT) {
		if (Player.GetX() < 953)
		{
			fx = 1;
		}
	}
	if (keys[SDL_SCANCODE_SPACE] == KEY_DOWN)
	{
		int x, y, w, h;
		Player.GetRect(&x, &y, &w, &h);
		Shots[idx_shot].Init(x + w - 44, y + (h >> 1) - 33, 25, 25, -10);
		idx_shot++;
		idx_shot %= MAX_SHOTS;
		
	}

	//Player update
	Player.Move(fx, fy);
	//Shots update
	for (int i = 0; i < MAX_SHOTS; ++i)
	{
		if (Shots[i].IsAlive())
		{
			Shots[i].Move(0, 1);
			if (Shots[i].GetX() > WINDOW_WIDTH)	Shots[i].ShutDown();
		}
	}
		
	return false;
}
void Game::Draw()
{
	SDL_Rect rc;

	//Set the color used for drawing operations
	SDL_SetRenderDrawColor(Renderer, 193, 0, 0, 255);
	//Clear rendering target
	SDL_RenderClear(Renderer);

	if (god_mode) SDL_SetRenderDrawColor(Renderer, 192, 0, 0, 255);

	scene.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(Renderer, background, NULL, NULL);
	

	//Draw player
	Player.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(Renderer, spriteTexture, NULL,&rc);
	if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
	
	//Draw shots
	
	for (int i = 0; i < MAX_SHOTS; ++i)
	{
		if (Shots[i].IsAlive())
		{
			Shots[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
			SDL_RenderCopy(Renderer, shooting, NULL, &rc);
			if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
		}
	}

	Player.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(Renderer, spriteTexture, NULL, &rc);
	if (god_mode) SDL_RenderDrawRect(Renderer, &rc);

	//Update screen
	SDL_RenderPresent(Renderer);

	SDL_Delay(10);	// 1000/10 = 100 fps max
}