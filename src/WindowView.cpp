#include "WindowView.h"
#include "HandleView.h"
#include "PanelView.h"
#include "TrashView.h"
#include "ItemListView.h"
#include <View.h>

CWindowView::CWindowView( BRect r, bool haslabel, bool isbigicon, bool trash )
	: CPanelView( r, "DockView", B_FOLLOW_ALL, 
			B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE ),
	bLabel( haslabel ),
	bBig( isbigicon )
{
	rect = r;
}


void CWindowView::AttachedToWindow()
{
	SetViewColor( 0xdd, 0xdd, 0xdd, 0xff );
	rect = Bounds();

	mTrashView = new CTrashView( BPoint( 64 + 20, rect.bottom - 40 ) );
	AddChild( mTrashView );


	mResizeView = new CHandleView( 
			BRect( rect.right - 15, rect.bottom - 15, rect.right - 1, rect.bottom - 1 ), B_FOLLOW_BOTTOM | B_FOLLOW_RIGHT, true, BOTTOM_RIGHT );
	AddChild( mResizeView );

	mMoveView = new CHandleView( BRect( 1, 1, rect.right - 1, 10 ), B_FOLLOW_TOP | B_FOLLOW_LEFT_RIGHT, 
		false, FILL );
	AddChild( mMoveView );	



	mItemListView = new CItemListView( BRect( 4, 14, rect.right -2, rect.bottom - 52 ) );
	AddChild( mItemListView );
	mItemListView->InitViews( bLabel, bBig );
}

void CWindowView::Draw( BRect r )
{
	
	BRect sr = Bounds(); //BRect( 4, 12, rect.right - rect.left - 4, rect.bottom - rect.top - 52 ); //mItemListView - >ConvertToParent( mItemListView - >Bounds() );
	sr.InsetBy( 4, 4 );
	sr.SetLeftTop( BPoint( sr.left, sr.top  +  10 ) );
	sr.SetLeftBottom( BPoint( sr.left, sr.bottom  -  48 ) );
	SetHighColor( GetLowColor() );
	MovePenTo( sr.right + 1, sr.top - 1 );
	StrokeLine( BPoint( sr.left - 1, sr.top - 1 ) );
	StrokeLine( BPoint( sr.left - 1, sr.bottom + 1 ) );
	SetHighColor( GetHighColor() );
	StrokeLine( BPoint( sr.right + 1, sr.bottom + 1 ) );
	StrokeLine( BPoint( sr.right + 1, sr.top - 1 ) );
	
	CPanelView::Draw( r );
}
