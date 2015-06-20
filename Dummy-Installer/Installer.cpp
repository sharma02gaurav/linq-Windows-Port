#include <iostream>
#include <string>
#include <stdlib.h>

/*
*	installer/ configurator for linq framework
*/

using namespace std;

#ifdef _WIN32
	#define FILESEP "\\"
#else
	#define FILESEP "//"
#endif
#ifdef _WIN32
	#define PATHSEP ";"
#else
	#define PATHSEP ":"
#endif

void runLoader(){
	string execLoader = string("Loader-Runtime") + FILESEP + "bin" + FILESEP + "java -jar LinqLoading";
	system(execLoader.c_str());
}

int main(int argc, char* argv[]){
	// startup loader
	string loaderCommand = string("Loader-Runtime") + FILESEP + "bin" + FILESEP + "java -jar LinqLoading.jar";
	system(loaderCommand.c_str());

	char ch;
	cout << "Installing linq framework" << endl;
	string homepath;
	#ifdef _WIN32
		// get the homepath for windows OS
		string basedrive = getenv("HOMEDRIVE");
		homepath = basedrive + FILESEP + string(getenv("HOMEPATH"));
	#else
		// get the homepath for unix like OS
		homepath = getenv("HOME");
	#endif
	string folderName = "Linq-Runtime";
	string makefolder = "md " + homepath + FILESEP + folderName;
	string makeMingW = "md " + homepath + FILESEP + folderName + FILESEP + "MinGW";
	string makeModules = "md "+ homepath + FILESEP + folderName + FILESEP + "Modules";
	string makeDependencies = "md "+ homepath + FILESEP + folderName + FILESEP + "Native-Dependencies";
	// making the installing location folder that contains the 
	// framework content.
	system(makefolder.c_str());
	system(makeMingW.c_str());
	system(makeModules.c_str());
	system(makeDependencies.c_str());

	string mingwPath = homepath + FILESEP + folderName + FILESEP + "MinGW" + FILESEP + "bin";
	
	string copyCompiler = string("xcopy /S MinGW ") + homepath + FILESEP + folderName + FILESEP + "MinGW";
	string copyModules = string("xcopy /S Modules ") +homepath + FILESEP + folderName + FILESEP + "Modules";
	string copyDependencies = string("xcopy /S Native-Dependencies ") + homepath + FILESEP + folderName + FILESEP + "Native-Dependencies";
	string copylinqc = string("copy linqc.exe ")+ homepath + FILESEP + folderName + FILESEP ;
	// added for v1.2
	// contains the version information and the bugs fixes information
	string copylinq = string("copy linq.exe ")+ homepath + FILESEP + folderName + FILESEP;
	
	system(copyCompiler.c_str());
	system(copyModules.c_str());
	system(copyDependencies.c_str());
	system(copylinqc.c_str());
	system(copylinq.c_str());

	/*
	*	copy framework libraris to system32 folder
	*/
	string systemFolderPath = basedrive + FILESEP + "Windows" + FILESEP + "System32";
	string command = "xcopy /S Runtime " + systemFolderPath;

	system(command.c_str());


	string installationPath = homepath + FILESEP + folderName;
	/*
	*	set the environment variable for this location
	*/
	string setCommand = string("\nSETX PATH \"") + getenv("PATH") + PATHSEP + installationPath + PATHSEP +  mingwPath + PATHSEP+"\"\n";

	//cout << setCommand << endl;

	system(setCommand.c_str());

	cout << "Installation complete!!" << endl;

	// run the database configuration wizard
	cout << "Running Database Configuration Wizard" << endl;
	system("SQLiteConfigurator");
	// startup loader
	string finishCommand = string("Loader-Runtime") + FILESEP + "bin" + FILESEP + "java -jar InstallationSuccess.jar";
	system(finishCommand.c_str());

	return 0;
}