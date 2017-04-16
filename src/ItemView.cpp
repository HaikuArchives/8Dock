#define BM_WID 95
#define BM_HEI 31

#include "Messages.h"

#include "ItemView.h"
#include "DockApp.h"
#include "PanelView.h"
#include "ItemListView.h"
#include "LTrackerMenu.h"
#include "BuildNavMenu.h"

#include <Alert.h>
#include <MenuItem.h>
#include <PopUpMenu.h>
#include <Messenger.h>
#include <NodeInfo.h>
#include <NodeMonitor.h>
#include <Node.h>
#include <Path.h>
#include <OS.h>
#include <Bitmap.h>
#include <Rect.h>
#include <stdlib.h>
#include <stdio.h>

const char *notracker = "Tracker! Tracker, where are you boy?";

CItemView::CItemView( BRect rect, int32 pos, CItemListView *iv, bool bLbl, bool bBigI )
	: CPanelView( rect, "ItemView", B_FOLLOW_TOP | B_FOLLOW_LEFT_RIGHT, 
			B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE  )
{

	mItemListView = iv;
	bLabel = bLbl;
	bBigIcon = bBigI;
	
	mySize = 20;
	
	mDockPos = pos;
	bIsEmpty = true;
	db = 0;
	bDrawBorder = false;
	bWatching = false;
	mSmallIcon = NULL;
	mLargeIcon = NULL;
	mName = NULL;
}

CItemView::~CItemView()
{
//	if( mSmallIcon )
//		delete mSmallIcon;
//	if( mName )
//		delete[] mName;

	if( bWatching )
	{
		stop_watching( BMessenger( this ) );
		bWatching = false;
	}
}

void CItemView::Empty()
{
	if(  bIsEmpty  )
		return;
	if(  bWatching  )
	{
		stop_watching(  BMessenger(  this  )  );
		bWatching = false;
	}

//	if( mName )
//		delete[] mName;
	bIsEmpty = true;
	( ( CItemListView * )Parent() )->Remove( mDockPos );
	Invalidate();
}

void CItemView::AttachedToWindow( void )
{
	mName = NULL;

	SetDrawingMode( B_OP_OVER );

	if( Parent() )
		SetViewColor( Parent()->ViewColor() );
	else
		SetViewColor( 0xdd, 0xdd, 0xdd );
}

bool CItemView::OverIcon( BPoint p )
{
	if( bBigIcon )
	{
		BRect r( Bounds().Width() / 2 - 16, 2, 32, 32 );
		if( r.Contains( p ) )
			return true;
	}
	return false;
}

void CItemView::MouseMoved( BPoint p, uint32 code, const BMessage *msg )
{
	if( code == B_ENTERED_VIEW && msg != NULL )
	{
		if( msg->what == B_SIMPLE_DATA )
		{
			bDrawBorder = true;
			Invalidate();
		}
	}
	
	if( code == B_INSIDE_VIEW && msg != NULL && !bIsEmpty )
	{
		if( !OverIcon( p ) )
			bDrawBorder = true;
		else
			bDrawBorder = false;
		Invalidate();
	}
	
	if( code == B_EXITED_VIEW && bDrawBorder == true )
	{
		bDrawBorder = false;
		Invalidate();
	}
}

