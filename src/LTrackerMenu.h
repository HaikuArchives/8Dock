//
// LTrackerMenu.h
//

#ifndef _LTRACKERMENU_H_
#define _LTRACKERMENU_H_

#include <Menu.h>
#include <Entry.h>

class LTrackerMenu : public BMenu
{
	public:
							LTrackerMenu(	entry_ref*			ref,
											menu_layout			layout = B_ITEMS_IN_COLUMN);
		virtual				~LTrackerMenu();
		void				ScanFolder( void );
		const bool			IsValid( void ) const
								{ return mIsValid; }

	protected:

	private:
		bool				mIsValid;
		entry_ref			mRef;
};

#endif
