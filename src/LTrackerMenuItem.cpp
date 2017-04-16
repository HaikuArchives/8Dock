//
// LTrackerMenuItem.cpp
//
#include <NodeInfo.h>

#include "LTrackerMenuItem.h"
//#include "GetTrackerIcon.h"

LTrackerMenuItem::LTrackerMenuItem(	const entry_ref*	ref,
									BMessage*			message,
									char				shortcut,
									uint32				modifiers )
	: BMenuItem( "", message, shortcut, modifiers )
{
	InitObject( ref );
}

LTrackerMenuItem::LTrackerMenuItem(	const entry_ref*	ref,
									BMenu*				subMenu,
									BMessage*			message )
	: BMenuItem( subMenu, message )
{
	InitObject( ref );
}

LTrackerMenuItem::~LTrackerMenuItem()
{
	delete mMiniIcon;
}

void
LTrackerMenuItem::InitObject( const entry_ref* ref )
{
	// label
	BEntry	entry( ref );
	char	label[B_FILE_NAME_LENGTH];
	entry.GetName( label );
	SetLabel( label );

	// icon
	BRect	iconRect( 0, 0, B_MINI_ICON-1, B_MINI_ICON-1 );
	mMiniIcon = new BBitmap( iconRect, B_COLOR_8_BIT );
	BNodeInfo::GetTrackerIcon( ref, mMiniIcon, B_MINI_ICON );
}

void
LTrackerMenuItem::GetContentSize( float* width, float* height )
{
	BMenuItem::GetContentSize( width, height );
	*width += 20;
	*height = 16;
}

void
LTrackerMenuItem::DrawContent( void )
{
	// icon
	drawing_mode drawingMode = Menu()->DrawingMode();
	Menu()->SetDrawingMode( B_OP_OVER );
	Menu()->MovePenTo( ContentLocation() );
	Menu()->DrawBitmapAsync( mMiniIcon );
	Menu()->SetDrawingMode( drawingMode );

	// label
	Menu()->MovePenBy( 20, 12 );
	Menu()->SetDrawingMode( B_OP_ALPHA );
	Menu()->SetLowColor(B_TRANSPARENT_COLOR);
	Menu()->DrawString( Label() );
	Menu()->SetDrawingMode( B_OP_OVER );
}

void
LTrackerMenuItem::Highlight( bool flag )
{
	if ( Submenu()!=NULL ) {
		LTrackerMenu* subMenu = (LTrackerMenu*)Submenu();
		if ( !subMenu->IsValid() ) {
			subMenu->ScanFolder();
			subMenu->SetTargetForItems( Target() );
		}
	}

	BMenuItem::Highlight( flag );
}
