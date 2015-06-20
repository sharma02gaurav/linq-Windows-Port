/*
*	linq framework compiler. linqc stands for LINQCompiler
*/

#include <_NativeFrameworkStreams.h>
#include <_NativeFrameworkDS.h>
#include <_NativeFrameworkConsole.h>
#include <_NativeFrameworkSystem.h>

#include "ImportFix.h"
#include "_LinqColon.cpp"

int main(int argsCount, char *args[]){
	if(argsCount == 2){
		_NativeFrameworkDSString filename = args[1];
		
		/*
		*	extension of the source file being provided as argument
		*/
		_NativeFrameworkDSString extension = filename.substring(filename.firstIndexOf(".")+1, filename.length());
		if(extension.equalsTo("linq")){
			/*
			*	this holds the copy of the name of the source file(just the name excluding linq extension)
			*/
			_NativeFrameworkDSString copyRealFilename = filename.substring(filename.firstIndexOf("."));
			/*
			*	linqc generates a temporary c++ source file to compile. This c++
			*	file is named after the linq source file. If the linq source file
			*	is "hello.linq" then the temporary c++ name would be hello_temp.cpp. The
			*	_temp.cpp extension is defined in this variable.
			*/
			const _NativeFrameworkDSString dummyExtension = "_temp.cpp";
			// the input file is the linq source file
			_NativeFrameworkFileInputStream readFile(filename.getString());
			/*
			*	The whole file content as string
			*/
			_NativeFrameworkDSString fileContent = readFile.getFileContent();
			if(fileContent.contains("main(") || fileContent.contains("StartProgram")){
				_NativeFrameworkDSString heading = "/*\n*\tThis is auto generated source file. Do not edit this source file.\n*/";
				_NativeFrameworkDSString defineHeader = "\n#include <runtime/SementicDefinition.linq>\n\n";

				_NativeFrameworkDSNumber totalLines = readFile.linesCount();

				/*
				* The lines counter keeps track of the lines processed by the linq compiler. Tracking the lines
				* numbers helps to indicate the user precisely that where the things went wrong and indicating
				* the error with the line numbers.
				*/
				_NativeFrameworkDSNumber linesCounter = 0;
				bool programStart = false;

				_NativeFrameworkDSString finalContent = heading;
					finalContent += defineHeader;

				while(linesCounter != totalLines){
					_NativeFrameworkDSString currentLine = readFile.getLine(linesCounter.getNumber());
					/*mark the program start*/
					if(!programStart){
						if(currentLine.contains("StartProgram") || 
							currentLine.contains("Function") || 
							currentLine.contains("Class") && 
							!currentLine.contains("\"")){

							programStart = true;
						
						}

						/*
						* If program is not start yet then we also need to import fix.
						* There will be no processing of imports after the program has started.
						*/
						/*DO IMPORTS FIXING CODE HERE*/
						if(currentLine.contains("Import") && !currentLine.contains("\"") && !currentLine.contains("/*") && !currentLine.contains("//") && !currentLine.contains("*/"))
							currentLine = ImportFix::getFixed(currentLine);
					}

					/*
					*	check if the import statement is used anywhere except the top of the source. If
					*	found anywhere else then the exception will be thrown and the compilation will terminate
					*/
					if(programStart){
						if(currentLine.contains("Import") || 
							currentLine.contains("SystemImport") ||
							 currentLine.contains("Constant") && !currentLine.contains("\"")){

							// generate errors
							_NativeFrameworkConsole::print((filename + " at line ").getString());
							_NativeFrameworkConsole::printLine(linesCounter.getNumber());
							_NativeFrameworkConsole::printLine(currentLine.getString());
							_NativeFrameworkConsole::printLine("ERROR: Import, SystemImport and Constant are valid only at the top of the source.");
						}
					}

					/*
					*	AUTO SEMICOLON REFINEMENT.. V1.1
					*/
					_LinqAutoColon autoColon;
					if(!currentLine.isEmpty()){
						bool containsChars = false;
						// check if it does not contains only the whitespaces
						for(int i=0; i<currentLine.length() ;i++){
							char currentChar = currentLine.charAt(i);
							if(currentChar != 9)
								containsChars = true;
						}
						if(containsChars)
							currentLine = autoColon.rectify(currentLine);
					}
					
					finalContent += currentLine + "\n";
					linesCounter += 1;
				}

				//_NativeFrameworkConsole::printLine(finalContent.getString());
				/*create a temp c++ file and then compile it*/
				_NativeFrameworkFileOutputStream outputFile((copyRealFilename+dummyExtension).getString());
				if(outputFile.isOpen()){
					outputFile.write(finalContent.getString());
					outputFile.close();
				}else
					_NativeFrameworkConsole::printLine("Error: Cannot compile at this location.");

				readFile.close();

				_NativeFrameworkDSString fileSep = _NativeFrameworkSystemProperties::getFileSeparator();

				// this is the path where the linq runtime is installed
				_NativeFrameworkDSString systemHome = _NativeFrameworkSystemProperties::getHomePath();
					systemHome +=  fileSep + "Linq-Runtime" + fileSep;
				systemHome.replace("\\","/");

				/*compiling the sources*/
				_NativeFrameworkDSString nativeLibraries = _NativeFrameworkDSString("-L")+ systemHome +"Native-Dependencies/libs -lfrmwrklinqconsole -lfrmwrklinqDS -lfrmwrklinqsqlite -lfrmwrklinqstreams -lfrmwrklinqsystem -lfrmwrklinqdate -lfrmwrklinqmaths ";
				_NativeFrameworkDSString nativeIncludes = _NativeFrameworkDSString("-I")+ systemHome +"Native-Dependencies " + _NativeFrameworkDSString("-I")+ systemHome +"Modules ";
				_NativeFrameworkDSString compileCommand = "g++ -w -std=c++11 ";
				_NativeFrameworkDSString finalCommand = compileCommand
					+ nativeLibraries + nativeIncludes
					+ (copyRealFilename+dummyExtension) 
					+ " -o" +copyRealFilename;

				_NativeFrameworkSystemCalls::execute(finalCommand.getString());

				_NativeFrameworkDSString deleteFile = _NativeFrameworkDSString("del ")+ (copyRealFilename+dummyExtension);

				_NativeFrameworkSystemCalls::execute(deleteFile.getString());
			}else{
				_NativeFrameworkConsole::printLine("Cannot compile the sources without the StartProgram module");
			}
		}else{
			_NativeFrameworkConsole::printLine("The Source files must contain \"linq\" extension.");
			return 1;
		}
	}else{
		_NativeFrameworkConsole::printLine("Required the linq file to compile.");
		_NativeFrameworkConsole::printLine("Proper syntax is: linqc [filename.linq]");

		return 1;
	}

	return 0;
}