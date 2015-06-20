#ifndef NATIVE_FRAMEWORK_STREAMSIMPL
#define NATIVE_FRAMEWORK_STREAMSIMPL

#include "../includes/_NativeFrameworkStreams.h"

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

#define isnot !

#define _StreamsException(Error_Message) _NativeFrameworkStreamsException(Error_Message, __ERRFILE, __ERRFUNC, __ERRLINE)

/*
*	Exception Implementation
*/

void _NativeFrameworkStreamsException::_printError(){
	cerr << "ERROR: " ;
	cerr << errorMessage << " \nin " << filename << " on line " << line << " in function " << function << endl;
	cerr << "Aborting" << endl;
}

_NativeFrameworkStreamsException::_NativeFrameworkStreamsException(string errorMessage,
	string filename, string function, int line){
	this -> errorMessage = errorMessage;
	this -> filename = filename;
	this -> function = function;
	this -> line = line;

	_printError();
}

string _NativeFrameworkStreamsException::toString(){
	return errorMessage;
}

/*
*	File Implementation
*/
_NativeFrameworkStreamsFile::_NativeFrameworkStreamsFile(string filename){
	this -> filename = filename;
	if(filename.find(__FILESEP) != -1){
		path = filename.substr(0, filename.rfind(__FILESEP));
		name = filename.substr(filename.rfind(__FILESEP)+1, filename.length());
	}else{
		path = "";
		name = filename;
	}
}
_NativeFrameworkStreamsFile::_NativeFrameworkStreamsFile(){
	filename = "";
	name = "";
	path = "";
}

bool _NativeFrameworkStreamsFile::exists(){
	if(filename != ""){
		ifstream inputFile(filename, ios_base::in);
		if(inputFile){
			inputFile.close();
			return true;
		}else{
			inputFile.close();
			return false;
		}
	}else
		throw _StreamsException("No filename provided. Invoke the constructor right way");
}
long _NativeFrameworkStreamsFile::size(){
	if(exists()){
		long size = 0;
		ifstream tempFile(filename, ios_base::in);
		char ch;
		while(! tempFile.eof()){
			string currentLine;
			getline(tempFile, currentLine);
			size += currentLine.length();
		}
		tempFile.close();
		return size-1;
	}else
		throw _StreamsException("No filename provided. Invoke the constructor right way");
}

/*
*	FileInputStream
*/

void _NativeFrameworkFileInputStream::_seekTo(__SEEK to){
	switch(to){
		case Begin:
			inputFile.seekg(ios_base::beg); break;
		case End:
			inputFile.seekg(ios_base::end); break;
	}
}

// 	input stream implementation
_NativeFrameworkFileInputStream::_NativeFrameworkFileInputStream(string path){
	inputFile.open(path.c_str(), ios_base::in);
	if(!inputFile)
		throw _StreamsException(path + " file not found.");
	closeFlag = false;
}
_NativeFrameworkFileInputStream::_NativeFrameworkFileInputStream(char *path){
	inputFile.open(path, ios_base::in);
	if(! inputFile)
		throw _StreamsException(string(path) + " file not found.");
	closeFlag = false;
}

bool _NativeFrameworkFileInputStream::isOpen(){
	return ! closeFlag;
}
void _NativeFrameworkFileInputStream::jumpToStart(){
	if(inputFile)
		_seekTo(Begin);
	else
		throw _StreamsException("Cannot apply Jump. The stream is closed.");
}
void _NativeFrameworkFileInputStream::jumpFromStart(int index){
	if(inputFile){
		if(index < 0)
			throw _StreamsException("Cannot jump back from the start position.");
		inputFile.seekg(index, ios_base::beg);
	}else
		throw _StreamsException("Cannot apply Jump. The stream is closed.");
} 
void _NativeFrameworkFileInputStream::jumpToEnd(){
	if(inputFile)
		_seekTo(End);
	else
		throw _StreamsException("Cannot apply Jump. The stream is closed.");
}

