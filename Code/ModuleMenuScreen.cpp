#include "Globals.h"
#include "Application.h"
#include "ModuleMenuScreen.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModulePlayer2.h"
#include "ModuleFadeToBlack.h"
#include "ModuleBackground.h"
#include "ModuleBullets.h"
#include "ModulePlayer.h"
#include "ModuleMusic.h"
#include "Animation.h"
#include "ModuleEnemies.h"
#include "ModuleFonts.h"
#include "SDL\include\SDL_timer.h"

ModuleMenuScreen::ModuleMenuScreen() {

	MenuScreenRect.x = 0;
	MenuScreenRect.y = 0;
	MenuScreenRect.w = 223;
	MenuScreenRect.h = 255;

	MainMenu.PushBack({ 0,0,223,255 });
	MainMenu.speed = 0.1f;
	MainMenu.loop = false;

	GameOver.PushBack({ 0,0,223,255 });

	Transition.PushBack({ 0,0,223,255 });
	Transition.PushBack({ 225,0,223,255 });
	Transition.PushBack({ 450,0,223,255 });
	Transition.PushBack({ 675,0,223,255 });
	Transition.PushBack({ 0,257,223,255 });
	Transition.PushBack({ 225,257,223,255 });
	Transition.PushBack({ 450,257,223,255 });
	Transition.PushBack({ 675,257,223,255 });
	Transition.PushBack({ 450,257,223,255 });
	Transition.PushBack({ 225,257,223,255 });
	Transition.PushBack({ 0,257,223,255 });
	Transition.PushBack({ 675,0,223,255 });
	Transition.PushBack({ 450,0,223,255 });
	Transition.PushBack({ 225,0,223,255 });
	Transition.PushBack({ 0,0,223,255 });
	Transition.speed = 0.25f;
	Transition.loop = false;

}
ModuleMenuScreen::~ModuleMenuScreen() {}

bool ModuleMenuScreen::Start() {
	raiden_sprite = App->textures->Load("Sprites/MenuImages/Raiden_Screen.png");
	loading_sprite = App->textures->Load("Sprites/MenuImages/Loading_Screen.png");
	player_sprite = App->textures->Load("Sprites/MenuImages/Players_Screen.png");
	gameOver_sprite = App->textures->Load("Sprites/MenuImages/Texture_Game_Over.png");
	font_score = App->fonts->Load("fonts/raiden_font.png", "! @,-./0123456789$;>&?abcdefghijklmnopqrstuvwxyz", 1);
	Menu_Actual_Fase = 0;
	selectorScreen(Start_Screen);
	return true;
}

bool ModuleMenuScreen::selectorScreen(MenuScreenNames name) {
	bool ret = true;
	switch (name) {
	case Start_Screen:
		current_time = SDL_GetTicks() + 1000;
		App->music->Disable();
		MenuScreenTexture = loading_sprite;
		current_animation = &Transition;
		break;

	case Players_Screen:
		App->music->Disable();
		MenuScreenTexture = player_sprite;
		current_animation = &MainMenu;
		break;

	case Game_Over_Screen:
		App->music->LoadMusic("Audio/Music/Game_Over.ogg");
		MenuScreenTexture = gameOver_sprite;
		current_animation = &GameOver;
		if (highscore == 0)
			highscore = 1;
		else if (highscore == -1) // player 2
			highscore = 2;
		Menu_Actual_Fase = 10;
		break;
	}

	return ret;
}

