#ifndef TRASHVIEW_H
#define TRASHVIEW_H


#include <Entry.h>
#include <View.h>

class CTrashView : public BView
{
public:
					CTrashView(BPoint);
					~CTrashView();
	void				Draw(BRect);
	void				AttachedToWindow(void);
	void				MouseDown(BPoint p);
	void				MessageReceived(BMessage *);
	void				MoveToTrash(BMessage*);
	void				MouseMoved(BPoint, uint32, const BMessage*);
	void				EmptyDir(BDirectory *);
	
private:

	void				OpenTracker();

	BDirectory		*mTrashDir;

	bool	 		menuok;
	BMenuItem		*mi;
	BPopUpMenu		*menu;
	bool			bHighlight;
	int64			db;
	BBitmap			*iconS;
	BBitmap			*iconL;
	BEntry			entry;
	entry_ref			mRef;
	uchar 			mHiliteTable[256]; 
	const color_map 		*systemColors; 
};

#endif
