#pragma once

/*
*	#dependecy	-> sqlite3.lib sqlite3.dll and sqlite3.h
*/

#include <iostream>
#include <stdlib.h>
#include <string>
#include <stdbool.h>

#include "../dependencies/sqlite3.h"

using namespace std;

/*
*	sqlite C API use sqlite3 object and error codes(int) to get certain error messages, 
*	this class is a handler for the same. Passing the required object, this class provides
*	the corrosponding error message by connecting the native C-API. This class is of no use
*	beyond this framework. It is used by the framework for better structure of the classes and
*	clean code.
*/
class _NativeFrameworkSQLiteErrMsgHandler{
	public:
		/*
		*	passing the most relevant error message for the database object
		*/
		static string getErrorMessage(sqlite3*);
		/*
		*	on executing the native SQlite C API, it returns the flags in int format,
		*	errors messaged could be obtained from these flags by passing the error code
		*	to this class.
		*/
		static string getErrorMessage(int);	
};

class _NativeFrameworkSQLiteException{
	private:
		string errorMessage;
		string filename;
		string function;
		int line;

		string _getErrorMessage(int errorCode){
			string errorMsg = sqlite3_errstr(errorCode);
			return errorMsg;
		}

		string _getErrorMessage(sqlite3 *database){
			return string(sqlite3_errmsg(database));
		}

		void _printError(){
			cerr << "ERROR: " ;
			cerr << errorMessage << " \nin " << filename << " on line " << line << " in function " << function << endl;
			cerr << "Aborting" << endl;
		}
	public:
		_NativeFrameworkSQLiteException(string, string, string, int);
		_NativeFrameworkSQLiteException(int, string, string, int);
		_NativeFrameworkSQLiteException(sqlite3*, string, string, int);

		string toString();
};

/*
*	SQLiteConfig
*	This class is the important file for the SQLITE Framework
*	the instance of this class provides the connectivity information
*	for the application to connect to the database file.
*/
class _NativeFrameworkSQLiteConfig{
	private:
		string dbPath;
		string initialDB;
	public:
		// the constructors
		_NativeFrameworkSQLiteConfig();
		_NativeFrameworkSQLiteConfig(string, string);
		_NativeFrameworkSQLiteConfig(char*, char*);

		// setter functions
		void setDatabasePath(string);
		void setDatabasePath(char*);
		void setInitialDatabase(string);
		void setInitialDatabase(char*);

		// getter functions
		string getDatabasePath();
		string getInitialDatabase();
};

#ifndef NATIVE_FRAMEWORK_COLLECTIONS
	#include "_NativeFrameworkCollections.h"	//used by Tuple class and Table class
#endif
#ifndef NATIVE_FRAMEWORK_DATASTRUCTURE
	#include "_NativeFrameworkDS.h"	// tuple requires variant type
#endif
/*
*	The Tuple and Table classes are the Data structures for the SQLite Result.
*	The tuple represents an individual row of a table and Table represents the collection
*	of Tuple (literally and logically). 
*/

class _NativeFrameworkSQLiteTuple{
	private:
		_NativeFrameworkCollectionsLinkedList<_NativeFrameworkDSVariant> columnValues;	// contains a single tuple 
	public:
		void addColumn(int);
		void addColumn(double);
		void addColumn(char*);
		void addColumn(string);
		void addNull();

		long getNumber(int);
		double getDecimal(int);
		string getString(int);
		_NativeFrameworkDSVariant getVariant(int);
		int columnsCount();
};

/*
*	LINQ_SQLITE_API_TABLE class the table class contains a collection
*	of LINQ_SQLITE_API_TUPLE with the help of 
*	LINQ_NATIVE_COLLECTIONS_GENERICLINKEDLIST<> aliased to _NativeFrameworkCollectionsLinkedList
*/
class _NativeFrameworkSQLiteTable{
	private:
		_NativeFrameworkCollectionsLinkedList<_NativeFrameworkSQLiteTuple> tuplesCollection;
	public:
		// add tuple to the collection
		void addTuple(_NativeFrameworkSQLiteTuple);
		// get tuple from the list
		_NativeFrameworkSQLiteTuple getTuple(int);
		// get the rows count
		int rowsCount();
};

/*
*	the metadata class
*/
class _NativeFrameworkSQLiteTableMetadata{
	private:

	public:
};

/*
*	THE RESULT CLASS
*	this class holds the results of the execution of a statement
*/
class _NativeFrameworkSQLiteResult{
	private:
		// the table could be virtual i.e. the query may contain filters for data fetching
		_NativeFrameworkSQLiteTable virtualTuples;
		sqlite3_stmt *statement;
		sqlite3 *application;

		int iteratorIndex;
		bool accessFlag;
		_NativeFrameworkSQLiteTuple currentTuple;

