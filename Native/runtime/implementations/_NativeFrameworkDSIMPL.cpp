#ifndef NATIVE_FRAMEWORK_DSIMPL
#define NATIVE_FRAMEWORK_DSIMPL

#include "../includes/_NativeFrameworkDS.h"

_NativeFrameworkDSException::_NativeFrameworkDSException(string errorMessage,
	string filename,
	string function,
	int line){

	this -> errorMessage = errorMessage;
	this -> filename = filename;
	this -> function = function;
	this -> line = line;

	_printError();
}

string _NativeFrameworkDSException::toString(){
	return errorMessage;
}

/*_NativeFrameworkDSString implementation*/
_NativeFrameworkDSString::_NativeFrameworkDSString(char* _strValue) : variableString(static_cast<string>(_strValue)){}
_NativeFrameworkDSString::_NativeFrameworkDSString(string _strValue) : variableString(_strValue){}
_NativeFrameworkDSString::_NativeFrameworkDSString(const _NativeFrameworkDSString &_strValue) : variableString(_strValue.getString()){
}
_NativeFrameworkDSString::_NativeFrameworkDSString() : variableString(static_cast<string>("")){}

int _NativeFrameworkDSString::length(){
	return variableString.length();
}
char _NativeFrameworkDSString::charAt(int index){
	if(index >= length())
		throw _DSException("Index out of bounds.");
	return variableString.at(index);
}
int _NativeFrameworkDSString::firstIndexOf(char character){
	return variableString.find(character);
}
int _NativeFrameworkDSString::firstIndexOf(char *lookString){
	return variableString.find(lookString);
}
int _NativeFrameworkDSString::firstIndexOf(string lookString){
	return variableString.find(lookString.c_str());
}
int _NativeFrameworkDSString::firstIndexOf(_NativeFrameworkDSString lookString){
	return variableString.find(lookString.getCString());
}
// V1.1 IMPL
int _NativeFrameworkDSString::indexOf(char ch){
	return firstIndexOf(ch);
}
int _NativeFrameworkDSString::indexOf(char *ch){
	return firstIndexOf(ch);
}
int _NativeFrameworkDSString::indexOf(string ch){
	return firstIndexOf(ch);
}
int _NativeFrameworkDSString::indexOf(_NativeFrameworkDSString ch){
	return firstIndexOf(ch);
}

int _NativeFrameworkDSString::lastIndexOf(char character){
	return variableString.rfind(character);
}
int _NativeFrameworkDSString::lastIndexOf(char *lookString){
	return variableString.rfind(lookString);
}
int _NativeFrameworkDSString::lastIndexOf(string lookString){
	return variableString.rfind(lookString.c_str());
}
int _NativeFrameworkDSString::lastIndexOf(_NativeFrameworkDSString lookString){
	return variableString.rfind(lookString.getCString());
}
char* _NativeFrameworkDSString::substring(int inset, int offset){
	if(inset < 0 || offset > length())
		throw _DSException("Index out of bounds.");
	return (char*)(variableString.substr(inset, offset)).c_str();
}
char* _NativeFrameworkDSString::substring(int offset){
	if(offset > length())
		throw _DSException("Index out of bounds.");
	return substring(0, offset);
}

// string.compare(string) function return 0 if strings are equal
bool _NativeFrameworkDSString::equalsTo(char *cmpString){
	if(! variableString.compare(cmpString)) return true;
	return false;
}
bool _NativeFrameworkDSString::equalsTo(string cmpString){
	return equalsTo(cmpString.c_str());
}
bool _NativeFrameworkDSString::equalsTo(_NativeFrameworkDSString cmpString){
	return equalsTo(cmpString.getCString());
}
bool _NativeFrameworkDSString::equalsIgnoringCase(char *cmpString){
	_NativeFrameworkDSString _tmpInstance = getString();
	_tmpInstance.toUpperCase();
	_NativeFrameworkDSString _tempInstance2 = cmpString;
	_tempInstance2.toUpperCase();

	return _tmpInstance.equalsTo(_tempInstance2);
}
bool _NativeFrameworkDSString::equalsIgnoringCase(string cmpString){
	return equalsIgnoringCase(cmpString.c_str());
}
bool _NativeFrameworkDSString::equalsIgnoringCase(_NativeFrameworkDSString cmpString){
	return equalsIgnoringCase(cmpString.getCString());
}
bool _NativeFrameworkDSString::isEmpty(){
	if (length() <= 0 || equalsTo("")) return true;
	return false;
}
bool _NativeFrameworkDSString::isWhitespaceOnly(){
	if(isEmpty()) return true;
	for(int i=0; i<length(); i++)
		if(charAt(i) != 9)
			return false;
	return true;
}
/*	
*	TEST THIS FUNCTION
*/
char* _NativeFrameworkDSString::getReverse(){
	char ind_char[length()];
	char gen_string[length()];
	strcpy(ind_char, getCString());
	int gen_ctr = 0;
	int i=length();
	
	while(i>=1)
		gen_string[gen_ctr++] = ind_char[--i];

	gen_string[gen_ctr] = '\0';
	string finalString = gen_string;
	return (char*)finalString.c_str();
}
bool _NativeFrameworkDSString::isPalindrome(){
	if(equalsTo(getReverse())) return true;
	return false;
}

