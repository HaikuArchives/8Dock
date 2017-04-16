#ifndef ITEMLISTVIEW_H
#define ITEMLISTVIEW_H


#include <View.h>
#include <List.h>
#include "ItemListView.h"
#include "PanelView.h"
#include "ItemView.h"
#include "PrefMessage.h"

#define BIG_SIZE 40
#define SMALL_SIZE 20

#define SLOT_COUNT 64

class CItemListView : public BView
{
public:
						CItemListView(BRect);
						~CItemListView();
	void				AttachedToWindow(void);
	void				MouseDown(BPoint);
	void				SetLayout( bool, bool );
	void				InitViews( bool, bool );
	void				Remove( int32 );
	void				Save(int32, const char * );
	void				EmptySlot(int32);

	void				MessageReceived( BMessage * );

	int32				FindByPath(BPath &);

	BList				mItemList;


private:

	BPoint				mSlotSize;
	CItemView			*mLastView;

	CPrefMessage		*mSettings;
	
	CItemListView		*mvItem;
	BPoint				mMouseDown;
		
	int32				mCount;

	int32				mWidth;
	rgb_color			mHighColor;
	rgb_color			mLowColor;

};

#endif
