#include "Game.h"
#include <math.h>
#include <random>

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

	std::vector<Entity> enemigos;

	int valor = 500;
	for (int i = 0; i < 10; i++)
	{
		Enemy[i].Init(valor, 50, 32, 32, 2);
		valor += 50;
	}
	valor = 500;
	for (int i = 10; i < 20; i++)
	{
		Enemy[i].Init(valor, 100, 32, 32, 2);
		valor += 50;
	}
	valor = 500;
	for (int i = 20; i < 30; i++)
	{
		Enemy[i].Init(valor, 150, 32, 32, 2);
		valor += 50;
	}
	valor = 500;
	for (int i = 30; i < 40; i++)
	{
		Enemy[i].Init(valor, 200, 32, 32, 2);
		valor += 50;
	}
		
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

	//ROBOT
	SDL_Surface* robotSurface = IMG_Load("assets/robot.png");
	robot = SDL_CreateTextureFromSurface(Renderer, robotSurface);
	SDL_FreeSurface(robotSurface);

	//LASER
	SDL_Surface* laserSurface = IMG_Load("assets/laserRobot.png");
	laser = SDL_CreateTextureFromSurface(Renderer, laserSurface);
	SDL_FreeSurface(laserSurface);

	return true;

}

void Game::Release()
{
	//Clean up all SDL initialized subsystems
	SDL_DestroyTexture(spriteTexture);
	SDL_DestroyTexture(shooting);
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(robot);
	SDL_DestroyTexture(laser);
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
	//COLISION BALAS/ENEMIGOS
	for (int i = 0; i < MAX_SHOTS; i++)
	{
		int x, y, w, h;
		SDL_Rect shotRect = {Shots[i].GetX(),Shots[i].GetY(),Shots[i].GetWidth(),Shots[i].GetHeight()};
		SDL_Rect playerRect = { Player.GetX(),Player.GetY(),Player.GetWidth(),Player.GetHeight() };
		SDL_Rect enemyShotRect = { enemyShots[i].GetX(),enemyShots[i].GetY(),enemyShots[i].GetWidth(),enemyShots[i].GetHeight() };
		
		for (int j = 0; j < MAX_ENEMIES; j++)
		{
			SDL_Rect enemyRect = { Enemy[j].GetX(),Enemy[j].GetY(),Enemy[j].GetWidth(),Enemy[j].GetHeight() };
			if (SDL_HasIntersection(&enemyRect, &shotRect))
			{
				Enemy[j].EnemyReciveDamage(Shots[i].damage, Enemy[j]);
				Shots[i].ShutDown();
			}
		}
		if (SDL_HasIntersection(&playerRect, &enemyShotRect))
		{
			Player.PlayerReciveDamage(enemyShots[i].damage, Player);
			enemyShots[i].ShutDown();
		}

		//ELIMINAR ABUELA
		if (Player.IsAlive()==false)
		{			
			SDL_DestroyTexture(spriteTexture);	
		}
		
		//ELIMINAR ENEMIGOS
		int index = -1; // Variable para almacenar el índice del objeto a eliminar
		for (int i = 0; i < MAX_ENEMIES; i++) {
			if (!Enemy[i].IsAlive()) {
				index = i; // Guardar el índice del objeto a eliminar
				break;
			}
		}
		if (index != -1) { // Si se encontró un objeto para eliminar
			// Mover los elementos posteriores una posición hacia atrás
			for (int i = index; i < MAX_ENEMIES-1; i++) {
				Enemy[i] = Enemy[i + 1];
			}
			// El último elemento debe ser eliminado o reinicializado para evitar duplicados
			Enemy[MAX_ENEMIES-1] = Entity();
		}

		//ELIMINAR BALA
		index = -1;
		for (int i = 0; i < MAX_SHOTS; i++) {
			if (!Shots[i].IsAlive()) {
				index = i; // Guardar el índice del objeto a eliminar
				break;
			}
		}
		if (index != -1) { // Si se encontró un objeto para eliminar
			// Mover los elementos posteriores una posición hacia atrás
			for (int i = index; i < MAX_SHOTS-1; i++) {
				Shots[i] = Shots[i + 1];
			}
			// El último elemento debe ser eliminado o reinicializado para evitar duplicados
			Shots[MAX_SHOTS -1] = Entity();
		}

		//ELIMINAR BALA ENEMIGA
		index = -1;
		for (int i = 0; i < MAX_SHOTS; i++) {
			if (!enemyShots[i].IsAlive()) {
				index = i; // Guardar el índice del objeto a eliminar
				break;
			}
		}
		if (index != -1) { // Si se encontró un objeto para eliminar
			// Mover los elementos posteriores una posición hacia atrás
			for (int i = index; i < MAX_SHOTS - 1; i++) {
				enemyShots[i] = enemyShots[i + 1];
			}
			// El último elemento debe ser eliminado o reinicializado para evitar duplicados
			enemyShots[MAX_SHOTS - 1] = Entity();
		}
	}
		
	//Enemy move
	
	for (int i = 0; i < 40; i++) {
		Enemy[i].Move(fxRob, 0);
		if (fxRob == 1 && Enemy[i].GetX() == 1000)
		{
			fxRob = -1;
		}
		else if (fxRob == -1 && Enemy[i].GetX() == 230)
		{
			fxRob = 1;
		}
		if (rand() % 5000 == 0)
		{
			int x, y, w, h;
			Enemy[i].GetRect(&x, &y, &w, &h);
			enemyShots[idx_shot].Init(x + w - 44, y + (h >> 1) - 33, 25, 25, -10);
			idx_shot++;
			idx_shot %= MAX_SHOTS;
		}
	}

	for (int i = 0; i < MAX_SHOTS; ++i)
	{
		if (enemyShots[i].IsAlive())
		{
			enemyShots[i].Move(0, -1);
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

	for (int i = 0; i < MAX_ENEMIES; i++) {
		Enemy[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
		SDL_RenderCopy(Renderer, robot, NULL, &rc);
		if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
	}
	for (int i = 0; i < MAX_SHOTS; i++)
	{
		if (enemyShots[i].IsAlive())
		{
			enemyShots[i].GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
			SDL_RenderCopy(Renderer,laser , NULL, &rc);
			if (god_mode) SDL_RenderDrawRect(Renderer, &rc);
		}
	}
	

	//Update screen
	SDL_RenderPresent(Renderer);

	SDL_Delay(10);	// 1000/10 = 100 fps max
}