char* _NativeFrameworkDSString::toUpperCase(){
	for_each(
		variableString.begin(),
		variableString.end(),
		Upper());
	return (char*)variableString.c_str();
}
char* _NativeFrameworkDSString::toUppercase(){
	return toUppercase();
}
char* _NativeFrameworkDSString::toLowerCase(){
	for_each(
		variableString.begin(),
		variableString.end(),
		Lower());

	return (char*)variableString.c_str();
}
char* _NativeFrameworkDSString::toLowercase(){
	return toLowerCase();
}

int _NativeFrameworkDSString::countOccurance(char character){
	char ind_char[length()];
	strcpy(ind_char, getCString());
	int navigator = 0;
	int char_counter = 0;

	while(ind_char[navigator] != '\0')
		if(ind_char[navigator++] == character)
			++char_counter;

	return char_counter;
}

bool _NativeFrameworkDSString::contains(char character){
	if(firstIndexOf(character) != -1) return true;
	return false;
}
bool _NativeFrameworkDSString::contains(char *testString){
	if(firstIndexOf(testString) != -1) return true;
	return false;
}
bool _NativeFrameworkDSString::contains(string testString){
	if(firstIndexOf(testString.c_str()) != -1) return true;
	return false;
}
bool _NativeFrameworkDSString::contains(_NativeFrameworkDSString testString){
	if(firstIndexOf(testString) != -1) return true;
	return false;
}
void _NativeFrameworkDSString::replace(char character, char withChar){
	char ind_char[length()];
	strcpy(ind_char, getCString());
	int ctr = 0;
	while(ind_char[ctr++] != '\0'){
		if(ind_char[ctr] == character)
			ind_char[ctr] = withChar;
	}
	variableString = string((char*)ind_char);
}
void _NativeFrameworkDSString::replace(char *_string, char *withString){
	while(contains(_string)){
		int startIndex = firstIndexOf(_string);
		int endIndex = startIndex + ((string(_string)).length() -1);

		string firstPart = substring(0, startIndex);
		string secondPart = substring(endIndex+1, length());

		variableString = firstPart + string(withString) + secondPart;
	}
}
void _NativeFrameworkDSString::replace(string _string, string withString){
	replace(_string.c_str(), withString.c_str());
}
void _NativeFrameworkDSString::replace(_NativeFrameworkDSString _string, _NativeFrameworkDSString withString){
	replace(_string.getCString(), withString.getCString());
}
bool _NativeFrameworkDSString::startWith(char character){
	if(charAt(0) == character) return true;
	return false;
}
bool _NativeFrameworkDSString::startWith(string cppString){
	char indChar[cppString.length()];
	strcpy(indChar, cppString.c_str());
	bool match = true;
	int ctr = 0;
	while(cppString[ctr] != '\0' && match == true){
		if(cppString[ctr] != charAt(ctr++))
			match = false;
	}
	if(match)
		return true;
	return false;
}
bool _NativeFrameworkDSString::startWith(char *cString){
	return startWith(string(cString));
}
bool _NativeFrameworkDSString::startWith(_NativeFrameworkDSString stringInstance){
	return startWith(stringInstance.getCString());
}

