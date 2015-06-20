#ifndef NATIVE_FRAMEWORK_SQLITEIMPL
#define NATIVE_FRAMEWORK_SQLITEIMPL

#include "../includes/_NativeFrameworkSQLite.h"

#ifndef EXCEPTION_DEFS
	#define EXCEPTION_DEFS
	
	#define __ERRFILE __FILE__
	#define __ERRFUNC __func__
	#define __ERRLINE __LINE__
#endif

// the exceptions defin.
#define _SQLiteException(Error_Message) _NativeFrameworkSQLiteException(Error_Message, __ERRFILE, __ERRFUNC, __ERRLINE)
#define _SQLiteExceptionFor(Error_Code) _NativeFrameworkSQLiteException(Error_Code, __ERRFILE, __ERRFUNC, __ERRLINE)
#define _SQLiteSystemException(SQLite_Application)

/*
*	ERRHANDLER IMPLEMENTATION
*/
string _NativeFrameworkSQLiteErrMsgHandler::getErrorMessage(sqlite3 *database){
	return  string(sqlite3_errmsg(database));
}
string _NativeFrameworkSQLiteErrMsgHandler::getErrorMessage(int errorCode){
	return string(sqlite3_errstr(errorCode));
}

/*
*	EXCEPTION IMPLEMENTATION
*/

_NativeFrameworkSQLiteException::_NativeFrameworkSQLiteException(string errorMessage,
	string filename,
	string function,
	int line){

	this -> errorMessage = errorMessage;
	this -> filename = filename;
	this -> function = function;
	this -> line = line;

	_printError();
}

_NativeFrameworkSQLiteException::_NativeFrameworkSQLiteException(int errorCode,
	string filename,
	string function,
	int line){

	this -> errorMessage = _getErrorMessage(errorCode);
	this -> filename = filename;
	this -> function = function;
	this -> line = line;

	_printError();
}

_NativeFrameworkSQLiteException::_NativeFrameworkSQLiteException(sqlite3 *database,
	string filename,
	string function,
	int line){

	this -> errorMessage = _getErrorMessage(database);
	this -> filename = filename;
	this -> function = function;
	this -> line = line;

	_printError();
}

string _NativeFrameworkSQLiteException::toString(){
	return errorMessage;
}

/*
*	SQLiteConfig IMPLEMENTATION
*/

_NativeFrameworkSQLiteConfig::_NativeFrameworkSQLiteConfig(){
	dbPath = "";
	initialDB = "";
}
_NativeFrameworkSQLiteConfig::_NativeFrameworkSQLiteConfig(string dbPath,
	string initialDB){
	this -> dbPath = dbPath;
	this -> initialDB = initialDB;
}
_NativeFrameworkSQLiteConfig::_NativeFrameworkSQLiteConfig(char *dbPath, char *initialDb){
	this -> dbPath = dbPath;
	this -> initialDB = initialDb;
}
void _NativeFrameworkSQLiteConfig::setDatabasePath(string dbPath){
	this -> dbPath = dbPath;
}
void _NativeFrameworkSQLiteConfig::setDatabasePath(char *dbPath){
	this -> dbPath = dbPath;
}
void _NativeFrameworkSQLiteConfig::setInitialDatabase(string initialDB){
	this -> initialDB = initialDB;
}
void _NativeFrameworkSQLiteConfig::setInitialDatabase(char *initialDB){
	this -> initialDB = initialDB;
}
string _NativeFrameworkSQLiteConfig::getDatabasePath(){
	return dbPath;
}
string _NativeFrameworkSQLiteConfig::getInitialDatabase(){
	return initialDB;
}

/*
*	SQLiteTuple IMPLEMENTATION
*/

void _NativeFrameworkSQLiteTuple::addColumn(int value){
	columnValues.addItem(_NativeFrameworkDSVariant(value));
}
void _NativeFrameworkSQLiteTuple::addColumn(double value){
	columnValues.addItem(_NativeFrameworkDSVariant(value));
}
void _NativeFrameworkSQLiteTuple::addColumn(char *value){
	columnValues.addItem(_NativeFrameworkDSVariant(value));
}
void _NativeFrameworkSQLiteTuple::addColumn(string value){
	columnValues.addItem(_NativeFrameworkDSVariant(value));
}
void _NativeFrameworkSQLiteTuple::addNull(){
}

_NativeFrameworkDSVariant _NativeFrameworkSQLiteTuple::getVariant(int index){
	if(index >= 0 && index < columnValues.size())
		return columnValues.getItem(index);
}