void _NativeFrameworkFileInputStream::jumpFromEnd(int index){
	if(inputFile)
		if(index > 0)
			throw _StreamsException("Cannot Jump beyond end. provide a -ve value instead");
		else
			inputFile.seekg(index, ios_base::end);
	else
		throw _StreamsException("Cannot apply jump. Stream is closed.");

}
void _NativeFrameworkFileInputStream::jumpForward(int index){
	if(inputFile)
		inputFile.seekg(index, ios_base::cur);
	else
		throw _StreamsException("Cannot apply jump. Stream is closed.");
}
void _NativeFrameworkFileInputStream::jumpBackward(int index){
	if(inputFile){
		if(index > 0)
			throw _StreamsException("Jump back index must be -ve.");
		inputFile.seekg(index, ios_base::cur);
	}else
		throw _StreamsException("Cannot jump back from the start");
}

string _NativeFrameworkFileInputStream::getFileContent(){
	if(inputFile){
		string documentContent;
		while(isnot inputFile.eof()){
			string currentLine;
			getline(inputFile, currentLine);
			documentContent += currentLine + "\n";
		}

		_seekTo(Begin);

		return documentContent;
	}
	throw _StreamsException("Cannot read file. The stream is closed.");
}
char _NativeFrameworkFileInputStream::charAtPosition(int index){
	if(inputFile){
		char readChar;
		for(int i=0; i<= index; i+=1)
			inputFile.get(readChar);

		_seekTo(Begin);
		return readChar;
	}
	throw _StreamsException("Cannot read the file. The stream is closed.");
}

int _NativeFrameworkFileInputStream::linesCount(){
	if(inputFile){
		int totalLines = 0;
		while(isnot inputFile.eof()){
			string currentLine;	
			getline(inputFile, currentLine);
			totalLines += 1;
		}
		_seekTo(Begin);

		return totalLines;
	}
	throw _StreamsException("Cannot read the file. The stream is closed.");
}

int _NativeFrameworkFileInputStream::countOccuranceOf(char character){
	if(inputFile){
		char readChar;
		int occCount = 0;

		while(isnot inputFile.eof()){
			inputFile.get(readChar);
			if(readChar == character)
				occCount += 1;
		}

		_seekTo(Begin);

		return occCount;
	}
	throw _StreamsException("Cannot read the file. The Stream is closed.");
}

bool _NativeFrameworkFileInputStream::isReadable(){
	if(inputFile)
		if(isnot inputFile.eof())
			return true;
	return false;
}
char _NativeFrameworkFileInputStream::getChar(){
	if(isReadable()){
		char ch;
		inputFile.get(ch);
		return ch;
	}
	throw _StreamsException("Cannot read the file. The stream is closed .");
}
string _NativeFrameworkFileInputStream::getLine(){
	if(isReadable()){

		string theLine;
		getline(inputFile, theLine);
		return theLine;
	}
	throw _StreamsException("Cannot read the file.The stream is close.");
}
string _NativeFrameworkFileInputStream::getLine(int lineNumber){
	if(isReadable()){
		string toReturn = "";
		if(lineNumber <= linesCount()){
			for(int i=0; i<lineNumber; i+=1)
				getLine();
			toReturn = getLine();
			_seekTo(Begin);
			return toReturn;
		}
		throw _StreamsException("Line number error: requested lines are greater than the available lines in the file.");
	}
	throw _StreamsException("Cannot read the file. The stream is closed.");
}
bool _NativeFrameworkFileInputStream::isClose(){
	return closeFlag;
}
void _NativeFrameworkFileInputStream::close(){
	if(! closeFlag){
		inputFile.close();
		closeFlag = true;
	}else
		throw _StreamsException("Stream is already closed.");
}

