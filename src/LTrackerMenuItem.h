//
// LTrackerMenuItem.h
//

#ifndef _LTRACKERMENUITEM_H_
#define _LTRACKERMENUITEM_H_

#include <MenuItem.h>
#include <Bitmap.h>
#include "LTrackerMenu.h"

class LTrackerMenuItem : public BMenuItem
{
	public:
							LTrackerMenuItem(	const entry_ref*	ref,
												BMessage*			message,
												char				shortcut = 0,
												uint32				modifiers = 0 );
							LTrackerMenuItem(	const entry_ref*	ref,
												BMenu*				subMenu,
												BMessage*			message = NULL );
							LTrackerMenuItem(	BMessage*	data );
		virtual				~LTrackerMenuItem();

	protected:
		virtual void		GetContentSize( float* width, float* height );
		virtual void		DrawContent( void );
		virtual void		Highlight( bool flag );

	private:
		void				InitObject( const entry_ref* ref );

		BBitmap*			mMiniIcon;
};

#endif
