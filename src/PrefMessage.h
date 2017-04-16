#ifndef _PREF_H_
#define _PREF_H_

#include <Message.h>
#include <Path.h>
#include <Application.h>
#include <Roster.h>
#include <string>
#include <String.h>

class CPrefMessage : public BMessage
{
	public:
							CPrefMessage( const char *filename = "pref" );
		bool				Load();
		bool				Save();

	private:
			
		status_t			CheckDirectory();

		BPath				_path;
		BString				_signature;
		app_info			_appinfo;
		BString				_filename;		
};

#endif
// /boot/develop/headers/be/app/Roster.h