		/*
		*	Initialize the table data
		*/
		void _initTable();
		void _reset();
	public:
		/*
		*	constructor for this class
		*/
		_NativeFrameworkSQLiteResult();
		//	static method to get the proper instance of this class
		static _NativeFrameworkSQLiteResult* getInstance(sqlite3_stmt*);
		static _NativeFrameworkSQLiteResult* getInstance(sqlite3_stmt*, sqlite3*);
		//	still to be implemented
		_NativeFrameworkSQLiteTableMetadata getMetadata();

		bool hasNextRow();

		// get integer in the current result set at position represented by int index
		long getNumber(int);
		/*
		*	get double in the current result set at position represented by int index
		*/
		double getDecimal(int);
		/*
		*	get string in the current result set at position represented by index
		*/
		string getString(int);
		/*
		*	get data type of the index
		*/
		DataTypes getType(int);

		int rowsCount();
		int columnsCount();
		/*
		*	reset the current result iterator
		*/
		void reset();
};
/*
*	THE STATEMENT CLASS
*/

/*
*	life of a native statement object (NATIVE PERSPECTIVE):
*	1.	create using sqlite3_prepare_v2();
*	2.	bind values to host parameters using the sqlite3_bind_*()
*	3.	run sql query using sqlite3_step()
*	4.	reset statement using sqlite3_reset() and go back to step 2.
*	5.	destroy using sqlite3_finalize().
*/
class _NativeFrameworkSQLiteStatement{
	private:
		sqlite3_stmt *statement;
		sqlite3 *sqliteContext;

		typedef enum __STMTTYPE{EXEC, ROW};

		void _attemptCloseStatement();

		// executing the query
		_NativeFrameworkSQLiteResult* _attemptExecute();
		
		// for non-SELECT statements
		int _attemptExecuteUpdate();

	public:
		/*
		*	constructor for this class
		*/
		_NativeFrameworkSQLiteStatement();
		/*
		*	this contructor is used to init the statement and is done by the
		*	_NativeFrameworkSQLiteApplication.createStatement(string); class
		*/
		_NativeFrameworkSQLiteStatement(sqlite3_stmt*);

		/*
		*	call this method to close this statement
		*/
		void closeStatement();
		/*
		* method to execute the query that returns data
		* This method return instance of _NativeFrameworkSQLiteResult that contains the table data
		*/
		_NativeFrameworkSQLiteResult executeQuery();
		/*
		*	all queries that does not contain SELECT are executed using executeUpdate();
		*	otherwise the exception is thrown.
		*/
		int executeUpdate();
		/*
		*	this is set at the time when application class creates ann instance
		*	of sqlite_statement. This context is required so as this class can
		*	access the sqlite3 application which is required for some operations.
		*/
		void setSQLiteApplicationContext(sqlite3*);
		/*
		*	this is required for internal processing. This instance may be requested
		*	by the classes having instance of sqlite_statement but not sqlite_application
		*	this method provides the native data-structure sqlite3 that is used for various
		*	native processings.
		*/
		sqlite3* getSQLiteApplicationContext();
};

/*
*	The Prepared Statement clas
*	_NativeFrameworkSQLitePreparedStatement
*	The prepared statement is different from normal statement as it could
*	use bounded parameters. The native C-API provides same under sqlite3_stmt
*	but linq sqlite API make it available under different classes.
*	example:
*		case#1:
*		INSERT INTO names VALUES (?, ?, ?);
*			stat.bindInt(1, 1);
*			stat.bindString(2, "gaurav"); 
*			stat.bindDouble(3, 34.78);
*		case#2:
*		INSERT INTO names VALUES(:id, :name, :salary);
*			stat.bindValue("id", 1);
*			stat.bindValue("name", "gaurav sharma");
*			stat.bindValue("salary", 34.78);
*/

/*
*	parameters indexing starts with 1 not 0
*/
class _NativeFrameworkSQLitePreparedStatement{
	private:
		sqlite3_stmt *statement;
		sqlite3 *sqliteContext;

		//	for non-SELECT queries
		int _attemptExecuteUpdate();
		//	for SELECT queries
		_NativeFrameworkSQLiteResult _attemptExecute_query();
		/*
		*	for numerically indexed statements (with ?)
		*/
		void _attemptBind(int, int);
		void _attemptBind(int, short);
		void _attemptBind(int, long);	// typedef long long int sqlite3_int64
		void _attemptBind(int, float);
		void _attemptBind(int, double);
		void _attemptBind(int, char*);
		void _attemptBindNull(int);

		// count the number of bounded params
		int _getParametersCount();
		// get the index of named parameter
		int _getIndexOf(char*);
		// get the name of the parameter at specified index
		char* _getParameterName(int);

		/*
		*	close must clear the bindings before calling
		*	sqlite3_reset(sqlite3_stmt*); by calling
		*/
		void _attemptCloseStatement();

