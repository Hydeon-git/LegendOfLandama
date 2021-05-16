#include "GuiCheckBox.h"
#include "App.h"
#include "Audio.h"
#include "Font.h"
#include "Window.h"
#include "Input.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::CHECKBOX, id)
{
	this->bounds = bounds;
	this->text = text;

	guiButtonFx = app->audio->LoadFx("Assets/Audio/Fx/gui_button_fx.wav");
	guiButtonMoveFx = app->audio->LoadFx("Assets/Audio/Fx/gui_button_move.wav");
}

GuiCheckBox::~GuiCheckBox()
{
}

bool GuiCheckBox::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		int mouseX, mouseY;
		app->input->GetMousePosition(mouseX, mouseY);

		/*mouseX += -app->render->camera.x / app->win->GetScale()*/;
		/*mouseY += -app->render->camera.y / app->win->GetScale()*/;

		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			state = GuiControlState::FOCUSED;
			if (!soundDone)
			{
				//app->audio->PlayFx(guiButtonMoveFx, 0);
				soundDone = true;
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) state = GuiControlState::PRESSED;

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				checked = !checked;
				//app->audio->PlayFx(guiButtonFx, 0);
				NotifyObserver();
			}
		}
		else
		{
			state = GuiControlState::NORMAL;
			soundDone = false;
		}
	}

	return false;
}

bool GuiCheckBox::Draw()
{
	app->render->DrawRectangle({ bounds.x - 2, bounds.y - 2,bounds.w + 4,bounds.h + 4}, 20, 20, 20);
	
	
	
	switch (state)
	{
	case GuiControlState::DISABLED:
	{
		if (checked) app->render->DrawRectangle(bounds, 100, 100, 100, 255);
		else app->render->DrawRectangle(bounds, 100, 100, 100, 255);
	} break;
	case GuiControlState::NORMAL: 
	{
		if (app->win->fullScreen && id == 1) app->render->DrawRectangle(bounds, 0, 255, 0, 255);
		else if (!app->win->fullScreen && id == 1) app->render->DrawRectangle(bounds, 255, 0, 0, 255);

		if (app->vSync && id == 2) app->render->DrawRectangle(bounds, 0, 255, 0, 255);
		else if (!app->vSync && id == 2) app->render->DrawRectangle(bounds, 255, 0, 0, 255);

	} break;
	case GuiControlState::FOCUSED: app->render->DrawRectangle(bounds, 200, 100, 220, 255);
		break;
	case GuiControlState::PRESSED: app->render->DrawRectangle(bounds, 250, 75, 150, 255);
		break;
	case GuiControlState::SELECTED: app->render->DrawRectangle(bounds, 0, 255, 0, 255);
		break;
	default:
		break;
	}
	app->font->DrawText(bounds.x - 120 + (app->render->camera.x) / app->win->GetScale(), bounds.y + 2 + (app->render->camera.y) / app->win->GetScale(), 0, text.GetString());

	if (app->scene->guiColliders && app->scene->pausedSettings)
	{
		if (app->win->fullScreen && id == 1) app->render->DrawRectangle({ bounds.x - 2, bounds.y - 2,bounds.w + 4,bounds.h + 4 }, 0, 155, 155, 150);
		else if (!app->win->fullScreen && id == 1) app->render->DrawRectangle({ bounds.x - 2, bounds.y - 2,bounds.w + 4,bounds.h + 4 }, 155, 155, 0, 150);

		if (app->vSync && id == 2) app->render->DrawRectangle({ bounds.x - 2, bounds.y - 2,bounds.w + 4,bounds.h + 4 }, 0, 155, 155, 150);
		else if (!app->vSync && id == 2) app->render->DrawRectangle({ bounds.x - 2, bounds.y - 2,bounds.w + 4,bounds.h + 4 }, 155, 155, 0, 150);
	}
	if (app->sceneIntro->guiColliders && !app->scene->paused)
	{
		if (app->win->fullScreen && id == 1) app->render->DrawRectangle({ bounds.x - 2, bounds.y - 2,bounds.w + 4,bounds.h + 4 }, 0, 155, 155, 150);
		else if (!app->win->fullScreen && id == 1) app->render->DrawRectangle({ bounds.x - 2, bounds.y - 2,bounds.w + 4,bounds.h + 4 }, 155, 155, 0, 150);

		if (app->vSync && id == 2) app->render->DrawRectangle({ bounds.x - 2, bounds.y - 2,bounds.w + 4,bounds.h + 4 }, 0, 155, 155, 150);
		else if (!app->vSync && id == 2) app->render->DrawRectangle({ bounds.x - 2, bounds.y - 2,bounds.w + 4,bounds.h + 4 }, 155, 155, 0, 150);
	}

	return false;
}
