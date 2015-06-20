COLOR F1

echo "Generating dll files"

g++ -w -std=c++11 implementations/_NativeFrameworkConsoleIMPL.cpp -shared -olibs/frmwrklinqconsole.dll
g++ -w -std=c++11 implementations/_NativeFrameworkDSIMPL.cpp -shared -olibs/frmwrklinqDS.dll
g++ -w -std=c++11 implementations/_NativeFrameworkConverterIMPL.cpp -shared -olibs/frmwrklinqconverter.dll
g++ -w -std=c++11 -Llibs -lsqlite3 -lfrmwrklinqDS implementations/_NativeFrameworkSQLiteIMPL.cpp -shared -olibs/frmwrklinqsqlite.dll
g++ -w -std=c++11 implementations/_NativeFrameworkStreamsIMPL.cpp -shared -olibs/frmwrklinqstreams.dll
g++ -w -std=c++11 implementations/_NativeFrameworkSystemIMPL.cpp -shared -olibs/frmwrklinqsystem.dll
g++ -w -std=c++11 implementations/_NativeFrameworkDateIMPL.cpp -shared -olibs/frmwrklinqdate.dll
g++ -w -std=c++11 implementations/_NativeFrameworkMathsIMPL.cpp -shared -olibs/frmwrklinqmaths.dll

echo "Done Compiling!!"

pause
cls