bool _NativeFrameworkDSString::startsWith(char character){
	return startWith(character);
}
bool _NativeFrameworkDSString::startsWith(string cppString){
	return startWith(cppString);
}
bool _NativeFrameworkDSString::startsWith(char *cString){
	return startWith(string(cString));
}
bool _NativeFrameworkDSString::startsWith(_NativeFrameworkDSString stringInstance){
	return startWith(stringInstance.getCString());
}
bool _NativeFrameworkDSString::endWith(char character){
	if(charAt(length()-1) == character) return true;
	return false;
}

bool _NativeFrameworkDSString::endWith(string cppString){
	// the last of the string equals to tge string passed to check
	string thisEndString = substring(length()-cppString.length(), length());
	if(cppString == thisEndString)
		return true;
	return false;
}
bool _NativeFrameworkDSString::endWith(char *cppString){
	return endWith(string(cppString));
}
bool _NativeFrameworkDSString::endWith(_NativeFrameworkDSString stringInstance){
	return endWith(stringInstance.getString());
}

bool _NativeFrameworkDSString::endsWith(char character){
	return endWith(character);
}

bool _NativeFrameworkDSString::endsWith(string cppString){
	return endWith(cppString);
}
bool _NativeFrameworkDSString::endsWith(char *cppString){
	return endWith(string(cppString));
}
bool _NativeFrameworkDSString::endsWith(_NativeFrameworkDSString stringInstance){
	return endWith(stringInstance.getString());
}

string _NativeFrameworkDSString::getString() const{
	return variableString;
}
char* _NativeFrameworkDSString::getCString() const{
	return (char*)(variableString.c_str());
}

_NativeFrameworkDSString _NativeFrameworkDSString::operator+(_NativeFrameworkDSString lsInstance){
	return _NativeFrameworkDSString(getString() + lsInstance.getString());
}
void _NativeFrameworkDSString::setString(string cppString){
	variableString = cppString;
}
void _NativeFrameworkDSString::setString(char *cString){
	variableString = cString;
}
void _NativeFrameworkDSString::setString(_NativeFrameworkDSString stringInstance){
	variableString = stringInstance.getString();
}
// operators
_NativeFrameworkDSString _NativeFrameworkDSString::operator+=(_NativeFrameworkDSString lsInstance){
	variableString += lsInstance.getString();
	return _NativeFrameworkDSString(getString());
}

/*_NativeFrameworkDSString _NativeFrameworkDSString::operator+(char append){
	string tostr = LinqConverter::toString(append);
	_NativeFrameworkDSString val = getString() + tostr;
	return val;
}
void _NativeFrameworkDSString::operator+=(char append){
	variableString += LinqConverter::toString(append);
}*/
_NativeFrameworkDSString _NativeFrameworkDSString::operator+(char *_string){
	return _NativeFrameworkDSString(getString() + string(_string));
}
_NativeFrameworkDSString _NativeFrameworkDSString::operator+=(char *_string){
	variableString += string(_string);
	return _NativeFrameworkDSString(getString());
}
_NativeFrameworkDSString _NativeFrameworkDSString::operator+(string _string){
	return _NativeFrameworkDSString(getString() + _string);
}
_NativeFrameworkDSString _NativeFrameworkDSString::operator+=(string _string){
	variableString += _string;
	return _NativeFrameworkDSString(getString());
}

// primitive native other types

_NativeFrameworkDSString _NativeFrameworkDSString::operator+(int appendInt){
	string returnString = _NativeFrameworkDSString::getString();
	
	ostringstream tempStream;
		tempStream << appendInt;
	returnString += tempStream.str();
	return _NativeFrameworkDSString(returnString);
}

_NativeFrameworkDSString _NativeFrameworkDSString::operator+=(int appendInt){
	string newString = _NativeFrameworkDSString::getString();

	ostringstream tempStream;
		tempStream << appendInt;

	variableString += tempStream.str();
	return _NativeFrameworkDSString(getString());
}

_NativeFrameworkDSString _NativeFrameworkDSString::operator+(long appendLong){
	string returnString = _NativeFrameworkDSString::getString();
	
	ostringstream tempStream;
		tempStream << appendLong;
	returnString += tempStream.str();
	return _NativeFrameworkDSString(returnString);
}

_NativeFrameworkDSString _NativeFrameworkDSString::operator+=(long appendLong){
	string newString = _NativeFrameworkDSString::getString();

	ostringstream tempStream;
		tempStream << appendLong;

	variableString += tempStream.str();
	return _NativeFrameworkDSString(getString());
}