long _NativeFrameworkSQLiteTuple::getNumber(int index){
	try{
		
		int value = columnValues.getItem(index).getInteger();
		return value;
	}catch(_NativeFrameworkCollectionsException error){

		// error with the linked list 
		throw _SQLiteException(string("Collection Error: ")+ string(error.toString()));
	}catch(_NativeFrameworkDSException error){
		
		// error with the variant data structure
		cout << "Type is: " << columnValues.getItem(index).getType() << endl; 
		throw _SQLiteException(string("Variant Error: ")+ string(error.toString()));
	}
	
	throw _SQLiteException("Int value error.");
}
double _NativeFrameworkSQLiteTuple::getDecimal(int index){
	try{
		double value = columnValues.getItem(index).getDecimal();
		return value;
	}catch(_NativeFrameworkCollectionsException error){

		// error with the collections
		throw _SQLiteException(string("Collection Error: ")+ string(error.toString()));
	}catch(_NativeFrameworkDSException error){
		
		// error witn the data structure's variant
		throw _SQLiteException(string("Variant Error: ")+ string(error.toString()));
	}
	throw _SQLiteException("Double value error.");
	
}
string _NativeFrameworkSQLiteTuple::getString(int index){
	try{
		string value = columnValues.getItem(index).getString();
		return value;
	}catch(_NativeFrameworkCollectionsException error){
		throw _SQLiteException(string("Collection Error: ")+ string(error.toString()));
	}catch(_NativeFrameworkDSException error){
		throw _SQLiteException(string("Variant Error: ")+ string(error.toString()));
	}
	throw _SQLiteException("String value error.");
}
int _NativeFrameworkSQLiteTuple::columnsCount(){
	return columnValues.size();
}

/*
*	SQLiteTable IMPLEMENTATION
*/

void _NativeFrameworkSQLiteTable::addTuple(_NativeFrameworkSQLiteTuple singleTuple){
	try{
		tuplesCollection.addItem(singleTuple);
	}catch(_NativeFrameworkCollectionsException error){
		throw _SQLiteException(string("Error in initializing table.")+ string(error.toString()));
	}
}

_NativeFrameworkSQLiteTuple _NativeFrameworkSQLiteTable::getTuple(int index){
	if(index < 0 || index >= tuplesCollection.size())
		throw _SQLiteException("Acess Error: Accessing out of bounds.");
	
	_NativeFrameworkSQLiteTuple tempTuple;
	try{
		tempTuple = tuplesCollection.getItem(index);
	}catch(_NativeFrameworkCollectionsException error){
		throw _SQLiteException(string("Collection Error: ")+ string(error.toString()));
	}
	return tempTuple;
}
int _NativeFrameworkSQLiteTable::rowsCount(){
	return tuplesCollection.size();
}

/*
*	SQLiteTableMetadata IMPLEMENTATION
*/

//	nothing yet

/*
*	SQLiteResult IMPLEMENTATION
*/

void _NativeFrameworkSQLiteResult::_initTable(){
	int executeFlag = sqlite3_step(statement);
	if(executeFlag == SQLITE_ROW){
		// get the metadata
		int columnsCount = sqlite3_column_count(statement);
		do{
			// iterate over the columns
			_NativeFrameworkSQLiteTuple indTuple;
			for(int colIndex = 0; colIndex < columnsCount; colIndex += 1){

				int typeFlag = sqlite3_column_type(statement, colIndex);

				switch(typeFlag){
					case SQLITE_INTEGER:{
						int intValue = sqlite3_column_int(statement, colIndex);
						//cout << intValue << "Added to column " << colIndex;
						indTuple.addColumn(intValue);
						break;
					}
					case SQLITE_FLOAT:{
						double doubleValue = sqlite3_column_double(statement, colIndex);
						//cout << doubleValue << "Added to column " << colIndex;
						indTuple.addColumn(doubleValue);
						break;
					}
					case SQLITE_TEXT:{
						string stringValue = (const char*)sqlite3_column_text(statement, colIndex);
						//cout << stringValue << "Added to column " << colIndex;
						indTuple.addColumn(stringValue);
						break;
					}
					case SQLITE_NULL:{
						break;
					}
					default:{
						cout << "Type " << typeFlag << "Detected!" << endl;
					}
				}
			}
			virtualTuples.addTuple(indTuple);
		}while((executeFlag = sqlite3_step(statement)) == SQLITE_ROW);
	}
	else if(executeFlag == SQLITE_DONE){
		_reset();
		throw _SQLiteException("Statement does not return table data. Try using statement.executeUpdate()");
	}else if(executeFlag == SQLITE_BUSY){
		throw _SQLiteException("Cannot write to the database as the database is acquired by some other process.");
	}else if(executeFlag == SQLITE_ERROR){
		_reset();
		//	NEED MODIFICATION AT THIS POINT
		//	throw _SQLiteException(string(sqlite3_errmsg(sqlite3*)));
		throw _SQLiteException("Error executing this statement");
	}else if(executeFlag == SQLITE_MISUSE){
		throw _SQLiteException("Statement was called inappropriately. Perhaps it is called on a statement that has already been closed or on one that had previously return error or executed. Or it could be case that the same database connection is being used by two or more threads at the same moment in time.");
	}
}
void _NativeFrameworkSQLiteResult::_reset(){
	sqlite3_reset(statement);
}

_NativeFrameworkSQLiteResult::_NativeFrameworkSQLiteResult(){
	this -> iteratorIndex = -1;
	this -> accessFlag = true;
}

