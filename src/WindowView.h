#ifndef WINDOWVIEW_H
#define WINDOWVIEW_H


#include <View.h>
#include "HandleView.h"
#include "TrashView.h"
#include "ItemListView.h"
#include "PanelView.h"

class CWindowView : public CPanelView
{
public:
						CWindowView( BRect, bool, bool, bool );
	void				Draw( BRect );
	void				SetWindowWidth( uint32 );
	void 				AttachedToWindow( void );
	
private:

	bool				bLabel;
	bool				bBig;
	
	BRect 				rect;
	CHandleView			*mResizeView;	
	CHandleView			*mMoveView;	
	CTrashView			*mTrashView;
	CItemListView		*mItemListView;
	uint32				iWindowWidth;
	char				message[128];
};

#endif
