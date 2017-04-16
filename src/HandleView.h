#ifndef HANDLEVIEW_H
#define HANDLEVIEW_H


#include <View.h>

/*
	Intended for putting that cute resize icon
	inside windows
*/

enum
{
	FILL,
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT
};

enum 
{
	RESIZE,
	MOVE
};

#define LEFT_RIGHT 1
#define UP_DOWN 3


class CHandleView : public BView
{
public:
						CHandleView(BRect, uint32, bool resize = true, uint32 pattern = FILL, uint32 movement = LEFT_RIGHT + UP_DOWN);
						~CHandleView();
	void				Draw(BRect);
	void				AttachedToWindow();
	void				MouseDown(BPoint);
	
private:

	rgb_color			mHighColor;
	rgb_color			mLowColor;
	BPoint				mMouseDown;
	bool				mResize;			// resize or move				
	uint32				mMovement;			// constraint
	uint32				mPattern;			// handlepattern
	BPicture			*mPicture;
};

#endif