/*
*	FileOutputStream IMPLEMENTATION
*/
void _NativeFrameworkFileOutputStream::_seekTo(__SEEK to){
	switch(to){
		case Begin:
			outputFile.seekp(ios_base::beg); break;
		case End:
			outputFile.seekp(ios_base::end); break;
	}
}
_NativeFrameworkFileOutputStream::_NativeFrameworkFileOutputStream(string path){
	// open the file in READ-WRITE mode.
	outputFile.open(path.c_str(), ios_base::out);
	if(! outputFile){
		activeFlag = false;
		throw _StreamsException(path +" is not accessible. Maybe no permissions to read from that location.");
	}
	activeFlag = true;
}
_NativeFrameworkFileOutputStream::_NativeFrameworkFileOutputStream(char *path){
	outputFile.open(path, ios_base::out);
	if(! outputFile){
		activeFlag = false;
		throw _StreamsException(string(path) +" is not accessible. Maybe no permissions to read from that location.");
	}
	activeFlag = true;
}

bool _NativeFrameworkFileOutputStream::isOpen(){
	if(outputFile)	
		return true;
	return false;
}

void _NativeFrameworkFileOutputStream::jumpToStart(){
	if(activeFlag)
		_seekTo(Begin);
	else
		throw _StreamsException("Cannot jump. The stream is closed.");
}
void _NativeFrameworkFileOutputStream::jumpToEnd(){
	if(activeFlag)
		_seekTo(End);
	else
		throw _StreamsException("Cannot jump. The stream is closed.");
}
void _NativeFrameworkFileOutputStream::jumpFromEnd(int index){
	if(activeFlag)
		if(index <= 0)
			outputFile.seekp(index, ios_base::end);
		else
			throw _StreamsException("The index value must be -ve in order to seek back.");
	else
		throw _StreamsException("Cannot jump. The Stream is closed.");
}
void _NativeFrameworkFileOutputStream::jumpFromStart(int index){
	if(activeFlag)
		if(index >= 0)
			outputFile.seekp(index, ios_base::beg);
		else
			throw _StreamsException("Cannot seek beyond the start index. Use a +ve value insted.");
	else
		throw _StreamsException("Cannot jump. The stream is closed.");
}
void _NativeFrameworkFileOutputStream::jumpForward(int index){
	if(activeFlag)
		outputFile.seekp(index, ios_base::cur);
	else
		throw _StreamsException("Cannot jump. The Stream is closed.");
}
void _NativeFrameworkFileOutputStream::jumpBackward(int negIndex){
	if(activeFlag)
		outputFile.seekp(negIndex, ios_base::cur);
	else
		throw _StreamsException("Cannot jump. The Stream is closed.");
}

bool _NativeFrameworkFileOutputStream::isWritable(){
	if(activeFlag)
		return (outputFile.eof()) ? false : true;
	return false;		
}
void _NativeFrameworkFileOutputStream::write(char ch){
	outputFile << ch;
}
void _NativeFrameworkFileOutputStream::write(string str){
	outputFile << str;
}
void _NativeFrameworkFileOutputStream::write(char *str){
	outputFile << str;
}
void _NativeFrameworkFileOutputStream::write(int i){
	outputFile << i;
}
void _NativeFrameworkFileOutputStream::write(long l){
	outputFile << l;
}
void _NativeFrameworkFileOutputStream::write(double d){
	outputFile << d;
}
void _NativeFrameworkFileOutputStream::writeLine(char ch){
	write(ch);
	writeNewline();
}
void _NativeFrameworkFileOutputStream::writeLine(string str){
	write(str);
	writeNewline();
}
void _NativeFrameworkFileOutputStream::writeLine(char *str){
	write(str);
	writeNewline();
}
void _NativeFrameworkFileOutputStream::writeLine(int i){
	write(i);
	writeNewline();
}
void _NativeFrameworkFileOutputStream::writeLine(long l){
	write(l);
	writeNewline();
}
void _NativeFrameworkFileOutputStream::writeLine(double d){
	write(d);
	writeNewline();
}
void _NativeFrameworkFileOutputStream::writeTab(){
	outputFile << "\t";
}
void _NativeFrameworkFileOutputStream::writeNewline(){
	outputFile << "\n";
}

