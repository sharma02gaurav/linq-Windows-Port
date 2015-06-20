#include "../includes/_NativeFrameworkConverter.h"


// string implementation
string _NativeFrameworkPrimitivesConverter::toString(int value){
	ostringstream _stream;
	_stream << value;
	return _stream.str();
}
string _NativeFrameworkPrimitivesConverter::toString(long value){
	ostringstream _stream;
	_stream << value;
	return _stream.str();
}
string _NativeFrameworkPrimitivesConverter::toString(double value){
	ostringstream _stream;
	_stream << value;
	return _stream.str();
}

// int implementation
int _NativeFrameworkPrimitivesConverter::toInt(string value){
	return atoi(value.c_str());
}
int _NativeFrameworkPrimitivesConverter::toInt(char* value){
	return atoi(value);
}
int _NativeFrameworkPrimitivesConverter::toInt(double value){
	return atoi((toString((int)value)).c_str());
}
// long implementation
long _NativeFrameworkPrimitivesConverter::toLong(string cppString){
	return (long)atol(cppString.c_str());
}
long _NativeFrameworkPrimitivesConverter::toLong(char *cString){
	return (long)atol(cString);
}
long _NativeFrameworkPrimitivesConverter::toNumber(string cppString){
	return toLong(cppString);
}
long _NativeFrameworkPrimitivesConverter::toNumber(char *cstring){
	return toLong(cstring);
}
// double implementation
double _NativeFrameworkPrimitivesConverter::toDouble(string cppString){
	return (double)atof(cppString.c_str());
}
double _NativeFrameworkPrimitivesConverter::toDouble(char *cString){
	return (double)atof(cString);
}
double _NativeFrameworkPrimitivesConverter::toDouble(long number){
	return (double)number;
}
double _NativeFrameworkPrimitivesConverter::toDecimal(string cppString){
	return toDouble(cppString);
}
double _NativeFrameworkPrimitivesConverter::toDecimal(char *cString){
	return toDouble(cString);
}
double _NativeFrameworkPrimitivesConverter::toDecimal(long number){
	return toDouble(number);
}