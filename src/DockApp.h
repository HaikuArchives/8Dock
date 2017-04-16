#ifndef DOCKAPP_H
#define DOCKAPP_H

#include <Application.h>

#include "DockWindow.h"
#include "PrefMessage.h"

#define SET_PREF		'setP'
#define GET_PREF		'getP'

class CDockApp : public BApplication
{
public:
							CDockApp();
							~CDockApp();
	void 					ReadyToRun(void);
	void					AboutRequested();

	CPrefMessage			*_prefs;

private:

	BRect					ScreenSize();
	CDockWindow				*mwDock;	
};

#endif