void _NativeFrameworkFileOutputStream::createCopyOf(string filePath){
	ifstream tempInputFile(filePath.c_str(), ios_base::in);
	if(tempInputFile){
		string content;
		while(!tempInputFile.eof()){
			string indLine;
			getline(tempInputFile, indLine);
			content += indLine + "\n";
		}
		// write the content to this file;
		outputFile << content;
		tempInputFile.close();
	}else
		throw _StreamsException(filePath +" file does not exists.");
}
void _NativeFrameworkFileOutputStream::createCopyOf(char *filePath){
	createCopyOf(string(filePath));
}

bool _NativeFrameworkFileOutputStream::isClose(){
	if(activeFlag)
		return false;
	return true;
}
void _NativeFrameworkFileOutputStream::close(){
	if(activeFlag){
		outputFile.close();
		closeFlag = true;
		activeFlag = false;
	}else if(closeFlag)
		throw _StreamsException("Cannot close. The stream is already close.");
}

/*
*	PersistentFileStream Implementation
*/

_NativeFrameworkPersistentFileStream::_NativeFrameworkPersistentFileStream(string inputPath){
	outFile.open(inputPath.c_str(), ios_base::app);
	if(outFile.good() && outFile){
		currentPos = outFile.tellp();
		openFlag = true;
	}else{
		openFlag = false;
		throw _StreamsException(inputPath +" file not accessible. Maybe no permissions to access file at this location.");
	}
}
_NativeFrameworkPersistentFileStream::_NativeFrameworkPersistentFileStream(char *inputPath){
	outFile.open(inputPath, ios_base::app);
	if(outFile.good() && outFile){
		currentPos = outFile.tellp();
		openFlag = true;
	}else{
		openFlag = false;
		throw _StreamsException(string(inputPath) +" file not accessible. Maybe no permissions to access file at this location.");
	}
}

bool _NativeFrameworkPersistentFileStream::isOpen(){
	return openFlag;
}
bool _NativeFrameworkPersistentFileStream::isClose(){
	return !openFlag;
}
bool _NativeFrameworkPersistentFileStream::isAppendable(){
	if(openFlag)
		if(! outFile.eof())
			return true;
	return false;
}
void _NativeFrameworkPersistentFileStream::appendValue(int i){
	outFile << i;
}
void _NativeFrameworkPersistentFileStream::appendValue(long l){
	outFile << l;
}
void _NativeFrameworkPersistentFileStream::appendValue(string str){
	outFile << str;
}
void _NativeFrameworkPersistentFileStream::appendValue(char *str){
	outFile << str;
}
void _NativeFrameworkPersistentFileStream::appendValue(double dbl){
	outFile << dbl;
}
void _NativeFrameworkPersistentFileStream::appendValue(char ch){
	outFile << ch;
}
void _NativeFrameworkPersistentFileStream::appendLine(int i){
	appendValue(i);
	appendNewline();
}
void _NativeFrameworkPersistentFileStream::appendLine(long l){
	appendValue(l);
	appendNewline();
}
void _NativeFrameworkPersistentFileStream::appendLine(string str){
	appendValue(str);
	appendNewline();
}
void _NativeFrameworkPersistentFileStream::appendLine(char *str){
	appendValue(str);
	appendNewline();
}
void _NativeFrameworkPersistentFileStream::appendLine(double dbl){
	appendValue(dbl);
	appendNewline();
}
void _NativeFrameworkPersistentFileStream::appendLine(char ch){
	appendValue(ch);
	appendNewline();
}
void _NativeFrameworkPersistentFileStream::appendTab(){
	outFile << "\t";
}
void _NativeFrameworkPersistentFileStream::appendNewline(){
	outFile << "\n";
}
void _NativeFrameworkPersistentFileStream::appendSpace(){
	outFile << " ";
}
void _NativeFrameworkPersistentFileStream::close(){
	if(openFlag){
		outFile.close();
		openFlag = false;
	}else
		throw _StreamsException("Cannot close stream. Already closed");
}

#endif