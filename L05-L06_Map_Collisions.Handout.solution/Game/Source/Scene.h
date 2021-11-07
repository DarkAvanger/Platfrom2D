#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool GodMode = false;

	bool Reset = false;

	struct player_struct
	{
		int x = 300;
		int y = 300;
		int velx = 0;
		int vely = 0;
		int y2 = 1;
	}player;

	bool jump = false;
	bool jumpFAll = false;
	bool doublejump = false;
	bool run = false;
	bool direction = true;


	bool LoadState(pugi::xml_node&);

	bool SaveState(pugi::xml_node&) const;

	int gravity = 0;
	int scene = 0;
	bool fall = true;
	int jumpA = 0;
	int auxY;
	int timer_salt = 0;
	iPoint position;
	iPoint velocity;
	iPoint directionP;

private:
	SDL_Texture* img;
	SDL_Texture* Owlet;

	SDL_Rect playerRect;
	SDL_Rect playerRect2;
	SDL_Rect playerRect3;
	SDL_Rect playerJumpR;
	SDL_Rect playerRunR;
	enum Animation { 
		IDLE, 
		JUMP, 
		RUN };
	Animation playerAnimation = IDLE;
};

#endif // __SCENE_H__