#ifndef PANELVIEW_H
#define PANELVIEW_H


#include <View.h>

//BRect frame, const char *name, uint32 resizingMode, uint32 flags

class CPanelView : public BView
{
public:
						CPanelView( BRect, const char*, uint32, uint32, bool raised = true );
	virtual void		Draw( BRect );
	virtual void		AttachedToWindow( void );
	virtual void		SetViewColor( rgb_color );
	void				SetViewColor( uchar r, uchar g, uchar b, uchar a = 0 );
	rgb_color			GetHighColor();
	rgb_color			GetLowColor();
	void				DrawBorder( BRect );
//	void				FrameResized( int32, int32 );
	
private:
	BRect 				mBounds;	
	static const int 	c_border_width = 4;

protected:
	bool				mIsRaised;
	rgb_color			mHighColor;
	rgb_color			mLowColor;
};

inline void	CPanelView::SetViewColor( uchar r, uchar g, uchar b, uchar a )
{
	rgb_color	a_color;
	a_color.red = r;		a_color.green = g;
	a_color.blue = b;		a_color.alpha = a;
	SetViewColor(a_color);
}

#endif