void CItemView::MouseDown( BPoint p )
{
	uint32	buttons;
	int64	clickspeed;
	get_click_speed( &clickspeed );
	
	BPoint p_;
	
	GetMouse( &p_, &buttons );
	
	if( buttons == B_SECONDARY_MOUSE_BUTTON )
	{	
		BPopUpMenu *menu;
		BMenuItem *m;

		menu = new BPopUpMenu( "HUH?" );
		
		if( bBigIcon )
			m = new BMenuItem( "Small Icons" , new BMessage( D8_SMALL_ICON ) );
		else
			m = new BMenuItem( "Big Icons" , new BMessage( D8_BIG_ICON ) );
		m->SetTarget( mItemListView );
		menu->AddItem( m );
		
		
		m = new BMenuItem( "About 8Dock" B_UTF8_ELLIPSIS, new BMessage( B_ABOUT_REQUESTED ) );
		m->SetTarget( be_app );
		menu->AddItem( m );
		
		m = new BMenuItem( "Quit", new BMessage( B_QUIT_REQUESTED ) );
		m->SetTarget( Window() );
		menu->AddItem( m );

		if( !bIsEmpty )
		{
			char t[100];
			sprintf( t, "Remove '%s'", mName );
			menu->AddItem( new BSeparatorItem(), 0 );
			m = new BMenuItem( t, new BMessage( D8_EMPTY_SLOT ) );
			m->SetTarget( this );
			menu->AddItem( m, 0 );

			BuildNavMenu( menu, &mRef, new BMessage( D8_TRACKER ), this );
		}
		
		p_.x -= 4;
		p_.y -= 4;
		m = menu->Go( ConvertToScreen( p_ ), true, true, true );
		return;
	}
	
	if( ( system_time()-db ) < clickspeed )
	{
		if( !bIsEmpty )
		{
			
			BMessage	message( B_REFS_RECEIVED );
			BMessenger	*tracker;
			tracker = new BMessenger( "application/x-vnd.Be-TRAK", -1, NULL );
			if ( tracker->IsValid() ) 
			{
				message.AddRef( "refs", &mRef );
				tracker->SendMessage( &message );		
				delete tracker;
			}
			else
			{
				(new BAlert( "eek!", notracker, "Hmmm..." ))->Go();					
			}
		}
		db = 0;	
	}
	else
	{
		if( modifiers() & B_SHIFT_KEY > 0 )
		{
			Parent()->MouseDown( ConvertToParent( p ) );
			return;
		}
		
		if( bIsEmpty )
			return;

		float x_offset = 0;
		
		BBitmap *ic;
		BView *iv;

		BFont f;
		font_height fh;
		BRect r;

		GetFont( &f );
		f.GetHeight( &fh );
		if( bBigIcon )
		{
			if( mStringWidth < 32 )
				r.Set( 0, 0, 32, 50 );
			else
				r.Set( 0, 0, mStringWidth + 8, 50 );
		}
		else
		{
			r.Set( 0, 0, mStringWidth + 32, fh.ascent + fh.descent+ 32 );
		}
		ic = new BBitmap( r, B_RGB32, true );
		iv = new BView( r, "t", B_FOLLOW_NONE, B_WILL_DRAW );

		ic->AddChild( iv );
		ic->Lock();
	
		iv->SetLowColor( 255, 255, 255, 0 );
		iv->SetDrawingMode( B_OP_COPY );
		iv->FillRect( r, B_SOLID_LOW );

		if( bBigIcon )
		{
			if( bLabel )
			{
				iv->SetHighColor( 0, 0, 0,  128);
				iv->MovePenTo( iv->Bounds().Width() / 2 - mStringWidth / 2, 46 );
				iv->DrawString( mName );
			}
			x_offset = iv->Bounds().Width() / 2 - 16;
			
			//iv->StrokeRect( iv->Bounds() );
			
			iv->SetDrawingMode( B_OP_ALPHA );
			iv->MovePenTo( x_offset, 2 );
			iv->SetBlendingMode( B_CONSTANT_ALPHA, B_ALPHA_COMPOSITE );
			
			if( mLargeIcon )
				iv->DrawBitmap( mLargeIcon );
		}
		else
		{

			if( bLabel )
			{
				iv->SetHighColor( 0, 0, 0,  128);
				iv->MovePenTo( 22, 15 );
				iv->DrawString( mName );
			}
			
			iv->SetDrawingMode( B_OP_ALPHA );
			iv->MovePenTo( 2, 2 );
			iv->SetBlendingMode( B_CONSTANT_ALPHA, B_ALPHA_COMPOSITE );
			if( mSmallIcon )
				iv->DrawBitmap( mSmallIcon );
		}
		
		
		BMessage *m = new BMessage( B_SIMPLE_DATA );
		m->AddInt32( "slot", mDockPos );
		m->AddRef( "refs", &mRef );
		if( bBigIcon )
			DragMessage( m, ic, B_OP_ALPHA, BPoint( p_.x - Bounds().Width() / 2 + x_offset + 16, p_.y ) );
		else
			DragMessage( m, ic, B_OP_ALPHA, BPoint( p_.x , p_.y ) );
	
		db = system_time();

		ic->Unlock();
	}
}

void CItemView::Draw( BRect r )
{
	
	if( !bIsEmpty )
	{
		if( bBigIcon )
		{	
			MovePenTo( Bounds().Width() / 2 - 16, 2 );
			if( mLargeIcon )
				DrawBitmapAsync( mLargeIcon );
			float f = Bounds().Width() / 2 - mStringWidth / 2;
			if( f < 2 ) f = 2;
			MovePenTo( f, 46 );
			DrawString( mName );
		}
		else
		{
			MovePenTo( 2, 2 );
			if( mSmallIcon )
				DrawBitmapAsync( mSmallIcon );
		
			MovePenTo( 22, 15 );
			DrawString( mName );
		}
	}
	if( bDrawBorder )
	{
		BRect r = Bounds();
		r.top += 1;
		r.left += 1;
		r.right -= 1;
		r.bottom -= 1;
		SetHighColor( 0, 0, 0xdd );
		StrokeRect( r );
	}
	SetHighColor( CPanelView::mHighColor.red-2, CPanelView::mHighColor.green-2, CPanelView::mHighColor.blue-2 );

	CPanelView::Draw( r );
}