_NativeFrameworkDSString _NativeFrameworkDSString::operator+(double appendDouble){
	string returnString = _NativeFrameworkDSString::getString();

	ostringstream tempStream;
		tempStream << appendDouble;

	returnString += tempStream.str();

	return _NativeFrameworkDSString(returnString);
}
_NativeFrameworkDSString _NativeFrameworkDSString::operator+=(double appendDouble){
	string newString = _NativeFrameworkDSString::getString();
	
	ostringstream tempStream;
		tempStream << appendDouble;
	variableString += tempStream.str();
	return _NativeFrameworkDSString(getString());
}

// real-world override
_NativeFrameworkDSString _NativeFrameworkDSString::operator*(int times){
	string genString = "";
	for(int i=0; i< times; i+=1){
		genString += getString();
		if(i != times)
			genString += " ";
	}
	return _NativeFrameworkDSString(genString);
}
_NativeFrameworkDSString _NativeFrameworkDSString::operator*=(int times){
	string genString = "";
	for(int i=0; i< times; i+=1){
		genString += getString();
		if(i != times)
			genString += " ";
	}
	variableString = genString;
	return _NativeFrameworkDSString(getString());
}
_NativeFrameworkDSString _NativeFrameworkDSString::operator*(long times){
	return _NativeFrameworkDSString(getString())*((int)times);
}
_NativeFrameworkDSString _NativeFrameworkDSString::operator*=(long times){
	setString(_NativeFrameworkDSString(getString())*((int)times));
	return _NativeFrameworkDSString(getString());
}
/*
*	equality operators impl.
*/
bool _NativeFrameworkDSString::operator==(_NativeFrameworkDSString stringInstance){
	return equalsTo(stringInstance);
}
bool _NativeFrameworkDSString::operator==(char *c_string){
	return equalsTo(c_string);
}
bool _NativeFrameworkDSString::operator==(string cppString){
	return equalsTo(cppString);
}
bool _NativeFrameworkDSString::operator!=(_NativeFrameworkDSString stringInstance){
	return equalsTo(stringInstance);
}
bool _NativeFrameworkDSString::operator!=(char *cString){
	return equalsTo(cString);
}
bool _NativeFrameworkDSString::operator!=(string cppString){
	return equalsTo(cppString);
}
/*
*	Native Framework DSNumber Implementation
*/
_NativeFrameworkDSNumber::_NativeFrameworkDSNumber() : number(static_cast<long>(0)){
}
_NativeFrameworkDSNumber::_NativeFrameworkDSNumber(int _number): number(static_cast<long>(_number)){
}
_NativeFrameworkDSNumber::_NativeFrameworkDSNumber(long _number) : number(static_cast<long>(_number)){
}
// valid methods
long _NativeFrameworkDSNumber::getNumber(){
	return number;
}
string _NativeFrameworkDSNumber::toString(){
	ostringstream oss;
	oss << number;
	return string(oss.str());
}
//operator overloading impl.
_NativeFrameworkDSNumber _NativeFrameworkDSNumber::operator+(_NativeFrameworkDSNumber selfInstance){
	return _NativeFrameworkDSNumber(getNumber() + selfInstance.getNumber());
}
_NativeFrameworkDSNumber _NativeFrameworkDSNumber::operator-(_NativeFrameworkDSNumber selfInstance){
	return _NativeFrameworkDSNumber(getNumber() - selfInstance.getNumber());
}
_NativeFrameworkDSNumber _NativeFrameworkDSNumber::operator*(_NativeFrameworkDSNumber selfInstance){
	return _NativeFrameworkDSNumber(getNumber() * selfInstance.getNumber());
}
_NativeFrameworkDSNumber _NativeFrameworkDSNumber::operator/(_NativeFrameworkDSNumber selfInstance){
	return _NativeFrameworkDSNumber(getNumber() / selfInstance.getNumber());
}
_NativeFrameworkDSNumber _NativeFrameworkDSNumber::operator%(_NativeFrameworkDSNumber selfInstance){
	return _NativeFrameworkDSNumber(getNumber() % selfInstance.getNumber());
}
void _NativeFrameworkDSNumber::operator+=(_NativeFrameworkDSNumber selfInstance){
	this -> number += selfInstance.getNumber();
}
void _NativeFrameworkDSNumber::operator-=(_NativeFrameworkDSNumber selfInstance){
	this -> number -= selfInstance.getNumber();
}
void _NativeFrameworkDSNumber::operator*=(_NativeFrameworkDSNumber selfInstance){
	this -> number *= selfInstance.getNumber();
}
void _NativeFrameworkDSNumber::operator/=(_NativeFrameworkDSNumber selfInstance){
	this -> number /= selfInstance.getNumber();
}
_NativeFrameworkDSNumber _NativeFrameworkDSNumber::operator++(){
	this -> number += 1;
	return _NativeFrameworkDSNumber(this -> number);
}
_NativeFrameworkDSNumber _NativeFrameworkDSNumber::operator++(int num){
	this -> number += 1;
	return _NativeFrameworkDSNumber(this -> number);
}
_NativeFrameworkDSNumber _NativeFrameworkDSNumber::operator--(){
	this -> number -= 1;
	return _NativeFrameworkDSNumber(this -> number);
}
_NativeFrameworkDSNumber _NativeFrameworkDSNumber::operator--(int num){
	this -> number -=1;
	return _NativeFrameworkDSNumber(this -> number);
}