		public:
		/*
		*	The default constructor
		*/
		_NativeFrameworkSQLitePreparedStatement();
		/*
		*	Init using sqlite3_stmt
		*/
		_NativeFrameworkSQLitePreparedStatement(sqlite3_stmt*);
		_NativeFrameworkSQLitePreparedStatement(sqlite3_stmt**);
		/*
		*	to execute the non-SELECT statements
		*/
		int executeUpdate();
		/*
		*	to execute the SELECT statements
		*/
		_NativeFrameworkSQLiteResult executeQuery();
		/*
		*	close this statement
		*/
		void closeStatement();
		/*
		*	binding functions
		*/
		/*
		*	binding an int value for index int
		*	@Param int for index
		*	@Param int for value
		*/
		void bindInt(int, int);
		/*
		*	binding short to the index int
		*	@Param int for index
		*	@Param short for value
		*/
		void bindShort(int, short);
		/*
		*	binding long to the index
		*/
		void bindLong(int, long);
		/*
		*	binding float to the index
		*/
		void bindFloat(int, float);
		/*
		*	bind double to the index
		*/
		void bindDouble(int, double);
		/*
		*	bind string to the index
		*/
		void bindString(int, string);
		void bindString(int, char*);
		/*
		*	binding null to the index
		*/
		void bindNull(int);

		/*
		*	public methods for binding to named parameters
		*	example:
		*		INSERT INTO emp VALUES(:id, :name, :dept, :salary);
		*			stat.bindValue("id", 1);
		*			stat.bindValue("dept", "Development");
		*			stat.bindValue("name", "Gaurav Sharma");
		*	all these methods calls to 
		*/
		void bindValue(char*, int);
		void bindValue(char*, short);
		void bindValue(char*, long);
		void bindValue(char*, double);
		void bindValue(char*, float);
		void bindValue(char*, string);
		void bindValue(char*, char*);
		void bindNullValue(char*);

		/*
		*	setting the sqlite3 application's context for internal use.
		*/
		void setSQLiteApplicationContext(sqlite3*);
		sqlite3* getSQLiteApplicationContext();
};

/*
*	THE APPLICATION CLASS
*	the class uses the linq_sqlite_config's instance to init the database connection
*/
class _NativeFrameworkSQLiteApplication{
	private:
		sqlite3 *database;
		sqlite3_stmt *statement;
		_NativeFrameworkSQLiteConfig config;
		//connection params
		string databasePath;
		string initialDatabase;
		bool useURI;
		bool initialized;
		bool activeStat;

		/*
		*	the native API handler and Public calls handler for 
		*	connecting to the database
		*/

		void _attemptConnecting();

		/*
		*	check if the database exists
		*/
		bool _databaseExists(string);
		// unsafe
		// call this function only to create the database and only if the database does
		// not exist. That's why the function is private. If not called properly, exeception
		// will occur. if database already exists and the function is called then the execption
		// will occur.
		void _attemptCreateDatabase(char*);
		/*
		*	the native API handler and public calls handler for closing the
		*	database connection.
		*/
		void _attemptClose();
		// instance of sqlite statement
		/*
		*	init using sqlite3_prepare() or sqlite3_prepare_v2()
		*
		*		int sqlite3_prepare(sqlite3 *database,
		*							const char *query,
		*							int sizeOfQuery,
		*							sqlite3_stmt **statement,
		*							const char **nextQuery);
		*		int sqlite3_prepare_v2(
		*							sqlite3 *database,
		*							const char *query,
		*							int sizeOfQuery,
		*							sqlite3_stmt **statement,
		*							const char **nextQuery);
		*
		*	
		*/
		_NativeFrameworkSQLiteStatement _attemptCreateStatement(const char*);
		/*
		*	this function is identical to the _attemptCreateStatement() except
		*	the type that it returns.
		*/
		_NativeFrameworkSQLitePreparedStatement _attemptPrepareStatement(const char*);
	public:
		/*
		*	connect to the database
		*/
		void initializeConnection(_NativeFrameworkSQLiteConfig);
		/*
		*	close the connection
		*/
		void closeConnection();

		/*
		*	creating a simple sqlite statement
		*/
		_NativeFrameworkSQLiteStatement createStatement(string);
		_NativeFrameworkSQLiteStatement createStatement(char*);

		/*
		*	creating a sqlite prepared statement
		*/
		_NativeFrameworkSQLitePreparedStatement prepareStatement(string);
		_NativeFrameworkSQLitePreparedStatement prepareStatement(char*);

		// checking the connection is open or not
		bool isOpen();
		/*
		*	V1.1
		*	open a new database
		*/
		void openDatabase(char*);
		/*
		*	V1.1
		*	checks if the database exists
		*/
		bool exists(char*);
		/*
		*	create a database based on the configuration params
		*/
		void createDatabase(char*);
		/*
		*	check if database exists
		*/
		bool databaseExists();
		/*
		*	to get the instance of configuration
		*/
		_NativeFrameworkSQLiteConfig getConfig();
};

/*
*	class to create the auto connection to the database
*/

class _NativeFrameworkSQLiteConnection{
	public:
		static _NativeFrameworkSQLiteApplication getConnection(char*, char*);
};