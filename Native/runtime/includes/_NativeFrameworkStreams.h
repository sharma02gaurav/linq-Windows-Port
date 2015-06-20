#pragma once

#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <stdbool.h>

using namespace std;

class _NativeFrameworkStreamsException{
	private:
		string errorMessage;
		string filename;
		string function;
		int line;

		void _printError();
	public:
		_NativeFrameworkStreamsException(string, string, string, int);
		string toString();
};

/*
*	The file handling class for linq framework
*/
class _NativeFrameworkStreamsFile{
	private:
		string filename;
		string name;
		string path;
	public:
		_NativeFrameworkStreamsFile(string);
		_NativeFrameworkStreamsFile();
		bool exists();
		long size();
		// complete path along with the filename
		string getCompletePath();
		// only the filename
		string getName();
		// only the path to file
		string getPath();
};
/*
*	The read-only capabilities on the file
*/
class _NativeFrameworkFileInputStream{
	private:
		ifstream inputFile;
		bool closeFlag;
		enum __SEEK{Begin, End};
		/*
		*	private seek method
		*/
		void _seekTo(__SEEK);
	public:
		_NativeFrameworkFileInputStream(string);
		_NativeFrameworkFileInputStream(char*);

		/*
		*	returns true if file is open
		*/
		bool isOpen();
		/*
		*	jumps to the start index of the file
		*/
		void jumpToStart();
		/*
		*	jump forward from the start index to the specified index
		*/
		void jumpFromStart(int);
		/*
		*	jump to end index of the file
		*/
		void jumpToEnd();
		/*
		*	jump to the specified index back from the end of the file
		*	index provides a -ve value to jump back.
		*/
		void jumpFromEnd(int);
		/*
		*	jump formward to the specified index from the current index position
		*/
		void jumpForward(int);
		/*
		*	jump backward from the current index to the specified index
		*/
		void jumpBackward(int);
		/*
		*	returns the text file content
		*/
		string getFileContent();
		/*
		*	get the charcter at the specified index in the file
		*/
		char charAtPosition(int);
		/*
		*	get the total number of lines in the file
		*/
		int linesCount();
		/*
		*	count the occurance of specified character
		*/
		int countOccuranceOf(char);
		/*
		*	returns true if the file did not reach EOF yet!
		*/
		bool isReadable();
		/*
		*	get the next char from the input stream
		*/
		char getChar();
		/*
		*	get the line as string from the input stream
		*/
		string getLine();
		/*
		*	get the line specified by the number
		*/
		string getLine(int);
		/* returns true of the file is close*/
		bool isClose();
		/*
		*	close the file stream
		*/
		void close();
};
/*
*	The FileOutputStream class provides a way to write to files or create one
*/
class _NativeFrameworkFileOutputStream{
	private:
		ofstream outputFile;
		bool activeFlag;
		bool closeFlag;

		enum __SEEK{Begin, End};
		/*
		*	private seek method
		*/
		void _seekTo(__SEEK);
	public:
		_NativeFrameworkFileOutputStream(string);
		_NativeFrameworkFileOutputStream(char*);


		bool isOpen();
		void jumpToStart();
		void jumpToEnd();
		void jumpFromEnd(int);
		void jumpFromStart(int);
		void jumpForward(int);
		void jumpBackward(int);

		bool isWritable();
		// the writing to file methods
		void write(char);
		void write(string);
		void write(char*);
		void write(int);
		void write(double);
		void write(long);

		// the writing line to file methods
		void writeLine(char);
		void writeLine(string);
		void writeLine(char*);
		void writeLine(int);
		void writeLine(double);
		void writeLine(long);
		
		// utility write
		void writeTab();
		void writeNewline();

		/* 
		*	create a copy of the file inputFile
		*	param represents the path of the file. throws exception if 
		*	file does not exists.
		*/
		void createCopyOf(string);
		void createCopyOf(char*);
		// 	check if the stream is close or not
		bool isClose();

		void close();
};

/*
*	persistence does write to a file as like the _NaitveFrameworkFileOutputStream
*	but in a safe way, the old content of file are never erased, instead appended.
*	pro 	:- safe and persistent write
*	cons 	:- cannot navigate across the bit-indexes(provides a sequential write-access only). 
*/
class _NativeFrameworkPersistentFileStream{
	private:
		ofstream outFile;
		bool openFlag;
		ios_base::streampos currentPos;
	public:
		_NativeFrameworkPersistentFileStream(string);
		_NativeFrameworkPersistentFileStream(char*);

		bool isOpen();
		bool isClose();
		bool isAppendable();
		void close();

		void appendValue(int);
		void appendValue(long);
		void appendValue(string);
		void appendValue(char*);
		void appendValue(double);
		void appendValue(char);

		void appendLine(int);
		void appendLine(long);
		void appendLine(string);
		void appendLine(char*);
		void appendLine(double);
		void appendLine(char);

		void appendTab();
		void appendNewline();
		void appendSpace();
};