_NativeFrameworkDSNumber _NativeFrameworkDSNumber::operator+(long value){
	return _NativeFrameworkDSNumber(getNumber() + value);
}
_NativeFrameworkDSNumber _NativeFrameworkDSNumber::operator-(long value){
	return _NativeFrameworkDSNumber(getNumber() - value);
}
_NativeFrameworkDSNumber _NativeFrameworkDSNumber::operator*(long value){
	return _NativeFrameworkDSNumber(getNumber() * value);
}
_NativeFrameworkDSNumber _NativeFrameworkDSNumber::operator/(long value){
	return _NativeFrameworkDSNumber(getNumber() / value);
}
_NativeFrameworkDSNumber _NativeFrameworkDSNumber::operator%(long value){
	return _NativeFrameworkDSNumber(getNumber() % value);
}
void _NativeFrameworkDSNumber::operator+=(long value){
	this -> number += value;
}
void _NativeFrameworkDSNumber::operator-=(long value){
	this -> number -= value;
}
void _NativeFrameworkDSNumber::operator*=(long value){
	this -> number *= value;
}
void _NativeFrameworkDSNumber::operator/=(long value){
	this -> number /= value;
}


// override int
_NativeFrameworkDSNumber _NativeFrameworkDSNumber::operator+(int value){
	return _NativeFrameworkDSNumber(getNumber() + value);
}
_NativeFrameworkDSNumber _NativeFrameworkDSNumber::operator-(int value){
	return _NativeFrameworkDSNumber(getNumber() - value);
}
_NativeFrameworkDSNumber _NativeFrameworkDSNumber::operator*(int value){
	return _NativeFrameworkDSNumber(getNumber() * value);
}
_NativeFrameworkDSNumber _NativeFrameworkDSNumber::operator/(int value){
	return _NativeFrameworkDSNumber(getNumber() / value);
}
_NativeFrameworkDSNumber _NativeFrameworkDSNumber::operator%(int value){
	return _NativeFrameworkDSNumber(getNumber() % value);
}
void _NativeFrameworkDSNumber::operator+=(int value){
	this -> number += value;
}
void _NativeFrameworkDSNumber::operator-=(int value){
	this -> number -= value;
}
void _NativeFrameworkDSNumber::operator*=(int value){
	this -> number *= value;
}
void _NativeFrameworkDSNumber::operator/=(int value){
	this -> number /= value;
}

_NativeFrameworkDSNumber _NativeFrameworkDSNumber::operator+(double value){
	return _NativeFrameworkDSNumber((long)(getNumber() + value));
}
_NativeFrameworkDSNumber _NativeFrameworkDSNumber::operator-(double value){
	return _NativeFrameworkDSNumber((long)(getNumber() - value));
}
_NativeFrameworkDSNumber _NativeFrameworkDSNumber::operator*(double value){
	return _NativeFrameworkDSNumber((long)(getNumber() * value));
}
_NativeFrameworkDSNumber _NativeFrameworkDSNumber::operator/(double value){
	return _NativeFrameworkDSNumber((long)(getNumber() / value));
}
void _NativeFrameworkDSNumber::operator+=(double value){
	this -> number += value;
}
void _NativeFrameworkDSNumber::operator-=(double value){
	this -> number -= value;
}
void _NativeFrameworkDSNumber::operator*=(double value){
	this -> number *= value;
}
void _NativeFrameworkDSNumber::operator/=(double value){
	this -> number /= value;
}

