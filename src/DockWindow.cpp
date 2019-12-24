#include <Window.h>
#include <Alert.h>

#include "Messages.h"
#include "PrefMessage.h"
#include "DockWindow.h"
#include "DockApp.h"

CDockWindow::CDockWindow( BRect r, bool haslabel, bool isbigicon, bool trash )
	: BWindow( r, "DockWin", B_BORDERED_WINDOW, 
		B_NOT_ZOOMABLE  | B_WILL_ACCEPT_FIRST_CLICK, B_ALL_WORKSPACES | B_FLOATING_ALL_WINDOW_FEEL )
{
	bPrefs = false;
	CDockApp *dapp;
	dapp = ( CDockApp * )be_app;
	mSettings = dapp->_prefs;
	mView = new CWindowView( Bounds(), haslabel, isbigicon, trash );
	AddChild( mView );
	
}

void CDockWindow::MessageReceived( BMessage *msg )
{
	
	switch( msg->what )
	{
		case D8_SAVE_LAYOUT:
			msg->PrintToStream();
			bool b, bb, bl;	// temp, big icon, label
			msg->FindBool( "haslabel", &bl );
			msg->FindBool( "isbigicon", &bb );
			if( mSettings->FindBool( "haslabel", &b ) == B_NAME_NOT_FOUND )
			{
				mSettings->AddBool( "haslabel", bl );
				mSettings->AddBool( "isbigicon", bb );
			}
			else
			{
				mSettings->ReplaceBool( "haslabel", bl );
				mSettings->ReplaceBool( "isbigicon", bb );
			}
			break;
		default:
			BWindow::MessageReceived( msg );
	}
}

void CDockWindow::ScreenChanged( BRect r, color_space cs )
{
	if( r.bottom < Frame().bottom )
		ResizeTo( Frame().right, r.bottom-2 );
}

void CDockWindow::FrameResized( float x, float y )
{
	if(x<MIN_WINDOW_WIDTH) x=MIN_WINDOW_WIDTH;
	if(y<MIN_WINDOW_HEIGHT) y=MIN_WINDOW_HEIGHT;

	float f;
	
	if( mSettings->FindFloat( "xwidth", &f ) == B_NAME_NOT_FOUND )
		mSettings->AddFloat( "xwidth", x );
	else
		mSettings->ReplaceFloat( "xwidth", x );
		
	if( mSettings->FindFloat( "ywidth", &f ) == B_NAME_NOT_FOUND )
		mSettings->AddFloat( "ywidth", y );
	else
		mSettings->ReplaceFloat( "ywidth", y );
	ResizeTo(x,y);
}

void CDockWindow::FrameMoved( BPoint p )
{
	float f;

	if( mSettings->FindFloat( "xpos", &f ) == B_NAME_NOT_FOUND )
		mSettings->AddFloat( "xpos", p.x );
	else
		mSettings->ReplaceFloat( "xpos", p.x );
		
	if( mSettings->FindFloat( "ypos", &f ) == B_NAME_NOT_FOUND )
		mSettings->AddFloat( "ypos", p.y );
	else
		mSettings->ReplaceFloat( "ypos", p.y );
}

bool CDockWindow::QuitRequested()
{
	be_app->PostMessage( B_QUIT_REQUESTED );
	mSettings->Save();
	return TRUE;
}

