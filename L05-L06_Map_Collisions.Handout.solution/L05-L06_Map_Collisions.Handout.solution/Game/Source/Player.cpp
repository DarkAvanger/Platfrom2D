#include "Player.h"
#include "Render.h"
#include "Input.h"
#include "Scene.h"
#include "Collider.h"
#include "Map.h"
#include "App.h"
#include "Textures.h"


Player::Player() {
	name.Create("player");
	//pugi::xml_parse_result result = PlayerDocument.load_file("PlayerSettings.xml");

	//if (result == NULL) {
	//	LOG("The xml file containing the player tileset fails. Pugi error: %s", result.description());
//	}
	
	//Loading XML File
	AnimationNode = PlayerDocument.child("config").child("AnimationCoords").child("Idle");
	LoadPushbacks(AnimationNode, Idle);
	AnimationNode = PlayerDocument.child("config").child("AnimationCoords").child("Run");
	LoadPushbacks(AnimationNode, Run);
	AnimationNode = PlayerDocument.child("config").child("AnimationCoords").child("Jump");
	LoadPushbacks(AnimationNode, Jump);
	PlayerSettings = PlayerDocument.child("config");
	
}

Player::~Player() {}


bool Player::CleanUp() {

	return true;
}

bool Player::Awake() {

	LOG("Player Module Loading");
	bool ret = true;
	return ret;
}

bool Player::Start() {
	//Load texture
	SString tmp = ("Assets\\maps\\Owlet_Monster.png"); //Should be correct path now
	PlayerTexture = app->tex->Load(tmp.GetString()); 

	if (app->scene->currentLevel == LEVEL1) {
		position.x = PlayerSettings.child("PlayerSettings").child("StartingPose").child("Level1").attribute("position.x").as_int();
		position.y = PlayerSettings.child("PlayerSettings").child("StartingPose").child("Level1").attribute("position.y").as_int();
	}

	velocity.x = 5;
	velocity.y = 5;
	fall = true;
	direction.x = 1;
	direction.y = -1;

	PlayerP.x = position.x;
	PlayerP.y = position.y;
	PlayerP.w = PlayerSettings.child("image").attribute("w").as_int();
	PlayerP.h = PlayerSettings.child("image").attribute("w").as_int();

	//SaveGame need it
	//app->SaveGame("save_game.xml");
	return true;
}

bool Player::PreUpdate() {

	return true;
}

bool Player::Update(float dt) {

	currentAnimation = &Idle;
	// X move
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		velocity.x = 5;
		direction.x = 1;
		position.x += velocity.x;
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		velocity.x = 5;
		direction.x = -1;
		position.x -= velocity.x;
	}

	// Y move
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		jump = true;
		jumpfall = false;
		velocity.y = 5;
	}

	// God mode

	//Jump
	if (jump) {

		if (velocity.y >= 0 && !jumpfall) {

			fall = false;
			position.y -= velocity.y;
			velocity.y -= 0.2;
			direction.y = 1;
		}

		else if (velocity.y < 0) {

			jumpfall = true;
			fall = true;
		}

	}
	//Fall
	if (fall) {

		direction.y = -1;

		if (velocity.y < 8)
			velocity.y += 0.2;

		position.y += velocity.y;
	}
	//Player Blit
	if (direction.x == 1 || direction.x == 0) {
		//app->render->DrawTexture(PlayerTexture, position.x, position.y, &(currentAnimation->GetCurrentFrame()), 1, 0, 0, 0, SDL_FLIP_HORIZONTAL);
	}
	if (direction.x == -1) {
		//app->render->DrawTexture(PlayerTexture, position.x, position.y, &(currentAnimation->GetCurrentFrame())); //Not Drawing character
	}
	return true;
}


bool Player::PostUpdate() {

	return true;
}

bool Player::Load(pugi::xml_node& data)
{
	position.x = data.child("player").attribute("x").as_int();
	position.y = data.child("player").attribute("y").as_int();
	return true;
}

bool Player::Save(pugi::xml_node& data) const
{

	return true;
}
/*
void Player::Collision() {
}
*/
void Player::LoadPushbacks(pugi::xml_node node, Animation& animation) {

	animation.loop = node.attribute("loop").as_bool();
	animation.speed = node.attribute("speed").as_float();
	SDL_Rect rect;

	for (node = node.child("Pushback"); node; node = node.next_sibling("Pushback")) {

		rect.x = node.attribute("x").as_int();
		rect.y = node.attribute("y").as_int();
		rect.w = node.attribute("w").as_int();
		rect.h = node.attribute("h").as_int();
		animation.PushBack({ rect });
	}
}