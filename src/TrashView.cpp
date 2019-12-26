#include <Message.h>
#include <Alert.h>
#include <View.h>
#include <Resources.h>
#include <Bitmap.h>
#include <Screen.h>
#include <Directory.h>
#include <DirectWindow.h>
#include <FindDirectory.h>
#include <Path.h>
#include <NodeInfo.h>
#include <NodeMonitor.h>
#include <PopUpMenu.h>
#include <MenuItem.h>

#include <Messenger.h>
#include <Entry.h>
#include <stdio.h>
#include <string.h>

#include "TrashView.h"

//#include "BuildNavMenu.h"

CTrashView::CTrashView( BPoint p )
	: BView( BRect( p.x, p.y, p.x+32,p.y+32 ), "TrashView", B_FOLLOW_H_CENTER | B_FOLLOW_BOTTOM,
		B_WILL_DRAW )
{
	iconL = new BBitmap( BRect( 0,0,31,31 ), B_RGBA32 );
	iconS = new BBitmap( BRect( 0,0,15,15 ), B_RGBA32 );
	bHighlight = false;
	menuok = false;
}

void CTrashView::AttachedToWindow( void )
{
	node_ref nref;
	BScreen screen;
	BPath p;
	find_directory( B_TRASH_DIRECTORY, &p );
	BEntry e( p.Path(  ) );
	mTrashDir = new BDirectory( p.Path(  ) );

	SetDrawingMode( B_OP_OVER );

	systemColors = system_colors(  );
	for ( int i = 0; i < 256; ++i ) 
	{              
		rgb_color aColor = systemColors->color_list[i]; 
		aColor.red = ( long )aColor.red * 2 / 3; 
		aColor.green = ( long )aColor.green * 2 / 3; 
		aColor.blue = ( long )aColor.blue * 2 / 3; 
		mHiliteTable[i] = screen.IndexForColor( aColor );        //( ( ( aColor.red>>3 )&0x1f )<<10 )+( ( ( aColor.green>>3 )&0x1f )<<5 )+( ( aColor.blue>>3 )&0x1f ); 
	} 

	e.GetRef( &mRef );

	BNodeInfo::GetTrackerIcon( &mRef, iconL, B_LARGE_ICON );
	BNodeInfo::GetTrackerIcon( &mRef, iconS, B_MINI_ICON );
	
	if( Parent(  ) )
		SetViewColor( Parent(  )->ViewColor(  ) );
	else
		SetViewColor( 0xdd,0xdd,0xdd );

	e.GetNodeRef( &nref );
	if( watch_node( &nref, B_WATCH_DIRECTORY, BMessenger( this ) ) != B_NO_ERROR )
		(new BAlert( "zz", "62 watch_node(  ) for Trash failed", "OK"  ))->Go();    
		
	menu = new BPopUpMenu( "HUH?" , false, false);
	BMenuItem *m;

	m = new BMenuItem( "Empty Trash" , new BMessage( 'empt' ) );
	m->SetTarget( this );
	menu->AddItem( m );
		
	m = new BMenuItem( "Open", new BMessage( 'open' ) );
	m->SetTarget( this );
	menu->AddItem( m );
//	SetViewColor( B_TRANSPARENT_32_BIT );
}

CTrashView::~CTrashView(  )
{
	stop_watching( BMessenger( this ) );
}

void CTrashView::MouseMoved( BPoint p, uint32 code, const BMessage *msg )
{
	if( code == B_ENTERED_VIEW && msg != NULL )
	{
		bHighlight = true;
		if( msg->what == B_SIMPLE_DATA )
		{
			uchar *data = ( uchar * )iconL->Bits(  );
			for ( long i = 0; i < iconL->BitsLength(  ); ++i ) 
			{
				if( *( data+i ) != B_TRANSPARENT_8_BIT )	
					*( data+i ) = mHiliteTable[*( data+i )];
			}	
			Invalidate(  );
		}
	}
	if( code == B_EXITED_VIEW && bHighlight )
	{
		BNodeInfo::GetTrackerIcon( &mRef, iconL, B_LARGE_ICON );
		BNodeInfo::GetTrackerIcon( &mRef, iconS, B_MINI_ICON );
		Invalidate(  );
	}

}

