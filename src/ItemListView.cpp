#include <stdio.h>

#include <Path.h>

#include "Messages.h"

#include "ItemListView.h"
#include "ItemView.h"
#include "WindowView.h"
#include "View.h"
#include "PrefMessage.h"
#include "DockApp.h"
#include "Alert.h"

CItemListView::CItemListView( BRect r )
	: BView( r, "ItemScroll", B_FOLLOW_ALL, 
				B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE ) //, false )
{
	mCount = 0;
	mSettings = new CPrefMessage( "slots" );
}

CItemListView::~CItemListView()
{
	mSettings->Save(); 
	delete mSettings;	
}

void CItemListView::MessageReceived( BMessage *msg )
{
	switch( msg->what )
	{
		case D8_BIG_ICON:
			SetLayout( true, true );
			break;
		case D8_SMALL_ICON:
			SetLayout( true, false );
			break;
		default: 
			BView::MessageReceived( msg );
	}
}

void CItemListView::EmptySlot( int32 slot )
{
	CItemView *v = ( CItemView * )mItemList.ItemAt( slot );	
	v->Empty();
}

void CItemListView::SetLayout( bool bLabel, bool bBig )	// has label, has big icon
{
	if( Window()->Lock() )
	{
		CItemView *v;
		float width = Bounds().IntegerWidth()-4;
			
		if( !bBig )
		{
			mSlotSize.Set( width, 20 );
		}
		else
		{
			mSlotSize.Set( width, 48 );
		}
				
		for( int32 i = 0; i < SLOT_COUNT; i++ )
		{
			v = ( CItemView *)mItemList.ItemAt( i );
			v->MoveTo( 1, i * ( mSlotSize.y + 1 )+1 );
			v->ResizeTo( mSlotSize.x, mSlotSize.y );
			v->SetLayout( bLabel, bBig );
		}
		BMessage *m = new BMessage( D8_SAVE_LAYOUT );
		m->AddBool( "haslabel", bLabel );
		m->AddBool( "isbigicon", bBig );
		Window()->PostMessage( m );
		
	}
}

void CItemListView::InitViews( bool bL, bool bB )
{
	CItemView *v = NULL;
	mSettings->Load();
	BPath path;
	const char *str;
	char t[10];
	char p[1000];
//	size_t size = 0;
//	uint32 type = '    ';
	if( Window()->Lock() )		// dont disturb the window while working
	{
		BNode n;
		for( mCount = 0; mCount < SLOT_COUNT; mCount++ )
		{
			float width = Bounds().IntegerWidth()-4;
			
			if( !bB )
			{
				mSlotSize.Set( width, 20 );
			}
			else
			{
				mSlotSize.Set( width, 48 );
			}
			
			float x = 1;
			float y = mCount * ( mSlotSize.y + 1 )+1;
			v = new CItemView( BRect(x, y, x+mSlotSize.x, y+mSlotSize.y), mCount, this, bL, bB ); // label, big
			AddChild(  v  );
			
			//v->MoveTo( 1, mCount * ( mSlotSize.y + 1 )+1 );
			//v->ResizeTo( mSlotSize.x, mSlotSize.y );
			mItemList.AddItem(  v  );		// store in BList for easy reference
			sprintf(  t, "slot %li", mCount  );

			if( !mSettings->FindString( t, &str ) )	// if pref for this slot exists and is string
			{
				//(new BAlert( "Z", str, "OK" ))->Go();
				memcpy( &p, str, strlen( str ) );    // copy the 	
				p[strlen( str )] = '\0';            // path to 
				if( n.SetTo( p ) == B_OK )
				{ 
					path.SetTo( p );
					v->SetData( path );
				}
				else
				{
					Remove( mCount );
				}
			}

		}

		Window()->Unlock();
	}
	mLastView = v;  
}

// Find slot by path
//
int32 CItemListView::FindByPath( BPath &path )
{
	CItemView *v;
	for( int32 i = 0; i < SLOT_COUNT; i++ )
	{
		v = ( CItemView* )mItemList.ItemAt( i );
		if( !v->bIsEmpty ) 
			if( path == v->mPath )
				return i;
	}
	return -1;
}

// Remove from prefs
//
void CItemListView::Remove( int32 pos )
{
	char t[200];
	sprintf( t, "slot %li", pos );
	
	mSettings->RemoveName( t );
	mSettings->Save();
	EmptySlot( pos );
}

// Store in prefs
//
void CItemListView::Save( int32 pos, const char *path )
{
	char t[10];
	const char *s;
	sprintf( t, "slot %li", pos );
	if( mSettings->FindString( t, &s ) == B_NAME_NOT_FOUND )
		mSettings->AddString( t, path );
	else
		mSettings->ReplaceString( t, path );
}

void CItemListView::AttachedToWindow( void )
{
	if( Parent() )
		SetViewColor( Parent()->ViewColor() );
	else
		SetViewColor( 0xdd, 0xdd, 0xdd );
}

// The scroll thingy, a bit messy
//
void CItemListView::MouseDown( BPoint p )
{
	uint32 b = 1;
	float ypos = 0.0;
	float bottom = ( SLOT_COUNT * ( mSlotSize.y + 1 )+1 );
	
	return; // this is buggy
	
	ScrollTo( 0, bottom );
	
	if( Window()->Lock() )
	{
		mMouseDown = p;
		BPoint t;
		while( b )
		{		
			snooze( 1000 * 20 ), 
			GetMouse( &p, &b, true );
			if( t != p )	// only if mouse has been moved...
			{
				ypos = -ConvertToParent( p ).y+Frame().top+mMouseDown.y;
				if( ypos > 0 )	// lets not scroll over the top item...
				{
					if( ypos > bottom-Frame().bottom )		// ... or under the bottom item
						ScrollTo( 0, bottom-Frame().bottom+Frame().top );
					else
						ScrollTo( 0, ypos );
				}
				else
				{	
					ScrollTo( 0, 0 );
				}
				t = p;
			}
		}
		t = BPoint( 0, 0 );
		mMouseDown = t;
		Window()->Unlock();
	}
}