// string append -> V1.1 

_NativeFrameworkDSString _NativeFrameworkDSNumber::operator+(const char *cString){
	ostringstream out;
	out << getNumber();

	return _NativeFrameworkDSString(out.str() + string(cString));
}
_NativeFrameworkDSString _NativeFrameworkDSNumber::operator+(_NativeFrameworkDSString stringInstance){
	ostringstream out;
	out << getNumber();

	return _NativeFrameworkDSString(out.str()) + stringInstance;
}

bool _NativeFrameworkDSNumber::operator==(_NativeFrameworkDSNumber number){
	return getNumber() == number.getNumber();
}
bool _NativeFrameworkDSNumber::operator==(int _number){
	return getNumber() == (long)_number;
}
bool _NativeFrameworkDSNumber::operator==(long _number){
	return getNumber() == _number;
}
bool _NativeFrameworkDSNumber::operator!=(_NativeFrameworkDSNumber number){
	return getNumber()!=number.getNumber();
}
bool _NativeFrameworkDSNumber::operator!=(int _number){
	return getNumber() != (long)_number;
}
bool _NativeFrameworkDSNumber::operator!=(long _number){
	return getNumber() != _number;
}
// V1.1 comparison operators
bool _NativeFrameworkDSNumber::operator<(int number){
	return getNumber() < number;
}
bool _NativeFrameworkDSNumber::operator<(long number){
	return getNumber() < number;	
}
bool _NativeFrameworkDSNumber::operator>(int number){
	return getNumber() > number;
}
bool _NativeFrameworkDSNumber::operator>(long number){
	return getNumber() > number;	
}
bool _NativeFrameworkDSNumber::operator<=(int number){
	return getNumber() <= number;
}
bool _NativeFrameworkDSNumber::operator<=(long number){
	return getNumber() <= number;	
}
bool _NativeFrameworkDSNumber::operator>=(int number){
	return getNumber() >= number;
}
bool _NativeFrameworkDSNumber::operator>=(long number){
	return getNumber() >= number;	
}
/*
*	Native Frameworkm DSDouble Implementation
*/
_NativeFrameworkDSDecimal::_NativeFrameworkDSDecimal(){
	value = 0;
}
_NativeFrameworkDSDecimal::_NativeFrameworkDSDecimal(double value){
	this -> value = value;
}
_NativeFrameworkDSDecimal::_NativeFrameworkDSDecimal(int value){
	this -> value = (double)value;
}

_NativeFrameworkDSDecimal _NativeFrameworkDSDecimal::operator+(_NativeFrameworkDSDecimal selfInstance){
	return _NativeFrameworkDSDecimal(getDecimal() + selfInstance.getDecimal());
}
_NativeFrameworkDSDecimal _NativeFrameworkDSDecimal::operator-(_NativeFrameworkDSDecimal selfInstance){
	return _NativeFrameworkDSDecimal(getDecimal() - selfInstance.getDecimal());
}
_NativeFrameworkDSDecimal _NativeFrameworkDSDecimal::operator*(_NativeFrameworkDSDecimal selfInstance){
	return _NativeFrameworkDSDecimal(getDecimal() * selfInstance.getDecimal());
}
_NativeFrameworkDSDecimal _NativeFrameworkDSDecimal::operator/(_NativeFrameworkDSDecimal selfInstance){
	return _NativeFrameworkDSDecimal(getDecimal() / selfInstance.getDecimal());
}
void _NativeFrameworkDSDecimal::operator+=(_NativeFrameworkDSDecimal selfInstance){
	this -> value += selfInstance.getDecimal();
}
void _NativeFrameworkDSDecimal::operator-=(_NativeFrameworkDSDecimal selfInstance){
	this -> value -= selfInstance.getDecimal();
}
void _NativeFrameworkDSDecimal::operator*=(_NativeFrameworkDSDecimal selfInstance){
	this -> value *= selfInstance.getDecimal();
}
void _NativeFrameworkDSDecimal::operator/=(_NativeFrameworkDSDecimal selfInstance){
	this -> value /= selfInstance.getDecimal();
}
_NativeFrameworkDSDecimal _NativeFrameworkDSDecimal::operator++(){
	return _NativeFrameworkDSDecimal(getDecimal()+1);
}
_NativeFrameworkDSDecimal _NativeFrameworkDSDecimal::operator--(){
	return _NativeFrameworkDSDecimal(getDecimal()-1);
}

