#ifndef ITEMVIEW_H
#define ITEMVIEW_H

#include "PanelView.h"
#include <Node.h>
#include <NodeInfo.h>
#include <Path.h>
#include <PopUpMenu.h>

class CItemListView;

class CItemView : public CPanelView
{
public:
						CItemView( BRect rect, int32, CItemListView *iv, bool bLbl = true, bool bBigI = false );
						~CItemView();
	void				AttachedToWindow( void) ;
	void				Draw( BRect );
	void				MouseMoved( BPoint, uint32, const BMessage * );
	void				MouseDown( BPoint );
	void				Empty();
	void				FromTracker( BMessage * );
	void				EntryMoved( BMessage * );
	void 				MessageReceived( BMessage * );
	void				SetData( BPath & );
	void	 			SetData();
	void				SetLayout( bool, bool );
	
	char				*mName;	
	bool				bIsEmpty;
	BPath				mPath;

	BMenuItem			*mi;
	int32				mDockPos;

private:

	bool				OverIcon( BPoint );

	bool				bLabel;		
	bool				bBigIcon;	
	
	int32				mySize;
	float				mStringWidth;
	
	rgb_color			mrgbHigh;
	rgb_color			mrgbLow;
	BBitmap				*mSmallIcon;
	BBitmap				*mLargeIcon;
	int64				db;
	
	bool				bDrawBorder;
	bool				bMouseDown;
	bool				bWatching;

	CItemListView		*mItemListView;
	BNode				mNode;
	BNodeInfo			mNodeInfo;
	entry_ref			mRef;
	BRect				mRect;
	BMessage			*msgTracker;
};

#endif