update_status ModuleMenuScreen::Update() {
	App->render->CleanRender();

	App->render->Blit(MenuScreenTexture, 59, 0, &MenuScreenRect);
	MenuScreenRect = current_animation->GetCurrentFrame();

	switch (Menu_Actual_Fase) {
	case 0:
		App->background->Disable();
		App->player->Disable();
		App->player2->Disable();
		App->enemies->Disable();
		App->bullet->Disable();
		App->collision->Disable();

		if (SDL_TICKS_PASSED(SDL_GetTicks(), current_time)) {
			App->enemies->Enable();
			MenuScreenTexture = raiden_sprite;
			current_animation = &MainMenu;
			Menu_Actual_Fase = 1;
		}
		break;
	case 1:
		if (App->input->keyboard[SDL_SCANCODE_5] == KEY_STATE::KEY_DOWN || App->input->buttonStart == KEY_STATE::KEY_DOWN || App->input->buttonStart2 == KEY_STATE::KEY_DOWN) {
			App->menuScreen->selectorScreen(Players_Screen);
			Menu_Actual_Fase = 2;
		}
		break;
	case 2:
		if (App->input->keyboard[SDL_SCANCODE_1] == KEY_STATE::KEY_DOWN || App->input->buttonA == KEY_STATE::KEY_DOWN) {
			current_time = SDL_GetTicks() + 1000;
			MenuScreenTexture = loading_sprite;
			current_animation = &Transition;
			current_animation->Reset();
			Menu_Actual_Fase = 3;
		}
		else if (App->input->keyboard[SDL_SCANCODE_2] == KEY_STATE::KEY_DOWN || App->input->buttonA2 == KEY_STATE::KEY_DOWN) {
			current_time = SDL_GetTicks() + 1000;
			MenuScreenTexture = loading_sprite;
			current_animation = &Transition;
			current_animation->Reset();
			Menu_Actual_Fase = 4;
		}
		break;
	case 3:
		if (SDL_TICKS_PASSED(SDL_GetTicks(), current_time)) {
			App->fade->FadeToBlack(this, App->background); // Li treiem el temps al fade to black
			App->player2->jugador2Activat = false;
			Menu_Actual_Fase = 5;
		}
		break;
	case 4:
		if (SDL_TICKS_PASSED(SDL_GetTicks(), current_time)) {
			App->fade->FadeToBlack(this, App->background); // Li treiem el temps al fade to black
			App->player2->Enable();
			App->player2->jugador2Activat = true;
			Menu_Actual_Fase = 5;
		}
		break;
	case 5:
		break;
	case 10:// GAME OVER
		if (App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN) {
			App->fonts->UnLoad(font_score);
			App->textures->Unload(gameOver_sprite);
			Menu_Actual_Fase = 0;
			break;
		}
		if (highscore == 1) {
			App->fonts->BlitText(110, 30, font_score, "player 1 score");
			char str[10];
			sprintf_s(str, "%i", App->player->puntuacioP1);
			App->fonts->BlitText(160, 45, font_score, str);
			sprintf_s(score_text, 10, "%7d", App->player->puntuacioP1);
			App->fonts->BlitText(65, 45, font_score, "high score!");
		}
		if (highscore == 2) {
			App->fonts->BlitText(110, 30, font_score, "player 1 score");
			App->fonts->BlitText(110, 85, font_score, "player 2 score");
			char str[10];
			sprintf_s(str, "%i", App->player->puntuacioP1);
			App->fonts->BlitText(160, 45, font_score, str);
			sprintf_s(score_text, 10, "%7d", App->player->puntuacioP1);
			sprintf_s(str, "%i", App->player2->puntuacioP2);
			App->fonts->BlitText(160, 100, font_score, str);
			sprintf_s(score_text, 10, "%7d", App->player2->puntuacioP2);
			if (App->player->puntuacioP1 > App->player2->puntuacioP2) {
				App->fonts->BlitText(65, 45, font_score, "high score!");
			}
			if (App->player->puntuacioP1 < App->player2->puntuacioP2) {
				App->fonts->BlitText(65, 100, font_score, "high score!");
			}
		}
	}
	return UPDATE_CONTINUE;
}

bool ModuleMenuScreen::CridaMenu() {
	selectorScreen(Game_Over_Screen);
	Menu_Actual_Fase = 10;
	return true;
}

bool ModuleMenuScreen::CleanUp() {
	bool ret = true;
	App->textures->Unload(MenuScreenTexture);
	App->textures->Unload(raiden_sprite);
	App->textures->Unload(loading_sprite);
	App->textures->Unload(player_sprite);
	App->textures->Unload(gameOver_sprite);
	App->fonts->UnLoad(font_score);
	App->menuScreen->Disable();
	return ret;
}