_NativeFrameworkDSDecimal _NativeFrameworkDSDecimal::operator+(long value){
	return _NativeFrameworkDSDecimal(getDecimal() + value);
}
_NativeFrameworkDSDecimal _NativeFrameworkDSDecimal::operator-(long value){
	return _NativeFrameworkDSDecimal(getDecimal() - value);
}
_NativeFrameworkDSDecimal _NativeFrameworkDSDecimal::operator*(long value){
	return _NativeFrameworkDSDecimal(getDecimal() * value);
}
_NativeFrameworkDSDecimal _NativeFrameworkDSDecimal::operator/(long value){
	return _NativeFrameworkDSDecimal(getDecimal() / value);
}
void _NativeFrameworkDSDecimal::operator+=(long value){
	this -> value += value;
}
void _NativeFrameworkDSDecimal::operator-=(long value){
	this -> value -= value;
}
void _NativeFrameworkDSDecimal::operator*=(long value){
	this -> value *= value;
}
void _NativeFrameworkDSDecimal::operator/=(long value){
	this -> value /= value;
}
_NativeFrameworkDSDecimal _NativeFrameworkDSDecimal::operator+(double value){
	return _NativeFrameworkDSDecimal((double)(getDecimal() + value));
}
_NativeFrameworkDSDecimal _NativeFrameworkDSDecimal::operator-(double value){
	return _NativeFrameworkDSDecimal((double)(getDecimal() - value));
}
_NativeFrameworkDSDecimal _NativeFrameworkDSDecimal::operator*(double value){
	return _NativeFrameworkDSDecimal((double)(getDecimal() * value));
}
_NativeFrameworkDSDecimal _NativeFrameworkDSDecimal::operator/(double value){
	return _NativeFrameworkDSDecimal((double)(getDecimal() / value));
}
void _NativeFrameworkDSDecimal::operator+=(double value){
	this -> value += value;
}
void _NativeFrameworkDSDecimal::operator-=(double value){
	this -> value -= value;
}
void _NativeFrameworkDSDecimal::operator*=(double value){
	this -> value *= value;
}
void _NativeFrameworkDSDecimal::operator/=(double value){
	this -> value /= value;
}

// appending string -> V1.1
_NativeFrameworkDSString _NativeFrameworkDSDecimal::operator+(const char *cString){
	ostringstream out;
	out << getDecimal();

	return _NativeFrameworkDSString(out.str() + string(cString));
}
_NativeFrameworkDSString _NativeFrameworkDSDecimal::operator+(_NativeFrameworkDSString stringInstance){
	return _NativeFrameworkDSString("") + getDecimal() + stringInstance;
}

// comparison operators
bool _NativeFrameworkDSDecimal::operator==(_NativeFrameworkDSDecimal doubleInstance){
	return doubleInstance.getDecimal() == getDecimal();
}
bool _NativeFrameworkDSDecimal::operator==(double doubleValue){
	return doubleValue == getDecimal();
}
bool _NativeFrameworkDSDecimal::operator!=(_NativeFrameworkDSDecimal doubleInstance){
	return doubleInstance.getDecimal() != getDecimal();
}
bool _NativeFrameworkDSDecimal::operator!=(double doubleValue){
	return getDecimal() != doubleValue;
}
// V1.1 comparison operators
bool _NativeFrameworkDSDecimal::operator<(double number){
	return getDecimal() < number;
}
bool _NativeFrameworkDSDecimal::operator<=(double number){
	return getDecimal() <= number;
}
bool _NativeFrameworkDSDecimal::operator>(double number){
	return getDecimal() > number;
}
bool _NativeFrameworkDSDecimal::operator>=(double number){
	return getDecimal() >= number;
}

// public methods
double _NativeFrameworkDSDecimal::getDecimal(){
	return value;
}
string _NativeFrameworkDSDecimal::toString(){
	ostringstream os;
	os << value;
	return os.str();
}
// primitives overriding

