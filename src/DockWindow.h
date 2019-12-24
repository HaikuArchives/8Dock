#ifndef DOCKWINDOW_H
#define DOCKWINDOW_H


#include <Window.h>
#include <Menu.h>

#include "WindowView.h"
#include "PrefMessage.h"

#define MIN_WINDOW_WIDTH 	100
#define MIN_WINDOW_HEIGHT	200

class CDockWindow : public BWindow
{
public:
							CDockWindow( BRect, bool, bool, bool );
	bool					QuitRequested();
	void					ScreenChanged( BRect, color_space );
	void					FrameResized( float, float );
	void					FrameMoved( BPoint );
	void					MessageReceived( BMessage * );
			
private:

	CWindowView				*mView;
	float					*mXpos, *mYpos, *mXsize, *mYsize; 		// user pos
	BMenu					*menu;
	bool					bPrefs;
	CPrefMessage			*mSettings;
	
};

#endif
