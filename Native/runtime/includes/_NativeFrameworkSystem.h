#pragma once
/*
*	NATIVE FRAMEWORK SYSTEM implementation class.
*	This class gives the access to the various system properties on which the class is being executed.
*	@Author gaurav sharma
*	TESTED on windows 8 x64, 8.1 x84 and windows 7 x86
*/

#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdbool.h>

using namespace std;

class _NativeFrameworkSystemException{
	private:
		string errorMessage;
		string filename;
		string function;
		int line;

		void _printError();
	public:
		_NativeFrameworkSystemException(string, string, string, int);

		string toString();
};

/*
*	This class provides the cross-platfform level operating system properties.
*/
class _NativeFrameworkSystemProperties{
	public:
		/*	
		*	the file separator for the operating system
		*	the file separator is "\" for windows and "/" for unix-like OS
		*/
		static string getFileSeparator();
		/*
		*	get the path separator for the operating system
		*	path separator is ";" for windows and ":" for unix-like OS
		*/
		static string getPathSeparator();
		/*
		*	get the home directory of the user.
		*	Home directory takes the follwing syntax:
		*		C:\\Users\\gaurav	-> on windows OS
		*	and,
		*		/home/gaurav		-> on unix-like OS
		*/
		static string getHomePath();
		/*
		*	The base directory is the root folder of the drive in which the OS is installer
		*	for windows it is like "C:\\", "D:\\" or any other drive and on unix-like systems
		*	it is "/" called ROOT FOLDER.
		*/
		static string getBaseDirectoryPath();
};


/*
*	The Native environment variable procesing class. This class must be used with care as it may break
*	the system. update/create methods must be used with precision and only if you know what you are doing.
*/

class _NativeFrameworkWindowsSystemVariable{
	private:
	public:
		/*
		*	Checks the existence of an environment variable
		*	@param1 represents the name of the variable to check
		*/
		static bool containsVariable(string);
		/*
		*	get the value of an environment variable
		*	@Param1 represents the name of the variable to get value of
		*/
		static string getValueOf(string);
		/*
		*	create a temporary environment variable
		*	@Param1 represents the name of the variable
		*	@Param2 represents the value of the variable
		*/
		static void createTempVariable(string, string);
		/*
		*	create a system level permanent environment variable
		*	@Param1 represents the name of the variable
		*	@Param2 represents the value of the variable
		*/
		static void updateTempVariable(string, string);
		/*
		*	update an already existing temp variable. This methods appends the new value to a existing variable
		*	instead of creating a new variable.
		*	@Param1 represents the variable to update
		*	@Param2 represents the variable's new value
		*/
		static void createSystemVariable(string, string);
		/*
		*	update an already existing system variable. This methods appends the new value to a existing variable
		*	instead of creating a new variable. The updateXXXVariable() method is considered safe over the 
		*	createXXXVariable() beacause this method appends the variable value if it already exists or create one
		*	if one does not exists.
		*	@Param1 represents the variable to update
		*	@Param2 represents the variable's new value
		*/
		static void updateSystemVariable(string, string);
};

/*
*	contains the very low level properties of the windows OS. this class uses the windows variables definesd in 
*	windows 7 and windows 8 OS. (Check for these in MS-XP)
*	for accessing the system properties. The properties are initialized by creating the instance
*	of this class and same could be accessed using the getter methods of this class.
*	This class is only working the windows OS. This class uses the Environment Variables created by the windows OS
*	automatically that defines the system. In case, the variable changes, the values that this class provide will
*	also change. But 99% chances are that this class will give the accurate information.
*/
class _NativeFrameworkWindowsProperties{
		//int processorsCount;			//	NUMBER_OF_PROCESSORS
		//string operatingSystem;		//	OS
		//string processorArch;			//	PROCESSOR_ARCHITECTURE
		//string processorId;			//	PROCESSOR_IDENTIFIER
		//int processorLevel;			//	PROCESSOR_LEVEL
		//string processorRevision;		// 	PROCESSOR_REVISION
	public:
		/*
		*	Get the number of processor in the system. Uses the Variable NUMBER_OF_PROCESSORS
		*/
		static long getProcessorsCount();
		/*
		*	Get the name of operating system. Uses the OS Variable
		*/
		static string getOSName();
		/*
		*	Get the Architecture of the processor in the system. Uses PROCESSOR_ARCHITECTURE Variable.
		*/
		static string getProcessorArchitecture();
		/*
		*	Get the Processor identification for the system. Uses PROCESSOR_IDENTIFICATION Variable
		*/
		static string getProcessorIdentification();
		/*
		*	Get the Processor Level. Uses the PROCESSOR_LEVEL Variable.
		*/
		static string getProcessorLevel();
		/*
		*	Get the Processor revision. Uses the PROCESSOR_REVISION Variable
		*/
		static string getProcessorRevision();
};

/*executing the system commands using this class*/
class _NativeFrameworkSystemCalls{
	public:
		static void execute(char*);
		static void execute(string);
};