//
// LTrackerMenu.cpp
//

#include "LTrackerMenu.h"
#include "LTrackerMenuItem.h"
#include <Entry.h>
#include <Directory.h>

LTrackerMenu::LTrackerMenu(	entry_ref*		ref,
							menu_layout		layout )
	: BMenu( "", layout ), mIsValid( false ), mRef( *ref )
{
	BEntry	entry( &mRef );
	char	label[B_FILE_NAME_LENGTH];
	entry.GetName( label );
	SetName( label );
}

LTrackerMenu::~LTrackerMenu()
{
}

void
LTrackerMenu::ScanFolder( void )
{
	BNode				node;
	BDirectory			folder;
	entry_ref			itemRef;
	LTrackerMenuItem*	item;
	LTrackerMenu*		subMenu;

	node.SetTo( &mRef );
	if ( !node.IsDirectory() ) {
#if defined(DEBUG)
		BEntry entry( &mRef );
		char label[B_FILE_NAME_LENGTH];
		entry.GetName( label );
		printf( "%s is not a folder\n", label );
#endif
		return;
	}

	folder.SetTo( &mRef );

	if ( folder.CountEntries() == 0 ) {
		BMenuItem* empty = new BMenuItem( "Empty Folder", NULL );
		empty->SetEnabled( false );
		SetTriggersEnabled( false );
		AddItem( empty );
	} else {
		while ( folder.GetNextRef( &itemRef ) != B_ENTRY_NOT_FOUND ) {
			node.SetTo( &itemRef );
			BMessage* msg = new BMessage( *Superitem()->Message() );
			msg->RemoveName( "refs" );
			msg->AddRef( "refs", &itemRef );
			if ( node.IsDirectory() ) {
				subMenu = new LTrackerMenu( &itemRef );
				subMenu->SetFont( be_plain_font );
				item = new LTrackerMenuItem( &itemRef, subMenu, msg );
			} else {
				item = new LTrackerMenuItem( &itemRef, msg );
			}
			AddItem( item );
		}
	}

	mIsValid = true;
}
