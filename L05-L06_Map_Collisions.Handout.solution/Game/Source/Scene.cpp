#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Timer.h"
#include"PerfTimer.h"


#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// L03: DONE: Load map
	//app->map->Load("hello.tmx");
	app->map->Load("level_01.tmx");

	// Sprites
	/*BG1 = app->tex->Load("Assets/.png");*/
	Owlet = app->tex->Load("Assets\\maps\\Owlet.png");
	
	// Load music
	app->render->camera.x = 0; //Need Position
	app->render->camera.y = -200;

	playerRect = { 0, 0, 55, 55 };
	playerJumpR = { 0, 0, 55, 55 };
	playerRunR = { 0, 0, 55, 55 };

	direction = true;

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	
	app->frame_cap = 68; //Fix frames to 60 average;

	app->capped_ms = 1000 / app->frame_cap;
	// Save/Load
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	// God Mode
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {

		GodMode = !GodMode;
	}
	if (GodMode) {

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			player.x += -4;
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			player.x += 4;
		}

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			player.y += -4;
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			player.y += 4;
		}

		//Camera not working yet
		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
			app->render->camera.y -= 1;
		}
		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
			app->render->camera.y += 1;
		}
		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
			app->render->camera.x -= 4;

		}
		if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
			app->render->camera.x += 4;

		}

	}
	else {

		//Player WASD
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			player.velx = -4;
			direction = false;
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			player.velx = 4;
			direction = true;
		}

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			run = true;
		}
		else {
			run = false;
		}

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !jump && !GodMode) {

			jump = true;
			fall = false;
			player.vely = -4;
			playerAnimation = JUMP;
		}
		if (jump = true && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			jump = false;
			fall = false;
			player.vely = -4;
			playerAnimation = JUMP;
		}



		if (!jump && !run) {
			playerAnimation = IDLE;
		}
		else if (run && !jump) {
			if (scene % 40 == 0) {
				if (playerAnimation != RUN) {
					playerAnimation = RUN;
				}
				else {
					playerAnimation = IDLE;
				}
			}
		}
		//Movement
		player.x += player.velx;
		player.y += player.vely;
		player.velx = 0;

		// Should work need collisions for full test
		if (gravity == 0) {
			player.vely += player.y2;
			gravity = 7;
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
				gravity = 20;
			}
		}
		else {
			gravity--;
		}

		// Collisions need it
	}
	// Draw map
	app->map->Draw();

	// Png posx posy
	if (direction) {
		playerRect.y = 90;
		playerRect.x = 150;

		playerJumpR.y = 18;
		playerJumpR.x = 0;

		playerRunR.y = 90;
		playerRunR.x = 220;
	}
	else {
		playerRect.y = 225;
		playerRect.x = 150;

		playerJumpR.y = 225;
		playerJumpR.x = 75;

		playerRunR.y = 225;
		playerRunR.x = 220;
	}

	switch (playerAnimation) {
	case IDLE:
		app->render->DrawTexture(Owlet, player.x, player.y, &playerRect, 1, 5);
		break;
	case JUMP:
		app->render->DrawTexture(Owlet, player.x, player.y, &playerJumpR, 1, 5);
		break;
	case RUN:
		app->render->DrawTexture(Owlet, player.x, player.y, &playerRunR, 1, 5);
		break;
	}

	if (scene <= 0) {
		scene = 200;
	}
	else {
		scene--;
	}

	// L03: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width, app->map->mapData.height,
		app->map->mapData.tileWidth, app->map->mapData.tileHeight,
		app->map->mapData.tilesets.count());

	app->win->SetTitle(title.GetString());
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool Scene::LoadState(pugi::xml_node& configRenderer)
{

	player.x = configRenderer.child("player").attribute("x").as_int();
	player.y = configRenderer.child("player").attribute("y").as_int();

	return true;
}
bool Scene::SaveState(pugi::xml_node& configRenderer) const
{
	pugi::xml_node player1 = configRenderer.child("player");

	player1.attribute("x").set_value(player.x);
	player1.attribute("y").set_value(player.y);

	return true;
}