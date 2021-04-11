#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiButton : public GuiControl
{
public:

	GuiButton(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GuiButton();

	bool Update(float dt);
	bool Draw();

private:

	// Gui Button specific properties
	// Maybe some animation properties for state change?

	int guiButtonFx = 0;
	int guiButtonErrorFx = 0;
	int guiButtonMoveFx = 0;
	bool soundDone = false;

};

#endif // __GUIBUTTON_H__