_NativeFrameworkSQLiteResult* _NativeFrameworkSQLiteResult::getInstance(sqlite3_stmt *statement){
	_NativeFrameworkSQLiteResult *result = new _NativeFrameworkSQLiteResult();
	
	result -> statement = statement;
	result -> _initTable();

	return result;
}
/*
*	NEED IMPLEMENTATION
*/
_NativeFrameworkSQLiteTableMetadata _NativeFrameworkSQLiteResult::getMetadata(){}

bool _NativeFrameworkSQLiteResult::hasNextRow(){
	if(++iteratorIndex != virtualTuples.rowsCount()){
		currentTuple = virtualTuples.getTuple(iteratorIndex);
		return true;
	}
	accessFlag = false;
	return false;
}

long _NativeFrameworkSQLiteResult::getNumber(int index){
	DataTypes type = currentTuple.getVariant(index).getType();
	if(accessFlag)
		if(type == LONG)
			return currentTuple.getNumber(index);
	string type_name = type == DOUBLE ? "long" :
						type == CHAR ? "char" :
						type == STRING ? "string" :
						type == BOOLEAN ? "boolean" : "undefined";
	cerr << "Cannot return " << type_name << " instead of int" << endl;
	throw _SQLiteException("Type cast exception");
}
double _NativeFrameworkSQLiteResult::getDecimal(int index){
	//if(accessFlag) return currentTuple.getDecimal(index);
	DataTypes type = currentTuple.getVariant(index).getType();
	if(accessFlag)
		if(type == DOUBLE)
			return currentTuple.getDecimal(index);
	string type_name = type == LONG ? "long" :
						type == CHAR ? "char" :
						type == STRING ? "string" :
						type == BOOLEAN ? "boolean" : "undefined";
	cerr << "Cannot return " << type_name << " instead of double" << endl;
	throw _SQLiteException("Type cast exception");	
}
string _NativeFrameworkSQLiteResult::getString(int index){
	DataTypes type = currentTuple.getVariant(index).getType();
	if(accessFlag) 
		if(type == STRING)
			return currentTuple.getString(index);
	string type_name = type == LONG ? "long" :
						type == CHAR ? "char" :
						type == DOUBLE ? "double" :
						type == BOOLEAN ? "boolean" : "undefined";
	cerr << "Cannot return " << type_name << " instead of string" << endl;
	throw _SQLiteException("Type cast exception");
}

DataTypes _NativeFrameworkSQLiteResult::getType(int index){
	if(currentTuple.columnsCount() >= 0 && index >= 0 && index <= currentTuple.columnsCount())
		return currentTuple.getVariant(index).getType();
	throw _SQLiteException("Index out of bounds error.");
}

int _NativeFrameworkSQLiteResult::rowsCount(){
	return virtualTuples.rowsCount();
}
int _NativeFrameworkSQLiteResult::columnsCount(){
	return virtualTuples.getTuple(0).columnsCount();
}

void _NativeFrameworkSQLiteResult::reset(){
	this -> iteratorIndex = -1;
}

/*
*	SQLiteStatement IMPLEMENTATION
*/

void _NativeFrameworkSQLiteStatement::_attemptCloseStatement(){
	int closeFlag = sqlite3_finalize(statement);
	if(closeFlag != SQLITE_OK)
		throw _SQLiteExceptionFor(closeFlag);
}

// executing the query
_NativeFrameworkSQLiteResult* _NativeFrameworkSQLiteStatement::_attemptExecute(){
	_NativeFrameworkSQLiteResult *result;
	try{
		result = _NativeFrameworkSQLiteResult::getInstance(statement);
		return result;
	}catch(_NativeFrameworkSQLiteException error){
		throw _SQLiteException(error.toString());
	}
}

// for non-SELECT statements
int _NativeFrameworkSQLiteStatement::_attemptExecuteUpdate(){
	int execUpdateFlag = sqlite3_step(statement);
	if(execUpdateFlag != SQLITE_DONE)
		throw _SQLiteExceptionFor(execUpdateFlag);
	return sqlite3_changes(this -> sqliteContext);
}

_NativeFrameworkSQLiteStatement::_NativeFrameworkSQLiteStatement(){}
_NativeFrameworkSQLiteStatement::_NativeFrameworkSQLiteStatement(sqlite3_stmt *statement){
	this -> statement = statement;
}

_NativeFrameworkSQLiteResult _NativeFrameworkSQLiteStatement::executeQuery(){
	try{
		_NativeFrameworkSQLiteResult _tempResult = *_attemptExecute();
		return _tempResult;
	}catch(_NativeFrameworkSQLiteException error){
		throw _SQLiteException(string("Error executing query: ")+ string(error.toString()));
	}
}

int _NativeFrameworkSQLiteStatement::executeUpdate(){
	try{
		int updated = _attemptExecuteUpdate();
		return updated;
	}catch(_NativeFrameworkSQLiteException error){
		throw _SQLiteException(string("Error executing update: ")+ string(error.toString()));
	}
}