_NativeFrameworkDSVariant::_NativeFrameworkDSVariant() : longValue(static_cast<long>(0)){}

/*variant's constructors impl_*/
_NativeFrameworkDSVariant::_NativeFrameworkDSVariant(int _integerValue) : longValue(static_cast<long>(_integerValue)){
	activeData = LONG;
}
_NativeFrameworkDSVariant::_NativeFrameworkDSVariant(short _shortValue) : longValue(static_cast<long>(_shortValue)){
	activeData = LONG;
}
_NativeFrameworkDSVariant::_NativeFrameworkDSVariant(long _longValue) : longValue(static_cast<long>(_longValue)){
	activeData = LONG;
}
_NativeFrameworkDSVariant::_NativeFrameworkDSVariant(double _doubleValue) : doubleValue(static_cast<double>(_doubleValue)){
	activeData = DOUBLE;
}
_NativeFrameworkDSVariant::_NativeFrameworkDSVariant(float _floatValue) : doubleValue(static_cast<double>(_floatValue)){
	activeData = DOUBLE;
}
_NativeFrameworkDSVariant::_NativeFrameworkDSVariant(char _charValue) : charValue(static_cast<char>(_charValue)){
	activeData = CHAR;
}
_NativeFrameworkDSVariant::_NativeFrameworkDSVariant(string _stringValue) : stringValue(static_cast<string>(_stringValue)){
	activeData = STRING;
}
_NativeFrameworkDSVariant::_NativeFrameworkDSVariant(char *_stringValue) : stringValue(static_cast<string>(_stringValue)){
	activeData = C_STR;
}
_NativeFrameworkDSVariant::_NativeFrameworkDSVariant(bool _booleanValue) : booleanValue(static_cast<bool>(_booleanValue)){
	activeData = BOOLEAN;
}


// getter's implementation
long _NativeFrameworkDSVariant::getInteger(){
	if(activeData == LONG)
		return longValue;
	throw _DSException("Number is not initialized");
}
long _NativeFrameworkDSVariant::getShort(){
	return getInteger();
}
long _NativeFrameworkDSVariant::getLong(){
	return getInteger();
}
long _NativeFrameworkDSVariant::getNumber(){
	return getInteger();
}
double _NativeFrameworkDSVariant::getDecimal(){
	if(activeData == DOUBLE)
		return doubleValue;
	throw _DSException("Decimal is not initilaized");
}
double _NativeFrameworkDSVariant::getFloat(){
	return getDecimal();
}
char _NativeFrameworkDSVariant::getChar(){
	if(activeData == CHAR)
		return charValue;
	throw _DSException("Characater value not initalized");
}
string _NativeFrameworkDSVariant::getString(){
	if(activeData == STRING || activeData == C_STR)
		return stringValue.c_str();
	throw _DSException("String value not initialized");
}
bool _NativeFrameworkDSVariant::getBool(){
	if(activeData == BOOLEAN)
		return booleanValue;
	throw _DSException("Boolean not initialized");
}
DataTypes _NativeFrameworkDSVariant::getType(){
	return activeData;
}
//operators implementation
bool _NativeFrameworkDSVariant::operator==(_NativeFrameworkDSVariant compareWith){
	if(compareWith.getType() == this -> getType())
		switch(getType()){
			case LONG:
				return compareWith.getLong() == getLong() ;
			case DOUBLE:
				return compareWith.getDecimal() == getDecimal();
			case CHAR:
				return compareWith.getChar() == getChar();
			case STRING:
				return compareWith.getString() == getString();
			case C_STR:
				return compareWith.getString() == getString();
			case BOOLEAN:
				return compareWith.getBool() == getBool();
		}
}

bool _NativeFrameworkDSVariant::operator!=(_NativeFrameworkDSVariant compareWith){
	if(getType() == compareWith.getType())
		switch(getType()){
			case LONG:
				return compareWith.getLong() != getLong();
			case DOUBLE:
				return compareWith.getDecimal() != getDecimal();
			case CHAR:
				return compareWith.getChar() != getChar();
			case STRING:
				return compareWith.getString() != getString();
			case C_STR:
				return compareWith.getString() != getString();
			case BOOLEAN:
				return compareWith.getBool() != getBool();
		}
	return true;
}

#endif