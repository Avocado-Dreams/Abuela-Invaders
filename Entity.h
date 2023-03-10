#pragma once
class Entity
{
public:
	Entity();
	~Entity();

	void Init(int posx, int posy, int w, int h, int s);
	void GetRect(int *posx, int *posy, int *w, int *h);
	void SetX(int posx);
	int  GetX();
	int GetY();
	int GetWidth();
	int GetHeight();
	void ShutDown(); 
	bool IsAlive();
	void Move(int dx, int dy);
	void EnemyReciveDamage(int damage, Entity &enemy);
	void PlayerReciveDamage(int damage, Entity& player);
	
	int damage = 1;
	

private:
	int enemyLife = 1;
	int abuelaLife = 5;
	int x, y;
	int width, height;
	int speed;
	bool is_alive;
	
};



