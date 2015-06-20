#ifndef NATIVE_FRAMEWORK_CONSOLEIMPL
#define NATIVE_FRAMEWORK_CONSOLEIMPL

#include "../includes/_NativeFrameworkConsole.h"

// to define the console color
#ifndef FRMWRK_BLK_COL
	#define FRMWRK_BLK_COL 0
#endif
#ifndef FRMWRK_BLU_COL
	#define FRMWRK_BLU_COL 1
#endif
#ifndef FRMWRK_GRN_COL
	#define FRMWRK_GRN_COL 2
#endif
#ifndef FRMWRK_AQU_COL
	#define FRMWRK_AQU_COL 3
#endif
#ifndef FRMWRK_RED_COL
	#define FRMWRK_RED_COL 4
#endif
#ifndef FRMWRK_PRP_COL
	#define FRMWRK_PRP_COL 5
#endif
#ifndef FRMWRK_YLW_COL
	#define FRMWRK_YLW_COL 6
#endif
#ifndef FRMWRK_SIL_COL
	#define FRMWRK_SIL_COL 7
#endif
#ifndef FRMWRK_GRY_COL
	#define FRMWRK_GRY_COL 8
#endif
#ifndef FRMWRK_LBL_COL
	#define FRMWRK_LBL_COL 9
#endif
#ifndef FMRWRK_LGR_COL
	#define FMRWRK_LGR_COL 'A'
#endif
#ifndef FRMWRK_LAQ_COL
	#define FRMWRK_LAQ_COL 'B'
#endif
#ifndef FRMWRK_LRD_COL
	#define FRMWRK_LRD_COL 'C'
#endif
#ifndef FRMWRK_LPR_COL
	#define FRMWRK_LPR_COL 'D'
#endif
#ifndef FRMWRK_LYL_COL
	#define FRMWRK_LYL_COL 'E'
#endif
#ifndef FRMWRK_WHT_COL
	#define FRMWRK_WHT_COL 'F'
#endif

// colors imlementation

string _NativeFrameworkConsoleColors::_getColorCode(__FrameworkColors requestedColor){
	ostringstream stringStream;
	switch(requestedColor){
		case Black: stringStream << FRMWRK_BLK_COL; break;
		case Blue: stringStream << FRMWRK_BLU_COL; break;
		case Green: stringStream << FRMWRK_GRN_COL; break;
		case Aqua: stringStream << FRMWRK_AQU_COL; break;
		case Red: stringStream << FRMWRK_RED_COL; break;
		case Purple: stringStream << FRMWRK_PRP_COL; break;
		case Yellow: stringStream << FRMWRK_LYL_COL; break;
		case Silver: stringStream << FRMWRK_SIL_COL; break;
		case Gray: stringStream << FRMWRK_GRY_COL; break;
		case LightBlue: stringStream << FRMWRK_LBL_COL; break;
		case LightGreen: stringStream << FMRWRK_LGR_COL; break;
		case LightRed: stringStream << FRMWRK_LRD_COL; break;
		case LightPurple: stringStream << FRMWRK_LPR_COL; break;
		case LightYellow: stringStream << FRMWRK_LYL_COL; break;
		case White: stringStream << FRMWRK_WHT_COL; break;
	}

	return stringStream.str();
}

char* _NativeFrameworkConsoleColors::_compileStatement(string backColor, string textColor){
	return (char*)(string("\nCOLOR " + backColor + textColor)).c_str();
}

/*implementing the console colors methods*/
void _NativeFrameworkConsoleColors::setColor(__FrameworkColors backColor, __FrameworkColors textColor){
	string backColorCode = _NativeFrameworkConsoleColors::_getColorCode(backColor);
	string textColorCode = _NativeFrameworkConsoleColors::_getColorCode(textColor);

	//cout << _NativeFrameworkConsoleColors::_compileStatement(backColorCode, textColorCode) << endl;
	// execute the command now
	//cout << _NativeFrameworkConsoleColors::_compileStatement(backColorCode, textColorCode) << endl;
	char* colorCommand = _NativeFrameworkConsoleColors::_compileStatement(backColorCode, textColorCode);
	system(colorCommand);
	//cout << endl;
}
void _NativeFrameworkConsoleColors::defaultColor(){
	system("\nCOLOR 0F\n");
}


