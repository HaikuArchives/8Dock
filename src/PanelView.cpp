#include "PanelView.h"
#include <ControlLook.h>

//BRect frame, const char *name, uint32 resizingMode, uint32 flags

CPanelView::CPanelView( BRect r, const char* name, uint32 mode, uint32 flags, bool raised )
	: BView( r, name, mode, flags )
{
	mIsRaised = raised;
	SetViewColor( B_TRANSPARENT_COLOR );
}

void CPanelView::Draw( BRect dirty )
{
	DrawBorder( dirty );
}

void CPanelView::DrawBorder( BRect dirty )
{
	BRect bounds = Bounds();
	be_control_look->DrawMenuBarBackground(this, bounds, dirty, ViewColor());
	return;
	BeginLineArray( 4 );
	if( mIsRaised )
		SetHighColor( mHighColor );
	else
		SetHighColor( mLowColor );
	MovePenTo( Bounds().right, 0 );
	StrokeLine( BPoint( 0, 0 ) );
	StrokeLine( BPoint( 0, Bounds().bottom ) );

	if( mIsRaised )
		SetHighColor( mLowColor );
	else
		SetHighColor( mHighColor );
	StrokeLine( BPoint( Bounds().right, Bounds().bottom ) );
	StrokeLine( BPoint( Bounds().right, 0 ) );
	EndLineArray();
}

void CPanelView::SetViewColor( rgb_color col )
{
	uchar t;	

	BView::SetViewColor(  col  );
	SetLowColor(  col  );
	
	col.red <= 0x33 ? t = 0 : t = col.red - 0x33; 
	mLowColor.red = t;
	col.green <= 0x33 ? t = 0 : t = col.green - 0x33; 
	mLowColor.green = t;
	col.blue <= 0x33 ? t = 0 : t = col.blue - 0x33; 
	mLowColor.blue = t;
	
	col.red >= 0xdd ? t = 0xff : t = col.red + 0x22; 
	mHighColor.red = t;
	col.green >= 0xdd ? t = 0xff : t = col.green + 0x22; 
	mHighColor.green = t;
	col.blue >= 0xdd ? t = 0xff : t = col.blue + 0x22; 
	mHighColor.blue = t;
}

rgb_color CPanelView::GetHighColor()
{
	return mHighColor;
}

rgb_color CPanelView::GetLowColor()
{
	return mLowColor;
}


void CPanelView::AttachedToWindow( void )
{
	if( Parent() )
		SetViewColor( Parent()->ViewColor() );
	else
		SetViewColor( 0xdd,0xdd,0xdd );
}

/*
void CPanelView::FrameResized( int32 new_width, int32 new_height )
{
	BRect new_bounds = Bounds();

	//Invalidate the right border
	int min_right = 0;
	if( new_bounds.right < mBounds.right )
		min_right = new_bounds.right-c_border_width;
	else if( new_bounds.right > mBounds.right )
		min_right = mBounds.right-c_border_width;
	if( new_bounds.right != mBounds.right )
	Invalidate( BRect( min_right,new_bounds.top,new_bounds.right,
		new_bounds.bottom ) );	//May be using the wrong parameters
					//but you get the idea...

	//Invalidate the bottom border
	int min_bottom = 0;
	if( new_bounds.bottom < mBounds.bottom )
		min_bottom = new_bounds. bottom-c_border_width;
	else if( new_bounds.bottom > mBounds.bottom )
		min_bottom = mBounds. bottom-c_border_width;
	if( new_bounds.bottom != mBounds.bottom )
	Invalidate( BRect( new_bounds.left,min_bottom,new_bounds.right,
		new_bounds.bottom ) );	//May be using the wrong parameters
					//but you get the idea...

	//Cache the bounds for use in draw
	mBounds = new_bounds;
}
*/
