#include <Window.h>
#include <Picture.h>
#include "HandleView.h"

CHandleView::CHandleView(BRect r, uint32 resizeMask, bool resize, uint32 pattern, uint32 movement)
	: BView(r, "HandleView", resizeMask, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE )
{
	mResize = resize;		// resize or move window?
	mPattern = pattern;
	mPicture = new BPicture;
	mMovement = movement;
}

CHandleView::~CHandleView()
{
	delete mPicture;
}

void CHandleView::AttachedToWindow()
{
	rgb_color col = Parent()->ViewColor();

	SetViewColor(col);
	SetLowColor(col);
//	SetViewColor(0xee,0,0xee);	// XXX
	
	col.red <= 0x77 ? mLowColor.red = 0 : mLowColor.red = col.red - 0x77; 
	col.green <= 0x77 ? mLowColor.green = 0 : mLowColor.green = col.green - 0x77; 
	col.blue <= 0x77 ? mLowColor.blue = 0 : mLowColor.blue = col.blue - 0x77; 
	col.red >= 0xdd ? mHighColor.red = 0xff : mHighColor.red = col.red + 0x22; 
	col.green >= 0xdd ? mHighColor.green = 0xff : mHighColor.green = col.green + 0x22; 
	col.blue >= 0xdd ? mHighColor.blue = 0xff : mHighColor.blue = col.blue + 0x22; 
	//MoveBy(-1,-1);
}

void CHandleView::MouseDown(BPoint p)
{
	uint32 b = 1;
	mMouseDown = p;
	BPoint t;
	
	while(b)
	{
		snooze(1000 * 20),
		GetMouse(&p, &b, true);
		if(p != t)
		{
			if(mResize)
			{
				Window()->ResizeBy(p.x-mMouseDown.x, p.y-mMouseDown.y);
			}
			else
			{
				Window()->MoveBy(p.x-mMouseDown.x, p.y-mMouseDown.y);
			}
			t = p;
		}
	}
}


void CHandleView::Draw(BRect r)
{
	float w;
	float t;
	float x;
	float y;

	float xf = Bounds().right;
	float yf = Bounds().bottom;
	xf <= yf ? t = xf: t = yf;
	xf <= yf ? w = yf: w = xf;

//	StrokeRect(Bounds());

	switch(mPattern)
	{
		case FILL:
			for(x = 1; x < xf; x+=3)
			{
				for(y = 1; y < yf; y+=3)
				{
					SetHighColor(mLowColor);
					StrokeLine(BPoint(x,y), BPoint(x,y));
					SetHighColor(mHighColor);
					StrokeLine(BPoint(x+1,y+1), BPoint(x+1,y+1));
				}
			}
			break;
		case BOTTOM_RIGHT:
			for(x = 1; x < t; x+=3)
			{
				for(y = 1; y <= x; y+=3)
				{
					SetHighColor(mLowColor);
					StrokeLine(BPoint(xf-t+x,yf-y-1), BPoint(xf-t+x,yf-y-1));
					SetHighColor(mHighColor);
					StrokeLine(BPoint(xf-t+x+1,yf-y), BPoint(xf-t+x+1,yf-y));
				}
			}
			break;
		case BOTTOM_LEFT:		
			for(x = 1; x <= t; x+=3)
			{
				for(y = 1; y <= x; y+=3)
				{
					SetHighColor(mLowColor);
					StrokeLine(BPoint(t-x-1,yf-y-1), BPoint(t-x-1,yf-y-1));
					SetHighColor(mHighColor);
					StrokeLine(BPoint(t-x,yf-y), BPoint(t-x,yf-y));
				}
			}
			break;
		case TOP_RIGHT:
			for(x = 1; x <= t; x+=3)
			{
				for(y = 1; y <= x; y+=3)
				{
					SetHighColor(mLowColor);
					StrokeLine(BPoint(xf-t+x,y), BPoint(xf-t+x,y));
					SetHighColor(mHighColor);
					StrokeLine(BPoint(xf-t+x+1,y+1), BPoint(xf-t+x+1,y+1));
				}
			}
			break;
		case TOP_LEFT:
			for(x = 1; x <= t; x+=3)
			{
				for(y = 1; y <= x; y+=3)
				{
					SetHighColor(mLowColor);
					StrokeLine(BPoint(t-x-1,y), BPoint(t-x-1,y));
					SetHighColor(mHighColor);
					StrokeLine(BPoint(t-x,y+1), BPoint(t-x,y+1));
				}
			}
	}	// switch}
}