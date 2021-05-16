#include "GuiSlider.h"
#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Font.h"
#include "Window.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
	this->bounds = bounds;
	this->boundsSlider = this->bounds;
	this->text = text;
	this->minValue = boundsSlider.x - 16;
	this->maxValue = this->minValue + 96;
	ChangeValue();

	guiButtonFx = app->audio->LoadFx("Assets/Audio/Fx/gui_button_fx.wav");
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(float dt)
{
	this->minValue = bounds.x - (this->value);
	this->maxValue = bounds.x + ((93 - this->value));

	if (state != GuiControlState::DISABLED)
	{
		int mouseX, mouseY;
		app->input->GetMousePosition(mouseX, mouseY);

		mouseX += -app->render->camera.x / app->win->GetScale();
		mouseY += -app->render->camera.y / app->win->GetScale();

		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			state = GuiControlState::FOCUSED;

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) state = GuiControlState::PRESSED;
		}
		else if (state == GuiControlState::PRESSED)
		{
			if ((app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) && (mouseX > minValue) && (mouseX < maxValue))
			{
				bounds.x = mouseX - (bounds.w / 2) / app->win->GetScale();
				ChangeValue();
				NotifyObserver();
				if(id==2) app->audio->PlayFx(guiButtonFx, 0);
			}
			
			else state = GuiControlState::NORMAL;
		}
		else state = GuiControlState::NORMAL;
	}

	return false;
}

bool GuiSlider::Draw()
{
	SDL_Rect rect = { boundsSlider.x - 16, boundsSlider.y - 1, 100, 10 };
	app->render->DrawRectangle({ rect.x - 2, rect.y - 2,rect.w + 4,rect.h + 4 }, 20, 20, 20);
	app->render->DrawRectangle(rect, 0, 220, 120, 255);

	
	switch (state)
	{
	case GuiControlState::DISABLED: app->render->DrawRectangle(bounds, 100, 100, 100, 255);
		break;
	case GuiControlState::NORMAL: app->render->DrawRectangle(bounds, 140, 75, 185, 255);
		break;
	case GuiControlState::FOCUSED: app->render->DrawRectangle(bounds, 200, 100, 220, 255);
		break;
	case GuiControlState::PRESSED: app->render->DrawRectangle(bounds, 250, 75, 150, 255);
		break;
	case GuiControlState::SELECTED: app->render->DrawRectangle(bounds, 0, 255, 0, 255);
		break;
	default:
		break;
	}

	app->font->DrawText(boundsSlider.x - 135 + (app->render->camera.x) / app->win->GetScale(), boundsSlider.y + 2 + (app->render->camera.y) / app->win->GetScale(), 0, text.GetString());

	if (app->scene->guiColliders && app->scene->pausedSettings)
	{
		app->render->DrawRectangle({ rect.x - 2, rect.y - 2,rect.w + 4,rect.h + 4 }, 225, 225, 0, 100);
		app->render->DrawRectangle(bounds, 0, 200, 255, 150);
	}
	
	if (app->sceneIntro->guiColliders && !app->scene->paused)
	{
		app->render->DrawRectangle({ rect.x - 2, rect.y - 2,rect.w + 4,rect.h + 4 }, 225, 225, 0, 100);
		app->render->DrawRectangle(bounds, 0, 200, 255, 150);
	}
	return false;
}


void GuiSlider::ChangeValue()
{
	this->value = bounds.x - minValue;

	if (this->value > 280) this->value = 280;

	else if (this->value <= 5) this->value = 0;
}

int GuiSlider::ReturnValue() const
{
	return this->value;
}