void _NativeFrameworkSQLiteStatement::setSQLiteApplicationContext(sqlite3 *dbContext){
	this -> sqliteContext = dbContext;
}
sqlite3* _NativeFrameworkSQLiteStatement::getSQLiteApplicationContext(){
	return this -> sqliteContext;
}

void _NativeFrameworkSQLiteStatement::closeStatement(){
	try{
		_attemptCloseStatement();
	}catch(_NativeFrameworkSQLiteException error){
		throw _SQLiteException(string("Error closing statement: ")+ string(error.toString()));
	}
}

/*
*	SQLitePreparedStatement IMPLEMENTATION
*/
_NativeFrameworkSQLitePreparedStatement::_NativeFrameworkSQLitePreparedStatement(){
	statement = nullptr;
}
_NativeFrameworkSQLitePreparedStatement::_NativeFrameworkSQLitePreparedStatement(sqlite3_stmt *statement){
	this -> statement = statement;
}
_NativeFrameworkSQLitePreparedStatement::_NativeFrameworkSQLitePreparedStatement(sqlite3_stmt **statement){
	this -> statement = *statement;
}

// the private and public methods
/*
*	LINQ_SQLITE_PREPARED_STATEMENT implementation
*/

//	_NativeFrameworkSQLitePreparedStatement is alias for _NativeFrameworkSQLitePreparedStatement
//PRIVATE METHODS

int _NativeFrameworkSQLitePreparedStatement::_attemptExecuteUpdate(){
	int execUpdateFlag = sqlite3_step(statement);
	if(execUpdateFlag != SQLITE_DONE)
		throw _SQLiteExceptionFor(execUpdateFlag);
	return sqlite3_changes(sqliteContext);
}
_NativeFrameworkSQLiteResult _NativeFrameworkSQLitePreparedStatement::_attemptExecute_query(){
	_NativeFrameworkSQLiteResult result;
	try{
		result = *_NativeFrameworkSQLiteResult::getInstance(statement);
		return result;
	}catch(_NativeFrameworkSQLiteException error){
		throw _SQLiteException(string("Cannot obtain result: ")+ string(error.toString()));
	}
}

void _NativeFrameworkSQLitePreparedStatement::_attemptBind(int index, int value){
	int bindFlag = sqlite3_bind_int(this -> statement, index, value);
	switch(bindFlag){
		case SQLITE_OK: break;
		case SQLITE_RANGE: throw _SQLiteException("Parameter index is out of range.");
		case SQLITE_NOMEM: throw _SQLiteException("Unable to allocate memory to perform operation");
		default: throw _SQLiteException("Unknown binding error.");
	}
}
void _NativeFrameworkSQLitePreparedStatement::_attemptBind(int index, short value){
	_attemptBind(index, (int) value);
}
void _NativeFrameworkSQLitePreparedStatement::_attemptBind(int index, long value){
	int bindFlag = sqlite3_bind_int64(statement,index , (long long int)value);
	switch(bindFlag){
		case SQLITE_OK: break;
		case SQLITE_RANGE: throw _SQLiteException("Parameter index is out of range.");
		case SQLITE_NOMEM: throw _SQLiteException("Unable to allocate memory to perform operation");
		default: throw _SQLiteException("Unknown binding error.");
	}
}
void _NativeFrameworkSQLitePreparedStatement::_attemptBind(int index, float value){
	int bindFlag = sqlite3_bind_double(statement, index, (double)value);
	switch(bindFlag){
		case SQLITE_OK: break;
		case SQLITE_RANGE: throw _SQLiteException("Parameter index is out of range.");
		case SQLITE_NOMEM: throw _SQLiteException("Unable to allocate memory to perform operation");
		default: throw _SQLiteException("Unknown binding error.");
	}
}
void _NativeFrameworkSQLitePreparedStatement::_attemptBind(int index, double value){
	int bindFlag = sqlite3_bind_double(statement, index, value);
	switch(bindFlag){
		case SQLITE_OK: break;
		case SQLITE_RANGE: throw _SQLiteException("Parameter index is out of range.");
		case SQLITE_NOMEM: throw _SQLiteException("Unable to allocate memory to perform operation");
		default: throw _SQLiteException("Unknown binding error");
	}
}
void _NativeFrameworkSQLitePreparedStatement::_attemptBind(int index, char *value){
	int bindFlag = sqlite3_bind_text(statement, index, value, -1, SQLITE_TRANSIENT);
	switch(bindFlag){
		case SQLITE_OK: break;
		case SQLITE_RANGE: throw _SQLiteException("Parameter index is out of range.");
		case SQLITE_NOMEM: throw _SQLiteException("Unable to allocate memory to perform operation");
		default: throw _SQLiteException("Unknown binding error");
	}	
}
void _NativeFrameworkSQLitePreparedStatement::_attemptBindNull(int index){
	int bindFlag = sqlite3_bind_null(statement, index);
	switch(bindFlag){
		case SQLITE_OK: break;
		case SQLITE_RANGE: throw _SQLiteException("Parameter index is out of range.");
		case SQLITE_NOMEM: throw _SQLiteException("Unable to allocate memory to perform operation");
		default: throw _SQLiteException("Unknown binding error");
	}
}

