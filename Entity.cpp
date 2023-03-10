#include "Entity.h"


Entity::Entity()
{
	is_alive = false;
}
Entity::~Entity()
{
}
void Entity::Init(int posx, int posy, int w, int h, int s) 
{
	x = posx;
	y = posy;
	width = w;
	height = h;
	speed = s;
	is_alive = true;
}
void Entity::GetRect(int *posx, int *posy, int *w, int *h)
{
	*posx = x;
	*posy = y;
	*w = width;
	*h = height;
}
int Entity::GetX()
{
	return x;
}
int Entity::GetY()
{
	return y;
}
int Entity::GetWidth()
{
	return width;
}
int Entity::GetHeight()
{
	return height;
}
void Entity::SetX(int posx)
{
	x = posx;
}
void Entity::ShutDown()
{
	is_alive = false;
}
bool Entity::IsAlive()
{
	return is_alive;
}

void Entity::Move(int dx, int dy)
{
	x += dx * speed;
	y += dy * speed;
}
void Entity::EnemyReciveDamage(int damage, Entity &enemy)
{
	enemy.enemyLife -= damage;
	if (enemyLife <= 0)
	{
		enemy.ShutDown();
	}	
}
void Entity::PlayerReciveDamage(int damag, Entity& player)
{
	player.abuelaLife -= damag;
	if (abuelaLife <= 0)
	{
		player.ShutDown();
	}
}