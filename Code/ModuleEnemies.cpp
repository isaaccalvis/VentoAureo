#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleEnemies.h"
#include "ModuleParticles.h"
#include "ModuleTextures.h"
#include "Enemy.h"
#include "Enemy_LightShooter.h"
#include "Enemy_BasicTank.h"
#include "Enemy_BonusPlane.h"
#include "Enemy_Turret.h"
#include "Object_Box.h"
#include "Object_Medal.h"
#include "Object_PowerUp.h"
#include "Asteroid.h"
#include "DiamondPlane.h"
#include "HeavyOrangeAirship.h"
#include "HeavyTurret.h"
#include "LightShooterLateral.h"
#include "vTank.h"
#include "ModuleMusic.h"

#define SPAWN_MARGIN 100

ModuleEnemies::ModuleEnemies(){
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		enemies[i] = nullptr;
}

ModuleEnemies::~ModuleEnemies(){}

bool ModuleEnemies::Start(){
	// Enemies BackGround 1
	sprite_LightShooter = App->textures->Load("Sprites/Enemies/Stage_1/Light_Shooter.png");
	sprite_BasicTank = App->textures->Load("Sprites/Enemies/Stage_1/Tank.png");
	sprite_Turret = App->textures->Load("Sprites/Enemies/Stage_1/Turret.png");
	sprite_BonusPlane = App->textures->Load("Sprites/Enemies/Stage_1/Bonus_Airship.png");
	sprite_Box = App->textures->Load("Sprites/Extras/Box.png");
	sprite_Medal_PowerUp_Bonus = App->textures->Load("Sprites/Extras/Powerups_and_bonusues.png");

	// Enemies BackGrodun 6
	sprite_Asteroid = App->textures->Load("Sprites/Enemies/Stage_6/Asteroid.png");

	sprite_Light_Shooter_Lateral = App->textures->Load("Sprites/Enemies/Stage_6/Light_Shooter_Lateral.png");

	return true;
}

update_status ModuleEnemies::PreUpdate(){
	// check camera position to decide what to spawn
	for (uint i = 0; i < MAX_ENEMIES; ++i){
		if (queue[i].type != ENEMY_TYPES::NO_TYPE){
			if (queue[i].y * SCREEN_SIZE < App->render->camera.y + (App->render->camera.h * SCREEN_SIZE) + SPAWN_MARGIN){
				SpawnEnemy(queue[i]);
				queue[i].type = ENEMY_TYPES::NO_TYPE;
				LOG("Spawning enemy at %d", queue[i].y * SCREEN_SIZE);
			}
		}
	}
	return UPDATE_CONTINUE;
}

update_status ModuleEnemies::Update() {
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) enemies[i]->Move();
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) enemies[i]->DrawDown();
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) enemies[i]->DrawUp();
	return UPDATE_CONTINUE;
}

update_status ModuleEnemies::PostUpdate(){
	// check camera position to decide what to spawn
	for (uint i = 0; i < MAX_ENEMIES; ++i) {
		if (enemies[i] != nullptr) {
			if (enemies[i]->position.y * SCREEN_SIZE > (App->render->camera.y + App->render->camera.h + SPAWN_MARGIN)) {
				LOG("DeSpawning enemy at %d", enemies[i]->position.y * SCREEN_SIZE);
				delete enemies[i];
				enemies[i] = nullptr;
			}
			else if (enemies[i]->position.y * SCREEN_SIZE < (App->render->camera.y - SPAWN_MARGIN)) {
				LOG("DeSpawning enemy at %d", enemies[i]->position.y * SCREEN_SIZE);
				delete enemies[i];
				enemies[i] = nullptr;
			}
		}
	}
	return UPDATE_CONTINUE;
}

bool ModuleEnemies::CleanUp(){
	App->textures->Unload(sprite_LightShooter);
	App->textures->Unload(sprite_BonusPlane);
	App->textures->Unload(sprite_Turret);
	App->textures->Unload(sprite_BasicTank);
	App->textures->Unload(sprite_Box);
	App->textures->Unload(sprite_Medal_PowerUp_Bonus);
	App->textures->Unload(sprite_Asteroid);
	App->textures->Unload(sprite_Diamond_Plane);
	App->textures->Unload(sprite_Heavy_Orange_Airship);
	App->textures->Unload(sprite_Heavy_Turret);
	App->textures->Unload(sprite_Light_Shooter_Lateral);
	App->textures->Unload(sprite_vTank);

	for (uint i = 0; i < MAX_ENEMIES; ++i){
		if (enemies[i] != nullptr){
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}
	return true;
}

bool ModuleEnemies::AddEnemy(ENEMY_TYPES type, int x, int y){
	bool ret = false;

	for (uint i = 0; i < MAX_ENEMIES; ++i){
		if (queue[i].type == ENEMY_TYPES::NO_TYPE){
			queue[i].type = type;
			queue[i].x = x;
			queue[i].y = y;
			ret = true;
			break;
		}
	}
	return ret;
}

void ModuleEnemies::SpawnEnemy(const EnemyInfo& info){
	uint i = 0;
	for (; enemies[i] != nullptr && i < MAX_ENEMIES; ++i);
	if (i != MAX_ENEMIES){
		switch (info.type){
		case ENEMY_TYPES::LIGHT_SHOOTER:
			enemies[i] = new Enemy_LightShooter(info.x, info.y);
			break;
		case ENEMY_TYPES::BASIC_TANK:
			enemies[i] = new Enemy_BasicTank(info.x, info.y);
			break;
		case ENEMY_TYPES::BONUS_PLANE:
			enemies[i] = new Enemy_BonusPlane(info.x, info.y);
			break;
		case ENEMY_TYPES::TURRET:
			enemies[i] = new Enemy_Turret(info.x, info.y);
			break;
		case ENEMY_TYPES::BOX:
			enemies[i] = new Object_Box (info.x, info.y);
			break;
		case ENEMY_TYPES::MEDAL:
			enemies[i] = new Object_Medal(info.x, info.y);
			break;
		case ENEMY_TYPES::POWER_UP:
			enemies[i] = new Object_PowerUp(info.x, info.y);
			break;
		case ENEMY_TYPES::ASTEROID:
			enemies[i] = new Asteroid(info.x, info.y);
			break;
		case ENEMY_TYPES::DIAMOND_PLANE:
			enemies[i] = new DiamondPlane(info.x, info.y);
			break;
		case ENEMY_TYPES::HEAVY_ORANGE_AIRSHIP:
			enemies[i] = new HeavyOrangeAirship(info.x, info.y);
			break;
		case ENEMY_TYPES::HEAVY_TURRET:
			enemies[i] = new HeavyTurret(info.x, info.y);
			break;
		case ENEMY_TYPES::LIGHT_SHOOTER_LATERAL:
			enemies[i] = new LightShooterLateral(info.x, info.y);
			break;
		case ENEMY_TYPES::VTANK:
			enemies[i] = new vTank(info.x, info.y);
			break;
		}
	}
}

void ModuleEnemies::OnCollision(Collider* c1, Collider* c2){
	for (uint i = 0; i < MAX_ENEMIES; ++i){
		if (enemies[i] != nullptr && enemies[i]->GetCollider() == c1){
			enemies[i]->OnCollision(c2);
			if (enemies[i]->vida <= 0) {
				delete enemies[i];
				enemies[i] = nullptr;
			}
			break;
		}
	}
}