int _NativeFrameworkSQLitePreparedStatement::_getParametersCount(){
	return sqlite3_bind_parameter_count(statement);
}
int _NativeFrameworkSQLitePreparedStatement::_getIndexOf(char *paramName){
	string appendDollar = "$";
	string appendColon = ":";
	string appendAtTheRate = "@";

	int initFlag = sqlite3_bind_parameter_index(statement,
		(appendDollar + string(paramName)).c_str());
	if(initFlag != 0) return initFlag;
	else{
		initFlag = sqlite3_bind_parameter_index(statement,
		(appendColon + string(paramName)).c_str());
		if(initFlag != 0) return initFlag;
		else{
			initFlag = sqlite3_bind_parameter_index(statement,
			(appendAtTheRate + string(paramName)).c_str());
			if(initFlag != 0) return initFlag;
			else
				throw _SQLiteException(string("No parameter with name \"") + string(paramName) + string("\" found."));
		}
	}

	/*string param_mod = paramName;
	int get_flag = sqlite3_bind_parameter_index(statement, paramName);
	return (get_flag != 0) ? get_flag : throw _SQLiteException(string("No parameter with \"")+ string(paramName) + "\" Name"); */
}
char* _NativeFrameworkSQLitePreparedStatement::_getParameterName(int index){
	return (sqlite3_bind_parameter_name(statement, index) != NULL) ?
		(char*)sqlite3_bind_parameter_name(statement, index) : 
		throw _SQLiteException("No parameter name found at specified index.");
}

void _NativeFrameworkSQLitePreparedStatement::_attemptCloseStatement(){
	int clearBindingsFlag = sqlite3_clear_bindings(statement);
	if(clearBindingsFlag == SQLITE_OK){
		int resetFlag = sqlite3_reset(statement);
		if(resetFlag != SQLITE_OK)
			throw _SQLiteException("Error closing prepared statement");
	}else
		throw _SQLiteException("Error reset of binding parameters");
}

//PUBLIC METHODS
int _NativeFrameworkSQLitePreparedStatement::executeUpdate(){
	try{
		int updated = _attemptExecuteUpdate();
		return updated;
	}catch(_NativeFrameworkSQLiteException error){
		throw _SQLiteException(string("Cannot execute query: ")+ string(error.toString()));
	}
}
_NativeFrameworkSQLiteResult _NativeFrameworkSQLitePreparedStatement::executeQuery(){
	try{
		_NativeFrameworkSQLiteResult result = _attemptExecute_query();
		return result;
	}catch(_NativeFrameworkSQLiteException error){
		throw _SQLiteException(string("Cannot execute query: ")+ string(error.toString()));
	}
}

void _NativeFrameworkSQLitePreparedStatement::bindInt(int index, int value){
	try{
		_attemptBind(index, value);
	}catch(_NativeFrameworkSQLiteException error){
		throw _SQLiteException("Error binding int: "+ error.toString());
	}
}
void _NativeFrameworkSQLitePreparedStatement::bindShort(int index, short value){
	try{
		_attemptBind(index, value);
	}catch(_NativeFrameworkSQLiteException error){
		throw _SQLiteException("Error binding short: "+ error.toString());
	}	
}
void _NativeFrameworkSQLitePreparedStatement::bindLong(int index, long value){
	try{
		_attemptBind(index, value);
	}catch(_NativeFrameworkSQLiteException error){
		throw _SQLiteException("Error binding long: "+ error.toString());
	}
}
void _NativeFrameworkSQLitePreparedStatement::bindDouble(int index, double value){
	try{
		_attemptBind(index, value);
	}catch(_NativeFrameworkSQLiteException error){
		throw _SQLiteException("Error binding double: "+ error.toString());
	}
}
void _NativeFrameworkSQLitePreparedStatement::bindFloat(int index, float value){
	try{
		_attemptBind(index, value);
	}catch(_NativeFrameworkSQLiteException error){
		throw _SQLiteException("Error binding float: "+ error.toString());
	}	
}
void _NativeFrameworkSQLitePreparedStatement::bindString(int index, string value){
	try{
		_attemptBind(index, (char*)value.c_str());
	}catch(_NativeFrameworkSQLiteException error){
		throw _SQLiteException("Error binding string: "+ error.toString());
	}	
}
void _NativeFrameworkSQLitePreparedStatement::bindString(int index, char* value){
	try{
		_attemptBind(index, value);
	}catch(_NativeFrameworkSQLiteException error){
		throw _SQLiteException("Error binding string: "+ error.toString());
	}	
}
void _NativeFrameworkSQLitePreparedStatement::bindNull(int index){
	try{
		_attemptBindNull(index);
	}catch(_NativeFrameworkSQLiteException error){
		throw _SQLiteException("Error binding null: "+ error.toString());
	}	
}