/*
*	console implementation
*/

void _NativeFrameworkConsole::_readDefault(string &stringVal){
	cin >> stringVal;
	cout.flush();
}
void _NativeFrameworkConsole::_readDefault(char *stringVal){
	cin >> stringVal;
	cout.flush();
}
void _NativeFrameworkConsole::_readDefault(long &val){
	cin >> val;
	cout.flush();
}
void _NativeFrameworkConsole::_readDefault(double &dblValue){
	cin >> dblValue;
	cout.flush();
}
void _NativeFrameworkConsole::_readDefault(char &charVal){
	cin >> charVal;
	cout.flush();
}

// implementation
// print functions goes here
void _NativeFrameworkConsole::print(string printString){
	cout << printString.c_str();
}
void _NativeFrameworkConsole::print(char *printString){
	cout << printString;
}
void _NativeFrameworkConsole::print(long printLong){
	cout << printLong;
}
void _NativeFrameworkConsole::print(double printDouble){
	cout << printDouble;
}
void _NativeFrameworkConsole::print(char printChar){
	cout << printChar;
}
// v1.1 implementation
void _NativeFrameworkConsole::print(_NativeFrameworkObject object){
	cout << object.toString();
}
//print line functions go here
void _NativeFrameworkConsole::printLine(){
	cout << endl;
}
void _NativeFrameworkConsole::printLine(string printString){
	_NativeFrameworkConsole::print(printString);
	_NativeFrameworkConsole::printLine();
}
void _NativeFrameworkConsole::printLine(char *printString){
	_NativeFrameworkConsole::print(string(printString));
	_NativeFrameworkConsole::printLine();
}
void _NativeFrameworkConsole::printLine(long printLong){
	_NativeFrameworkConsole::print(printLong);
	_NativeFrameworkConsole::printLine();
}
void _NativeFrameworkConsole::printLine(double printDouble){
	_NativeFrameworkConsole::print(printDouble);
	_NativeFrameworkConsole::printLine();
}
void _NativeFrameworkConsole::printLine(char printChar){
	_NativeFrameworkConsole::print(printChar);
	_NativeFrameworkConsole::printLine();
}

void _NativeFrameworkConsole::printLine(_NativeFrameworkObject object){
	cout << object.toString() << endl;
}

// read functions impl. goes here
void _NativeFrameworkConsole::read(string &readString){
	_NativeFrameworkConsole::_readDefault(readString);
}
void _NativeFrameworkConsole::read(char *readString){
	_NativeFrameworkConsole::_readDefault(readString);
}
void _NativeFrameworkConsole::read(long &longValue){
	_NativeFrameworkConsole::_readDefault(longValue);
}
void _NativeFrameworkConsole::read(double &dblValue){
	_NativeFrameworkConsole::_readDefault(dblValue);
}
void _NativeFrameworkConsole::read(char &charValue){
	_NativeFrameworkConsole::_readDefault(charValue);
}

// dynamic read functions
long _NativeFrameworkConsole::readLong(){
	long num;
	_NativeFrameworkConsole::read(num);
	return num;
}
long _NativeFrameworkConsole::readNumber(){
	return readLong();
}
double _NativeFrameworkConsole::readDouble(){
	double dbl;
	_NativeFrameworkConsole::read(dbl);
	return dbl;
}
double _NativeFrameworkConsole::readDecimal(){
	return readDouble();
}
string _NativeFrameworkConsole::readString(){
	string str;
	_NativeFrameworkConsole::read(str);
	return str;
}
char* _NativeFrameworkConsole::readCString(){
	char *str;
	_NativeFrameworkConsole::read(str);
	return str;
}
char _NativeFrameworkConsole::readChar(){
	char ch;
	_NativeFrameworkConsole::read(ch);
	return ch;
}

// colors

void _NativeFrameworkConsole::setConsoleColor(__FrameworkColors backColor, __FrameworkColors textColor){
	_NativeFrameworkConsoleColors::setColor(backColor, textColor);
}
void _NativeFrameworkConsole::setDefaultColor(){
	_NativeFrameworkConsoleColors::defaultColor();
}


#endif