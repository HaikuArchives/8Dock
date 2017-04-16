//
// BuildNavMenu.cpp
//

#include "BuildNavMenu.h"
#include "LTrackerMenu.h"
#include "LTrackerMenuItem.h"
#include <Menu.h>
#include <MenuItem.h>
#include <Entry.h>
#include <Node.h>
#include <Directory.h>

status_t BuildNavMenu( BMenu* top, const entry_ref* ref, BMessage* message, BHandler* target )
{
	BNode		node;
	BEntry		entry;
	BDirectory	folder;

	node.SetTo( ref );
	if ( !node.IsDirectory() ) {
		return B_ERROR;
	}

	LTrackerMenu* subMenu = new LTrackerMenu( (entry_ref*)ref );
	subMenu->SetFont( be_plain_font );
	BMessage* msg = new BMessage( *message );
	msg->AddRef( "refs", ref );
	LTrackerMenuItem* item = new LTrackerMenuItem( ref, subMenu, msg );
	item->SetTarget( target );
	top->AddItem( item ,0);

	return B_NO_ERROR;
}