void CItemView::SetData( BPath &p )
{
	if( !bIsEmpty )
	{
		BPath path = mPath.Path();
		( ( CItemView *)mItemListView->mItemList.ItemAt( ( mDockPos+1 ) % SLOT_COUNT ) )->SetData( path );
	}
	
	BEntry e( p.Path() );
	if( e.InitCheck() == B_NO_INIT )
	{
		char t[200];
		sprintf( t, "The path '%s' is invalid.", p.Path() );		
		Empty();
		(new BAlert( "eeek!", t, "OK" ))->Go();
		return;
	}
	mPath = p;
	e.GetRef( &mRef );
	SetData();
}

void CItemView::SetData()
{
	BFont f;
	GetFont( &f );

	bDrawBorder = false;
	
	mNode.SetTo( &mRef );
	mNodeInfo.SetTo( &mNode );	
	
	if( mSmallIcon == NULL )
		mSmallIcon = new BBitmap( BRect( 0, 0, 15, 15 ), B_COLOR_8_BIT );
	if( mLargeIcon == NULL )
		mLargeIcon = new BBitmap( BRect( 0, 0, 31, 31 ), B_COLOR_8_BIT );


	BNodeInfo::GetTrackerIcon(  &mRef, mLargeIcon, B_LARGE_ICON  );
	BNodeInfo::GetTrackerIcon(  &mRef, mSmallIcon, B_MINI_ICON  );

	if(  mName != NULL  )
		delete[] mName;

	mName = new char[strlen( mRef.name )+2];
	strcpy( mName, mRef.name );

	mStringWidth = f.StringWidth( mName );

	mItemListView->Save( mDockPos, mPath.Path() );
	bIsEmpty = false;

	if( !bWatching )
	{	
		node_ref nref;
		BEntry e( &mRef );
		e.GetNodeRef( &nref );
		if( watch_node( &nref, B_ENTRY_MOVED | B_ENTRY_REMOVED, BMessenger( this ) ) != B_NO_ERROR )
		{	
			char t[500];
			sprintf( t, "Could not watch %s.", mPath.Path() );
			(new BAlert( "eeek!", t, "OK" ))->Go();
			bWatching = true;
		}
	}
	Invalidate();

}

void CItemView::EntryMoved( BMessage *msg )
{
	const char *name;
	
	msg->FindInt32( "device", &mRef.device ); 
	msg->FindInt64( "to directory", &mRef.directory );
	msg->FindString( "name", &name );

	mRef.set_name( name );

	BEntry e( &mRef );
	e.GetPath( &mPath );

	SetData();
}

void CItemView::MessageReceived( BMessage *msg )
{
	switch( msg->what )
	{
		case D8_TRACKER:
			{
				entry_ref ref;
				msg->FindRef( "refs", &ref );
				BMessage	message( B_REFS_RECEIVED );
				BMessenger	*tracker;
				tracker = new BMessenger( "application/x-vnd.Be-TRAK", -1, NULL );
				if ( tracker->IsValid() ) 
				{
					message.AddRef( "refs", &ref );
					tracker->SendMessage( &message );		
					delete tracker;
				}
				else
				{
					(new BAlert( "eek!", notracker, "Hmmm..." ))->Go();					
				}
			}
			break;
		case B_SIMPLE_DATA :
			FromTracker( msg );		// drag from the tracker
			break;
		case B_NODE_MONITOR:
			int32 opcode;
			msg->FindInt32( "opcode", &opcode );
			switch( opcode )
			{
				case B_ENTRY_REMOVED:
					Empty();
					break;
				case B_ENTRY_MOVED:
					EntryMoved( msg );
					break;
			}
			break;
		case D8_EMPTY_SLOT:
			Empty();
			break;
		default:
			CPanelView::MessageReceived( msg );
	}
}


void CItemView::FromTracker( BMessage *msg )
{
	entry_ref ref;
	if( msg->FindRef( "refs", &ref ) == B_NO_ERROR )
	{

		int32 item;
		BPath p;
		BEntry e;
		if( msg->HasInt32( "slot" ) )	// msg from another slot
		{
			msg->FindInt32( "slot", &item );
			if( item == mDockPos )
				return;
				
			mItemListView->Remove( item );

			e.SetTo( &ref, true );
			e.GetPath( &p );
			SetData( p );
		}
		else
		{
			entry_ref ref;
			int32 num;
			
			for ( int count = 0; msg->FindRef( "refs", count, &ref ) == B_NO_ERROR; count ++ ) 
			{
				e.SetTo( &ref, true );
				e.GetPath( &p );
	
				num = mItemListView->FindByPath( p );
				if( num != -1 ) 
				{
					mItemListView->Remove( num );
				}
				SetData( p );
			}
		}
	}
}

void CItemView::SetLayout( bool label, bool big )
{
	bLabel = label;
	bBigIcon = big;
	Invalidate();
}