void CTrashView::MouseDown( BPoint p )
{
	uint32	buttons;
	int64	clickspeed;
	get_click_speed( &clickspeed );
	
	BPoint p_;
	GetMouse( &p_, &buttons );

	if( buttons == B_SECONDARY_MOUSE_BUTTON )
	{	
		if( !menuok )	
		{
			menu->AddItem( new BSeparatorItem(  ),0 );
			//BuildNavMenu( menu, &mRef, new BMessage( '8dTR' ), this );
			menuok = true;
		}
		p_ -= BPoint(4, 4);
		menu->Go( ConvertToScreen( p_ ), true, true );
		return;
	}

	if( ( system_time(  )-db ) < clickspeed )
	{
		OpenTracker(  );
		db = 0;	
	}
	else
	{
		db = system_time(  );
	}
}

void CTrashView::OpenTracker(  )
{
	BMessage	message( B_REFS_RECEIVED );
	BMessenger	*tracker;
	tracker = new BMessenger( "application/x-vnd.Be-TRAK", -1, NULL );
	if ( tracker->IsValid(  ) ) 
	{
		message.AddRef( "refs", &mRef );
		tracker->SendMessage( &message );		
		delete tracker;
	}
}

void CTrashView::EmptyDir( BDirectory *dir )
{
	BEntry e;
	BPath p;
	while( dir->GetNextEntry( &e, false ) == B_OK )
	{
		if( e.IsDirectory(  ) )
		{
			e.GetPath( &p );
			BDirectory dir(p.Path());
			EmptyDir( &dir );
		}
		else
		{
			e.Remove(  ); 
		}
	};
	if( dir != mTrashDir )
	{
		dir->GetEntry( &e );
		e.Remove(  );
	}
}

// Statable.h Entry.h Directory.h Node.h

void CTrashView::MessageReceived( BMessage *msg )
{
	switch( msg->what )
	{
		case 'open':
			OpenTracker(  );
			break;
		case 'empt':
			EmptyDir( mTrashDir );
			break;
		case B_NODE_MONITOR:
			snooze( 15000 );
			BNodeInfo::GetTrackerIcon( &mRef, iconL, B_LARGE_ICON );
			BNodeInfo::GetTrackerIcon( &mRef, iconS, B_MINI_ICON );
			Invalidate(  );
			break;
		case B_SIMPLE_DATA :
			if( msg->FindInt32( "slot" ) )
				msg->SendReply( new BMessage( '8dcl' ) );
			else
				MoveToTrash( msg ); 
			break;
		default:
			BView::MessageReceived( msg );
	}
}

void CTrashView::MoveToTrash( BMessage *msg )
{
	entry_ref ref;
	if( msg->FindRef( "refs", &ref ) == B_NO_ERROR )
	{

//		(new BAlert( "zz", "MoveToTrash", "OK"  ))->Go();    

		BMessage msg, reply; 
      	BMessage specifier( 'sref' );

		// form scripting request 
		specifier.AddRef( "refs", &ref );
		specifier.AddString( "property", "Entry" ); 

		msg.what = B_DELETE_PROPERTY; 

		msg.AddSpecifier( &specifier ); 
		msg.AddSpecifier( "Poses" ); 
		msg.AddSpecifier( "Window", 1 ); 
      
      // deliver request and fetch response 
		BMessenger m("application/x-vnd.Be-TRAK");
		m.SendMessage(&msg, &reply); 
//  		printf( "ERROR: %ld %s\n", s, strerror(s) ) ;
//		printf( "REPLY: " ) ;
//		reply.PrintToStream() ;
   
	}
}

void CTrashView::Draw( BRect r )
{
	MovePenTo( 0,0 );
	if( iconL )
	{
		DrawBitmapAsync( iconL );
	}
}
