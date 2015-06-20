#pragma once

#include <string>
#include <sstream>

using namespace std;
/*
*	every class in linq API extends this class and it provides the basic function
*	that almost every class must implement in order to utilize the beauty
*	and power for linq framework.
*/
class _NativeFrameworkObjectFacade{
	private:
	public:
		// convert the object to string..normally to get value
		virtual string toString() = 0;
};
class _NativeFrameworkObject : public _NativeFrameworkObjectFacade{
	private:

		enum ACTIVATED{NUMBER, DECIMAL, STR, CHAR};
		long number;
		double decimal;
		string str;
		char ch;
		ACTIVATED activated;
	
	public:
		_NativeFrameworkObject();
		_NativeFrameworkObject(int);
		_NativeFrameworkObject(long);
		_NativeFrameworkObject(double);
		_NativeFrameworkObject(string);
		_NativeFrameworkObject(char*);
		_NativeFrameworkObject(char);
		
		string toString();
};

_NativeFrameworkObject::_NativeFrameworkObject(){
	activated = STR;
	str = "";
}
_NativeFrameworkObject::_NativeFrameworkObject(int number){
	activated = NUMBER;
	this -> number = (long)number;
}
_NativeFrameworkObject::_NativeFrameworkObject(long number){
	activated = NUMBER;
	this -> number = number;
} 
_NativeFrameworkObject::_NativeFrameworkObject(double decimal){
	activated = DECIMAL;
	this -> decimal = decimal;
}
_NativeFrameworkObject::_NativeFrameworkObject(string str){
	activated = STR;
	this -> str = str;
}
_NativeFrameworkObject::_NativeFrameworkObject(char *str){
	activated = STR;
	this -> str = str;
}
_NativeFrameworkObject::_NativeFrameworkObject(char ch){
	activated = CHAR;
	this -> ch = ch;
}
string _NativeFrameworkObject::toString(){
	ostringstream oss;
	switch(activated){
		case NUMBER: oss << number; break;
		case DECIMAL: oss << decimal; break;
		case STR: oss << str; break;
		case CHAR: oss<< ch; break;
	}
	return oss.str();
}