//	bindings for named parameters
void _NativeFrameworkSQLitePreparedStatement::bindValue(char *paramName, int value){
	try{
		int paramIndex = _getIndexOf(paramName);
		bindInt(paramIndex, value);		
	}catch(_NativeFrameworkSQLiteException error){
		string pname = paramName;
		string errorMessage = string("Error binding int value for parameter ") + paramName + string(": ");
		throw _SQLiteException(errorMessage + error.toString());
	}
}
void _NativeFrameworkSQLitePreparedStatement::bindValue(char *paramName, short value){
	try{
		int paramIndex = _getIndexOf(paramName);
		bindShort(paramIndex, value);		
	}catch(_NativeFrameworkSQLiteException error){
		string pname = paramName;
		string errorMessage = string("Error binding short value for parameter ") + paramName + string(": ");
		throw _SQLiteException(errorMessage + error.toString());
	}	
}
void _NativeFrameworkSQLitePreparedStatement::bindValue(char *paramName, long value){
	try{
		int paramIndex = _getIndexOf(paramName);
		bindLong(paramIndex, value);		
	}catch(_NativeFrameworkSQLiteException error){
		string pname = paramName;
		string errorMessage = string("Error binding long value for parameter ") + paramName + string(": ");
		throw _SQLiteException(errorMessage + error.toString());
	}	
}
void _NativeFrameworkSQLitePreparedStatement::bindValue(char *paramName, double value){
	try{
		int paramIndex = _getIndexOf(paramName);
		bindDouble(paramIndex, value);		
	}catch(_NativeFrameworkSQLiteException error){
		string pname = paramName;
		string errorMessage = string("Error binding double value for parameter ") + paramName + string(": ");
		throw _SQLiteException(errorMessage + error.toString());
	}	
}
void _NativeFrameworkSQLitePreparedStatement::bindValue(char *paramName, float value){
	try{
		int paramIndex = _getIndexOf(paramName);
		bindFloat(paramIndex, value);		
	}catch(_NativeFrameworkSQLiteException error){
		string pname = paramName;
		string errorMessage = string("Error binding float value for parameter ") + paramName + string(": ");
		throw _SQLiteException(errorMessage + error.toString());
	}	
}
void _NativeFrameworkSQLitePreparedStatement::bindValue(char *paramName, char *value){
	try{
		int paramIndex = _getIndexOf(paramName);
		bindString(paramIndex, value);		
	}catch(_NativeFrameworkSQLiteException error){
		string pname = paramName;
		string errorMessage = string("Error binding string value for parameter ") + paramName + string(": ");
		throw _SQLiteException(errorMessage + error.toString());
	}	
}
void _NativeFrameworkSQLitePreparedStatement::bindValue(char *paramName, string value){
	try{
		int paramIndex = _getIndexOf(paramName);
		bindString(paramIndex, value);		
	}catch(_NativeFrameworkSQLiteException error){
		string pname = paramName;
		string errorMessage = string("Error binding string value for parameter ") + paramName + string(": ");
		throw _SQLiteException(errorMessage + error.toString());
	}	
}
/*void bindNullValue(char *paramName){
	try{
		int paramIndex = _getIndexOf(paramName);
		bindNull(paramIndex);		
	}catch(_NativeFrameworkSQLiteException error){
		string pname = paramName;
		string errorMessage = string("Error binding null value for parameter ") + paramName + string(": ");
		throw _SQLiteException(errorMessage + error.toString());
	}	
}*/
void _NativeFrameworkSQLitePreparedStatement::setSQLiteApplicationContext(sqlite3 *sqliteContext){
	this -> sqliteContext = sqliteContext;
}
sqlite3* _NativeFrameworkSQLitePreparedStatement::getSQLiteApplicationContext(){
	return sqliteContext;
}
void _NativeFrameworkSQLitePreparedStatement::closeStatement(){
	try{
		_attemptCloseStatement();
	}catch(_NativeFrameworkSQLiteException error){
		throw _SQLiteException(string("Error closing prepared statement: ")+ error.toString());
	}
}

/*
*	SQLiteApplication IMPLEMENTATION
*/

void _NativeFrameworkSQLiteApplication::_attemptConnecting(){
	// the flags for sqlite3 executions are saved here
	int executingFlags;
	if(databasePath.length() != 0){
		string finalPath = initialDatabase.length() > 0 ? databasePath + "\\" + initialDatabase + string(".db") : databasePath;

		if(_databaseExists(finalPath)){

			executingFlags = sqlite3_open(finalPath.c_str(), &database);

			if(executingFlags != SQLITE_OK){
				string err = _NativeFrameworkSQLiteErrMsgHandler::getErrorMessage(executingFlags);
				if(err == "unable to open database file"){
					string errMsg = "Database " + initialDatabase + " does not exists.";
					throw _SQLiteException(errMsg);
				}else
					throw _SQLiteException(err);
			}else	initialized = true;
		}else{
			string errMsg = "Database " + initialDatabase + " does not exists.";
			throw _SQLiteException(errMsg);
		}
	}
}

