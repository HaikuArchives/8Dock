#include <Application.h>
#include <Screen.h>
#include <stdio.h>
#include <DirectWindow.h>
#include <Alert.h>
#include <PopUpMenu.h>

#include "DockApp.h"

const char *app_signature = "application/x-vnd.jonr-8Dock";

CDockApp::CDockApp()
	: BApplication( app_signature )
{
	_prefs = new CPrefMessage( "general" );
}

CDockApp::~CDockApp()
{
	delete _prefs;
}

void CDockApp::ReadyToRun()
{
	float x, y, xw, yw;
	bool bBig, bLabel, bTrash;
	
	BScreen screen( B_MAIN_SCREEN_ID );
	BRect r = screen.Frame();

	_prefs->Load();

	if( _prefs->FindBool( "haslabel", &bLabel ) != B_OK )
		bLabel = true;
	if( _prefs->FindBool( "isbigicon", &bBig ) != B_OK )
		bBig = false;
	if( _prefs->FindBool( "hastrash", &bTrash ) != B_OK )
		bTrash = true;

	mwDock = new CDockWindow( BRect( 100, 100, 300, 100 ), bLabel, bBig, bTrash );

	if( _prefs->FindFloat( "xpos", &x ) != B_OK )
		x = 2;
	if( _prefs->FindFloat( "ypos", &y ) != B_OK )
		y = 2;
	if( _prefs->FindFloat( "xwidth", &xw ) != B_OK )
		xw = 86;
	if( _prefs->FindFloat( "ywidth", &yw ) != B_OK )
		yw = r.bottom - 4;

	

	mwDock->MoveTo( x, y );
	mwDock->ResizeTo( xw, yw );
	mwDock->SetSizeLimits( 100, 200, 200, r.bottom - 4 );

	mwDock->Show();
}

void CDockApp::AboutRequested()
{
	BAlert *a = new BAlert( "O", "8Dock v. 0.9.5 \nAuthor: Jón Ragnarsson (jonr@vortex.is)\n \
Thanks to:\n\t Hiromasa Kato for his TrackerMenu classes.\n", "OK" );
	a->Go();
}

int main( int argc, char **argv )
{
	CDockApp *dock = new CDockApp();
	dock->Run();
	return 0;
}
