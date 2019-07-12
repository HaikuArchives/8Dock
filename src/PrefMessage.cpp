#include "PrefMessage.h"
#include <FindDirectory.h>
#include <Application.h>
#include <Alert.h>
#include <stdio.h>
#include <DataIO.h>
#include <Directory.h>
#include <File.h>

CPrefMessage::CPrefMessage( const char *filename )
	: BMessage( 'pref' )
{
	find_directory( B_USER_CONFIG_DIRECTORY, &_path );
	_filename = filename;
	
	if( be_app->GetAppInfo( &_appinfo ) != B_OK )
	{
		BAlert *alert = new BAlert( "EH?", "Could not read applicaton signature, preferences will not be saved", "Go Away" );
		alert->Go( NULL );
	}
	else
	{
		_signature = _appinfo.signature;
	} 
	AddString( "Signature", _signature.String() );
}

bool CPrefMessage::Load()
{
	CheckDirectory();
	int32 p = _signature.FindFirst("/"); 
	BString filename = BString( _path.Path() );
	filename.Append( "/settings" );		// Where is the operator+?
	filename.Append( (_signature.String())+p );
	filename.Append( "/" );
	filename.Append( _filename );

	BFile preffile( filename.String(), B_READ_ONLY );
	Unflatten( &preffile );

//	printf(" load:\n" ); 	PrintToStream();

	return true;
}

bool CPrefMessage::Save()
{
	CheckDirectory();
	int32 p = _signature.FindFirst("/"); 
	BString filename = BString( _path.Path() );
	filename.Append( "/settings" );		// Where is the operator+?
	filename.Append( (_signature.String())+p );
	filename.Append( "/" );
	filename.Append( _filename );

	BFile preffile( filename.String(), B_READ_WRITE | B_CREATE_FILE );
	Flatten( &preffile );

//	printf(" save:\n" );	PrintToStream();
	
	return true;
}

status_t CPrefMessage::CheckDirectory()
{
	int32 p = _signature.FindFirst("/"); 
	BString fn = BString( _path.Path() );
	fn.Append( "/settings" );
	fn.Append( (_signature.String())+p );
	BDirectory d,e;
	status_t st = d.SetTo( fn.String() );
	if( st == B_ENTRY_NOT_FOUND );
	{
		BString f( _path.Path() );
		f.Append( "/settings" );

		d.SetTo( f.String() );
		st = d.CreateDirectory( _signature.String()+p+1 , &e );
		
	}
	return st;
}