/*
*	check if the database exists
*/
bool _NativeFrameworkSQLiteApplication::_databaseExists(string qualifiedPath){
	sqlite3 *tempDatabase;
	int execFlags = sqlite3_open_v2(qualifiedPath.c_str(), &tempDatabase,
		SQLITE_OPEN_READONLY, NULL);
	if(execFlags == SQLITE_OK) 
		return true;
	sqlite3_close(tempDatabase);
	return false;
}
// unsafe
// call this function only to create the database and only if the database does
// not exist. That's why the function is private. If not called properly, exeception
// will occur. if database already exists and the function is called then the execption
// will occur.
void _NativeFrameworkSQLiteApplication::_attemptCreateDatabase(char *createDatabaseName){
	string finalPath = initialDatabase.length() > 0 ? databasePath + "\\" + createDatabaseName + string(".db") : databasePath;
	sqlite3 *tmpDatabase;
	int execFlags = sqlite3_open(finalPath.c_str(), &tmpDatabase);
	if(execFlags != SQLITE_OK){
		sqlite3_close(tmpDatabase);
		throw _SQLiteException(execFlags);
	}
	sqlite3_close(tmpDatabase);
}

/*
*	the native API handler and public calls handler for closing the
*	database connection.
*/
void _NativeFrameworkSQLiteApplication::_attemptClose(){
	if(activeStat)
		sqlite3_finalize(statement);
	int closeFlag = sqlite3_close(database);
	if(closeFlag != SQLITE_OK){
		if(closeFlag == SQLITE_BUSY)
			throw _SQLiteException("Close Error: Unfinalized statements exists.");
		throw _SQLiteExceptionFor(closeFlag);
	}else initialized = false;
}

_NativeFrameworkSQLiteStatement _NativeFrameworkSQLiteApplication::_attemptCreateStatement(const char *query){
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
	/*init the statement*/
	int initFlag = sqlite3_prepare_v2(
						database, 
						query,
						string(query).length(),
						&statement,
						NULL);
	if(initFlag != SQLITE_OK){
		string error = _NativeFrameworkSQLiteErrMsgHandler::getErrorMessage(database);
		if(error == "library routine called out of sequence"){
			//	 the database connection is not initalized!!
			string errMsg = "Database connection not initialized. Try using \n\t _NativeFrameworkSQLiteApplication.initalizeConnection(_NativeFramworkSQLiteConfig)\nbefore using any function in this class.";
			throw _SQLiteException(errMsg);
		}
		throw _SQLiteException(error);
	}

	_NativeFrameworkSQLiteStatement tempStatement(statement);
		tempStatement.setSQLiteApplicationContext(database);

	activeStat = true;
	return tempStatement;
}
/*
*	this function is identical to the _attemptCreateStatement() except
*	the type that it returns.
*/
_NativeFrameworkSQLitePreparedStatement _NativeFrameworkSQLiteApplication::_attemptPrepareStatement(const char *query){
	// init the statement
	int initFlag = sqlite3_prepare_v2(database,
		query,
		string(query).length(),
		&statement,
		NULL);
	if(initFlag != SQLITE_OK){
		string error = _NativeFrameworkSQLiteErrMsgHandler::getErrorMessage(database);
		if(error == "library routine called out of sequence"){
			//	 the database connection is not initalized!!
			string errMsg = "Database connection not initialized. Try using \n\t _NativeFrameworkSQLiteApplication.initalizeConnection(_NativeFramworkSQLiteConfig)\nbefore using any function in this class.";
			throw _SQLiteException(errMsg);
		}
		throw _SQLiteException(error);
	}

	_NativeFrameworkSQLitePreparedStatement tempPStatement(statement);
		tempPStatement.setSQLiteApplicationContext(database);

	activeStat = true;
	return tempPStatement;
}

void _NativeFrameworkSQLiteApplication::initializeConnection(_NativeFrameworkSQLiteConfig configuration){
	config = configuration;

	databasePath = configuration.getDatabasePath();
	initialDatabase = configuration.getInitialDatabase();
	useURI = false;
	initialized = false;
	activeStat = false;
	
	try{
		_attemptConnecting();
	}catch(_NativeFrameworkSQLiteException error){
		cout << "ERROR" << endl;
		throw _SQLiteException(error.toString());
	}
}

void _NativeFrameworkSQLiteApplication::closeConnection(){
	if(initialized)
		_attemptClose();
	else{
		string errMsg = "Database connection not initialized. Try using \n\t _NativeFrameworkSQLiteApplication.initalizeConnection(_NativeFramworkSQLiteConfig)\nbefore using any function in this class.";
		throw _SQLiteException(errMsg);
	}
}

