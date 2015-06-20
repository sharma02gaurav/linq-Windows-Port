#pragma once

/*
*	this header contains the network related classes
*/

#ifndef EXCEPTION_DEFS
	#define EXCEPTION_DEFS
	
	#define __ERRFILE __FILE__
	#define __ERRFUNC __func__
	#define __ERRLINE __LINE__
#endif

#ifdef _WIN32
	#define __FILESEP '\\'
	#define __UNIXSEP '/'
#else
	#define __FILESEP '/'
	#define __WINSEP '\\'
#endif

#include <iostream>
#include <string>
#include <string.h>

using namespace std;

/*
*	this class provides a way to manage the URLs in an Application. The URL could
*	follow any format (UNIX or Windows). This class automatically converts the URLs
*	as per the OS being employed.
*/
class _NativeFrameworkURL{
	private:
		string url;
	public:
		_NativeFrameworkURL(string);
		_NativeFrameworkURL(char*);
		/*
		*	returns the refactored URL as per the OS.
		*/
		string getURL();
};

_NativeFrameworkURL::_NativeFrameworkURL(string url){
	this -> url = url;
}
_NativeFrameworkURL::_NativeFrameworkURL(char *url){
	this -> url = url;
}

string _NativeFrameworkURL::getURL(){
	#ifdef _WIN32
		if(url.find(__UNIXSEP) != -1){
			char indChar[url.length()];
			strcpy(indChar, url.c_str());
			int ctr = 0;
			while(indChar[ctr] != '\0'){
				if(indChar[ctr] == __UNIXSEP)
					indChar[ctr] = __FILESEP;
				ctr+=1;
			}
			return string((char*)indChar);
		}
		return url;
	#else
		if(url.find(__WINSEP) != -1){
			char indChar[url.length()];
			strcpy(indChar, url.c_str());
			int ctr = 0;
			while(indChar[ctr] != '\0'){
				if(indChar[ctr] == __WINSEP)
					indChar[ctr] = __FILESEP;
				ctr+=1;
			}
			return string((char*)indChar);
		}
		return url;
	#endif
}