#include "GuiButton.h"
#include "App.h"
#include "Audio.h"
#include "Font.h"
#include "Window.h"
#include "Input.h"
#include "SceneWin.h"
#include "Textures.h"
#include "DialogSystem.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	guiButtonFx = app->audio->LoadFx("Assets/Audio/Fx/gui_button_fx.wav");
	guiButtonErrorFx = app->audio->LoadFx("Assets/Audio/Fx/gui_button_error.wav");
	guiButtonMoveFx = app->audio->LoadFx("Assets/Audio/Fx/gui_button_move.wav");
	//pointerText = app->tex->Load("Assets/Textures/pointer.png");
}

GuiButton::~GuiButton()
{
}

bool GuiButton::Update(float dt)
{

	if (state != GuiControlState::DISABLED)
	{
		int mouseX, mouseY;
		app->input->GetMousePosition(mouseX, mouseY);

		mouseX += -app->render->camera.x / app->win->GetScale();
		mouseY += -app->render->camera.y / app->win->GetScale();

		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			state = GuiControlState::FOCUSED;
			if (!soundDone)
			{
				app->audio->PlayFx(guiButtonMoveFx, 0);
				soundDone = true;
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) state = GuiControlState::PRESSED;

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				app->audio->PlayFx(guiButtonFx, 0);
				NotifyObserver();
			}
		}

		else
		{
			state = GuiControlState::NORMAL;
			soundDone = false;
		}

	}
	else if (state == GuiControlState::DISABLED)
	{
		int mouseX, mouseY;
		app->input->GetMousePosition(mouseX, mouseY);

		mouseX += -app->render->camera.x / app->win->GetScale();
		mouseY += -app->render->camera.y / app->win->GetScale();

		// Check collision between mouse and button bounds
		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) app->audio->PlayFx(guiButtonErrorFx, 0);
		}
	}

	return false;
}

bool GuiButton::Draw()
{
	//if (!app->scene->paused) app->render->DrawRectangle({ bounds.x - 2, bounds.y - 2,bounds.w + 4,bounds.h + 4 }, 20, 20, 20);
	/*if (app->scene->paused) app->render->DrawRectangle({ bounds.x - 4, bounds.y - 4,bounds.w + 8,bounds.h + 8 }, 20, 20, 20);
	else if (app->scene->pausedSettings) app->render->DrawRectangle({ bounds.x - 4, bounds.y - 4,bounds.w + 8,bounds.h + 8 }, 20, 20, 20);*/

	switch (state)
	{
	case GuiControlState::DISABLED: app->render->DrawRectangle(bounds, 211, 211, 211, 255);
		break;
	case GuiControlState::NORMAL: //app->render->DrawRectangle(bounds, 255, 255, 255, 255);
		break;
	case GuiControlState::FOCUSED: app->render->DrawTexture(app->dialogueSystem->pointerText, bounds.x - 20, bounds.y-2);
		break;
	case GuiControlState::PRESSED: 
		app->render->DrawRectangle(bounds, 123, 104, 238, 255);
		app->render->DrawTexture(app->dialogueSystem->pointerText, bounds.x - 20, bounds.y - 2);
		break;
	case GuiControlState::SELECTED: app->render->DrawRectangle(bounds, 0, 255, 0, 255);
		break;
	default:
		break;
	}

	app->font->DrawText(bounds.x - 1 + (app->render->camera.x) / app->win->GetScale(), bounds.y + (app->render->camera.y) / app->win->GetScale(), 0, text.GetString());

	//app->font->DrawText(bounds.x / app->win->GetScale(), bounds.y / app->win->GetScale(), 0, text.GetString());


	if (app->scene->guiColliders && app->scene->paused) app->render->DrawRectangle({ bounds.x - 4, bounds.y - 4,bounds.w + 8,bounds.h + 8 }, 0, 0, 255, 100);

	if (app->sceneIntro->guiColliders && !app->scene->paused && !app->sceneLose->guiColliders && !app->sceneWin->guiColliders)
	{
		if (this->state == GuiControlState::DISABLED) app->render->DrawRectangle({ bounds.x - 2, bounds.y - 2,bounds.w + 4,bounds.h + 4 }, 0, 0, 0, 100);
		else app->render->DrawRectangle({ bounds.x - 2, bounds.y - 2,bounds.w + 4,bounds.h + 4 }, 0, 0, 255, 100);
	}

	if (app->sceneLose->guiColliders && !app->sceneIntro->guiColliders && !app->sceneWin->guiColliders && !app->scene->paused) app->render->DrawRectangle({ bounds.x - 2, bounds.y - 2,bounds.w + 4,bounds.h + 4 }, 0, 0, 255, 100);

	if (app->sceneWin->guiColliders && !app->sceneLose->guiColliders && !app->sceneIntro->guiColliders && !app->scene->paused) app->render->DrawRectangle({ bounds.x - 2, bounds.y - 2,bounds.w + 4,bounds.h + 4 }, 0, 0, 255, 100);

	return false;
}


//bool GuiButton::CleanUp()
//{
//	//LOG("Freeing scene");
//	bool ret = true;
//	
//	ret = app->tex->UnLoad(pointerText);
//	pointerText = nullptr;
//
//	return ret;
//}