_NativeFrameworkSQLiteStatement _NativeFrameworkSQLiteApplication::createStatement(string query){
	if(initialized)
		return _attemptCreateStatement(query.c_str());
	else{
		string errMsg = "Database connection not initialized. Try using \n\t _NativeFrameworkSQLiteApplication.initalizeConnection(_NativeFramworkSQLiteConfig)\nbefore using any function in this class.";
		throw _SQLiteException(errMsg);
	}
}
_NativeFrameworkSQLiteStatement _NativeFrameworkSQLiteApplication::createStatement(char *query){
	if(initialized)
		return _attemptCreateStatement(query);
	else{
		string errMsg = "Database connection not initialized. Try using \n\t _NativeFrameworkSQLiteApplication.initalizeConnection(_NativeFramworkSQLiteConfig)\nbefore using any function in this class.";
		throw _SQLiteException(errMsg);
	}
}
_NativeFrameworkSQLitePreparedStatement _NativeFrameworkSQLiteApplication::prepareStatement(char *query){
	if(initialized)
		return _attemptPrepareStatement(query);
	else{
		string errMsg = "Database connection not initialized. Try using \n\t _NativeFrameworkSQLiteApplication.initalizeConnection(_NativeFramworkSQLiteConfig)\nbefore using any function in this class.";
		throw _SQLiteException(errMsg);
	}
}
_NativeFrameworkSQLitePreparedStatement _NativeFrameworkSQLiteApplication::prepareStatement(string query){
	if(initialized)
		return _attemptPrepareStatement(query.c_str());
	else{
		string errMsg = "Database connection not initialized. Try using \n\t _NativeFrameworkSQLiteApplication.initalizeConnection(_NativeFramworkSQLiteConfig)\nbefore using any function in this class.";
		throw _SQLiteException(errMsg);
	}
}

bool _NativeFrameworkSQLiteApplication::isOpen(){
	return initialized;
}

#ifdef _WIN32
	string fileSep = "\\";
#else
	string fileSep = "//";
#endif
//	v1.1
void _NativeFrameworkSQLiteApplication::openDatabase(char *databaseToOpen){
	string dbToOpen(databaseToOpen);
	string finalPath = databasePath + fileSep + dbToOpen + ".db";
	if(_databaseExists(finalPath)){
		// close exsiting connection
		closeConnection();
		_NativeFrameworkSQLiteConfig config(databasePath, databaseToOpen);
		initializeConnection(config);
	}else
		throw _SQLiteException(finalPath + " database does not exists.");
}

bool _NativeFrameworkSQLiteApplication::exists(char *checkDB){
	string finalPath = databasePath + fileSep + checkDB + ".db";
	return _databaseExists(finalPath);
}

bool _NativeFrameworkSQLiteApplication::databaseExists(){
	string finalPath = initialDatabase.length() > 0 ? databasePath + "\\" + initialDatabase + string(".db") : databasePath;
	return _databaseExists(finalPath);
}
void _NativeFrameworkSQLiteApplication::createDatabase(char *createDatabaseName){
	string finalPath = initialDatabase.length() > 0 ? databasePath + "\\" + createDatabaseName + string(".db") : databasePath;
	if(_databaseExists(finalPath)){
		string error = "Database " + initialDatabase + " already exists.";
		throw _SQLiteException(error);
	}else{
		//create database here
		_attemptCreateDatabase(createDatabaseName);
	}
}
_NativeFrameworkSQLiteConfig _NativeFrameworkSQLiteApplication::getConfig(){
	return config;
}

/*
*	SQLite connection implementation
*/

_NativeFrameworkSQLiteApplication _NativeFrameworkSQLiteConnection::getConnection(
	char *username, char *sendPassword){
	string baseDirectory = getenv("HOMEDRIVE");
	string homeDirectory = getenv("HOMEPATH");

	string homepath = baseDirectory + fileSep + homeDirectory;

	string databasePath = homepath + fileSep + ".linq-db";
	string initDatabase = "config";

	// define configuration
	_NativeFrameworkSQLiteConfig configuration;
		configuration.setDatabasePath(databasePath);
		configuration.setInitialDatabase(initDatabase);
	//connect now
	_NativeFrameworkSQLiteApplication database;
		database.initializeConnection(configuration);

	if(database.isOpen()){
		// fetch the contents of credentials table
		_NativeFrameworkSQLitePreparedStatement preparedStatement = database.prepareStatement("SELECT *FROM credentials WHERE uname = :username");
			preparedStatement.bindValue("username", username);
		try{
			_NativeFrameworkSQLiteResult result = preparedStatement.executeQuery();
			if(result.hasNextRow()){
			string password = result.getString(1);	// 1 is the second column(0 being first)
			if (password == string(sendPassword)){
				preparedStatement.closeStatement();
				database.closeConnection();
				_NativeFrameworkSQLiteApplication dbApp;
				dbApp.initializeConnection(_NativeFrameworkSQLiteConfig(databasePath, initDatabase));
				return dbApp;
			}
		}else
			throw _SQLiteException("Connection Credential not authenticated.");
		}catch(_NativeFrameworkSQLiteException exception){
			throw _SQLiteException("Connection credentials not authenticated");
		}
	}else
		throw _SQLiteException("Database instance not configured.");

}


#endif