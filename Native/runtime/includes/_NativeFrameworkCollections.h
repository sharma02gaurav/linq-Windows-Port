#pragma once

#include <iostream>
#include <string>
#include <stdlib.h>
#include <functional>

#ifndef EXCEPTION_DEFS
	#define EXCEPTION_DEFS
	
	#define __ERRFILE __FILE__
	#define __ERRFUNC __func__
	#define __ERRLINE __LINE__
#endif

using namespace std;

#define _CollectionsException(Error_Message) _NativeFrameworkCollectionsException(Error_Message, __ERRFILE, __ERRFUNC, __ERRLINE)

/*
*	The exception class
*/
class _NativeFrameworkCollectionsException{
	private:
		string errorMessage;
		string filename;
		string function;
		int line;

		void _printError(){
			cerr << "ERROR: " ;
			cerr << errorMessage << " \nin " << filename << " on line " << line << " in function " << function << endl;
			cerr << "Aborting" << endl;
		}
	public:
		_NativeFrameworkCollectionsException(string, string, string, int);
		string toString();
};

_NativeFrameworkCollectionsException::_NativeFrameworkCollectionsException(string errorMessage,
	string filename,
	string function,
	int line){

	this -> errorMessage = errorMessage;
	this -> filename = filename;
	this -> function = function;
	this -> line = line;

	_printError();
}

string _NativeFrameworkCollectionsException::toString(){
	return errorMessage;
}

/*	forward declaration of the iterator class
*/
template<typename T>
class _NativeFrameworkCollectionsBasicIterator;

/*
*	THE COLLECTIONS CLASS
*/
template<typename T>
class _NativeFrameworkCollections{
	public:
		virtual _NativeFrameworkCollectionsBasicIterator<T> getIterator() = 0;
};

/*
*	THE COLLECTIONS LINKED LIST CLASS
*/
template<typename T>
class _NativeFrameworkCollectionsLinkedList : public _NativeFrameworkCollections<T>{
	private:
		
		int itemsCount;
		bool enableCopyItems;
		/*
		*	the generic container structure
		*/
		typedef struct ContainerStruct_{
			int indexer;
			T item;
			ContainerStruct_ *nextTStruct;

			ContainerStruct_(){}
		};

		// required data structure for the service
		ContainerStruct_ *firstTStruct = nullptr;
		ContainerStruct_ *currentTStruct;
		ContainerStruct_ *lastTStruct;

		/*
		*	this method is a private method to add the value to the generic list
		*/

		void _attemptAdd(T value){
			
			if(firstTStruct == nullptr){
				
				firstTStruct = new ContainerStruct_();

				firstTStruct -> indexer = itemsCount++;
				firstTStruct -> item = value;
				firstTStruct -> nextTStruct = nullptr;

				
				lastTStruct = firstTStruct;
			}else{
				
				currentTStruct = new ContainerStruct_();

				currentTStruct -> indexer = itemsCount++;
				currentTStruct -> item = value;
				currentTStruct -> nextTStruct = nullptr;

				lastTStruct -> nextTStruct = currentTStruct;
				lastTStruct = currentTStruct;

			}
		}

		/*
		*	native private code to update the value in the collection. This works with
		*	the public method addItem(int, T), where int is the valid index of the element
		*	and T is the value to update the index to. Both int and T are validated by the
		*	public code and then are send to this function and this function solenely performs
		*	the update. The code arrives this function only if the int index and T values are
		*	validated but still if any error occurs, this function thow a 
		*	_NativeFrameworkCollections_exception exception.
		*/

		void _attemptUpdate(int index, T value, ContainerStruct_ *tempContainer){
			
			bool edited = false;
			
			while(tempContainer != nullptr){
				
				if(tempContainer -> indexer == index){
					tempContainer -> item = value;
					edited = true;
					goto eval_result;
				}
				
				tempContainer = tempContainer -> nextTStruct;
			}
			eval_result:
				if(! edited) throw _CollectionsException("Cannot update collection item");
		}

		/*
		*	checks if the T value is already present in the collection, if does return true
		*/

		bool _attemptCheckExists(T value, ContainerStruct_ *tempContainer){
			
			while(tempContainer != nullptr){
				
				if(tempContainer -> item == value)
					return true;
				
				tempContainer = tempContainer -> nextTStruct;
			}
			
			return false;
		}
		//find the index of specified item
		#define NOT_FOUND -1
		int _attemptFindIndex(T value, ContainerStruct_ *tempContainer){

			while(tempContainer != nullptr){
				if(tempContainer -> item == value)
					return tempContainer -> indexer;
				tempContainer = tempContainer -> nextTStruct;
			}
			return NOT_FOUND;
		}

		/*
		*	returns the value as reference represented by the index
		*/

		T _attemptGet(int index, ContainerStruct_ *tempContainer){
			while(tempContainer != nullptr){
				if(tempContainer -> indexer == index){
					return tempContainer -> item;
				}
				tempContainer = tempContainer -> nextTStruct;
			}
			cerr << "Error for index: " << index << endl;
			throw _CollectionsException("Required index element not found");
		}

		void _attemptDeleteItem(int index, ContainerStruct_ *tempContainer){
			
			if(index == size()-1){
				/*this is the last entry in the collection*/
				/*delete last entry and make the second last one the last entry*/
				while(tempContainer -> nextTStruct -> nextTStruct != nullptr)
					tempContainer = tempContainer -> nextTStruct;

				tempContainer -> nextTStruct = nullptr;

				lastTStruct = tempContainer;

				itemsCount -= 1;
				return;
			}

			/*find the entry in the collection*/
			/*if found..delete and back insert the preceeding collection values*/
			while(tempContainer -> nextTStruct != nullptr){
				if(tempContainer -> indexer == index){
					// this is the item to delete and modify preceeding
					// update the next values

					itemsCount -= 1;

					do{
						tempContainer -> item = tempContainer ->nextTStruct -> item;
						if(tempContainer -> nextTStruct -> nextTStruct == nullptr){

							delete tempContainer -> nextTStruct;

							tempContainer -> nextTStruct = nullptr;

							lastTStruct = tempContainer;

							break;
						}
						
						tempContainer = tempContainer -> nextTStruct;

						lastTStruct = tempContainer; 
					}while(tempContainer -> nextTStruct != nullptr);

					break;
				}
				tempContainer = tempContainer -> nextTStruct;
			}
		}

		void _attemptClearList(ContainerStruct_ *tempContainer){
			while(tempContainer != nullptr){
				ContainerStruct_ *next = tempContainer -> nextTStruct;
				delete tempContainer;
				tempContainer = next;
			}
			firstTStruct  = nullptr;

			itemsCount = 0;
		}

	public:
		// default constructor
		_NativeFrameworkCollectionsLinkedList();

		/*
		*	manipulation methods
		*/
		void addItem(T);
		/*
		*	add the item T at the specified index. The index should be >= 0
		*	or <= itemsCount.
		*	if index specified is = itemsCounter then the instertion is same as 
		*	addItem(int) otherwise the insertion is an update of existing value.
		*/
		void addItem(int, T);
		/*
		*	get the item at the requested index
		*/
		T getItem(int);
		/*
		*	returns boolean value indicating whether this list contains the T value or not
		*/
		bool containsItem(T);
		/*
		*	find the index of item T in this list. if there are multiple T's then the indiex
		*	of first T is returned.
		*/
		int indexOf(T);
		/*
		*	delete item at provided index
		*/
		void deleteItem(int);
		/*
		*	method to empty the linked list
		*/
		void clearList();

		/*
		*	utility methods
		*/
		// methods to set or not the copy items within the list (true by default)
		void setCopyItems(bool);
		// returns boolean value indicting whether the copy items enabled or not
		bool copyEnabled();
		// abastract function inherited from _NativeFrameworkCollections
		_NativeFrameworkCollectionsBasicIterator<T> getIterator();
		/*count the number of items in the list*/
		int size();
};

template<typename T> 
_NativeFrameworkCollectionsLinkedList<T>::_NativeFrameworkCollectionsLinkedList(){
	this -> itemsCount = 0;
	this -> enableCopyItems = true;
}


template<typename T> 
void _NativeFrameworkCollectionsLinkedList<T>::addItem(T item){
	
	//if(! copyEnabled())
		
		//if(_attemptCheckExists(item, firstTStruct)) 
			
			//throw _CollectionsException("Cannot add copy same items in same list. Try to change value or enable copy items.");
		
	_attemptAdd(item);
}


template<typename T> 
void _NativeFrameworkCollectionsLinkedList<T>::addItem(int index,
	T item){
	//if(! copyEnabled())

		//if(_attemptCheckExists(item, firstTStruct))
			
			//throw _CollectionsException("Cannot add copy same items in same list. Try to change value or enable copy items.");

	// if the index provided is the location where the new item is supposed to add
	if(index == itemsCount){
		try{
			
			_attemptAdd(item);
		
		}catch(_NativeFrameworkCollectionsException error){
			throw _CollectionsException(string("Error add item: ")+ string(error.toString()));
		}
	}// if the index provided is >= 0 and < itemsCount
	else if(index >= 0 && index < itemsCount){
		
		try{
			_attemptUpdate(index, item, firstTStruct);

		}catch(_NativeFrameworkCollectionsException error){
			
			throw _CollectionsException(string("Update Error: ")+ string(error.toString()));
		}
	}

	else throw _CollectionsException("Collection index out of bounds. Cannot add. Cannot add the items at null locations either the location being referred as index should be already initialized or is the next to be initialized.");

}


template<typename T> 
T _NativeFrameworkCollectionsLinkedList<T>::getItem(int index){
	
	if(index < 0 || index >= itemsCount)
		
		throw _CollectionsException("Out of bounds index provided.");
	
	/*get the value by iterating over the collction*/
	T temp = _attemptGet(index, firstTStruct);
	return temp;
	
	throw _CollectionsException("Item not found in the collection");
}


template<typename T> 
bool _NativeFrameworkCollectionsLinkedList<T>::containsItem(T item){
	return _attemptCheckExists(item, firstTStruct);
}

template<typename T> 
int _NativeFrameworkCollectionsLinkedList<T>::indexOf(T item){
	return _attemptFindIndex(item, firstTStruct);
}

template<typename T> 
void _NativeFrameworkCollectionsLinkedList<T>::deleteItem(int index){
	if(index >= itemsCount || index < 0)
		throw _CollectionsException("Collection index out of bounds.");
	_attemptDeleteItem(index, firstTStruct);
}

template<typename T> 
void _NativeFrameworkCollectionsLinkedList<T>::clearList(){
	_attemptClearList(firstTStruct);
}

/*
*	return _NativeFrameworkCollectionsIterator<T>. Inherited from _NativeFrameworkCollections
*/
template<typename T> 
_NativeFrameworkCollectionsBasicIterator<T>  _NativeFrameworkCollectionsLinkedList<T>::getIterator(){
	_NativeFrameworkCollectionsBasicIterator<T> sample = *this;
	return sample;
}

template<typename T> 
void _NativeFrameworkCollectionsLinkedList<T>::setCopyItems(bool enableCopyItems){
	this -> enableCopyItems = enableCopyItems;
}


template<typename T> 
bool _NativeFrameworkCollectionsLinkedList<T>::copyEnabled(){
	return enableCopyItems;
}


template<typename T> 
int _NativeFrameworkCollectionsLinkedList<T>::size(){
	return itemsCount;
}


/*
*	The Collections MAPLIST class
*/
template<typename T, typename K>
class _NativeFrameworkCollectionsMapList : public _NativeFrameworkCollections<T>{
	private:
		
		// the key pair
		_NativeFrameworkCollectionsLinkedList<T> keyPair;

		// the value pair
		_NativeFrameworkCollectionsLinkedList<K> valuePair;

		//enabling this allows duplicate keys
		bool duplicateKeys;

		/*
		*	add a pair to map list
		*/
		void _attemptPutItem(T key, K value){
			if(keyPair.containsItem(key) && ! duplicateKeys)
				throw _CollectionsException("Cannot insert duplicate keys!. Or you can enable duplicate keys by using set_duplicates(bool).");
			
			try{

				// add key to key pair
				keyPair.addItem(key);

				// add value to value pair
				valuePair.addItem(value);

			}catch(_NativeFrameworkCollectionsException error){
				throw _CollectionsException("Error put item.");
			}
		}

		// fetch and returns the K* item from the hash map
		// returns nullptr if item is not found

		K _attemptGetItem(T key){
			// we need to analyze the key_list

			int itemIndex = keyPair.indexOf(key);
			if(itemIndex != NOT_FOUND)
				return valuePair.getItem(itemIndex);
			
			return NULL;
		}

		/*
		*	try to delete item with the T key
		*/

		void _attemptDeleteItem(T key){
			if(keyPair.containsItem(key)){
				int keyIndex = keyPair.indexOf(key);

				try{
					keyPair.deleteItem(keyIndex);
					valuePair.deleteItem(keyIndex);
				}catch(_NativeFrameworkCollectionsException error){
					throw _CollectionsException("Error deleting item.");
				}
			}else throw _CollectionsException("Invalid key provided to delete a maplist item.");
		}

		/*
		*	clears both of the maps.
		*/
		void _attemptClearingMap(){
			try{
				keyPair.clearList();
				valuePair.clearList();
				
			}catch(_NativeFrameworkCollectionsException error){
				throw _CollectionsException(error.toString());
			}
		}

		// check
		bool _keyExists(T key){
			return keyPair.containsItem(key) ? true : false;
		}

		/*
		*	get the keys of the collection as _NativeFrameworkCollectionsLinkedList
		*/

		_NativeFrameworkCollectionsLinkedList<T> _getKeysList(){
			return keyPair;
		}

		/*
		*	get the values of the collection as _NativeFrameworkCollectionsLinkedList
		*/
		_NativeFrameworkCollectionsLinkedList<K> _getValueList(){
			return valuePair;
		}

	public:
		/*the basic constructor for this class*/
		// set duplicate keys to false by default
		_NativeFrameworkCollectionsMapList();

		/*operation methods*/
		/*
		*	Put an item in this map
		*	@param T represents the key value for entry to add.
		*	@param K represents the value for entry to add.
		*/
		void putItem(T, K);
		/*
		*	Get an item from this map
		*	@Returns K value which is the value against the value requested
		*	@Param T represents the Key of map in the list whose value is requested.
		*/
		K getItem(T);
		/*
		*	Delete a map from the list represented by the key provided
		*	@Param T represents the key of the map to delete. The delete operation leads to
		*	up shift the values residing below the maplist to fill the emptied block.
		*/
		void deleteItem(T);
		/*
		*	Clears every item in the map. No undo option.
		*/
		void clearMap();

		// asbtaract method from _NativeFrameworkCollections
		// for map, this returns the iterator for keys
		_NativeFrameworkCollectionsBasicIterator<T> getIterator();

		// utility methods

		int size();
};

/*
*	_NativeFrameworkCollectionsMapList's members impl_
*/

template<typename T, typename K> _NativeFrameworkCollectionsMapList<T, K>::_NativeFrameworkCollectionsMapList(){
	this -> duplicateKeys = false;
}
template<typename T, typename K> void _NativeFrameworkCollectionsMapList<T, K>::putItem(T key, K item){
	try{
		_attemptPutItem(key, item);
	}catch(_NativeFrameworkCollectionsException error){
		throw _CollectionsException(string("Cannot put entry: ")+ string(error.toString()));
	}
}
template<typename T, typename K> K _NativeFrameworkCollectionsMapList<T, K>::getItem(T key){
	if(_keyExists(key))
		return _attemptGetItem(key);
	throw _CollectionsException("Requested key does not exist");
}
template<typename T, typename K> void _NativeFrameworkCollectionsMapList<T, K>::deleteItem(T key){
	_attemptDeleteItem(key);
}
template<typename T, typename K> void _NativeFrameworkCollectionsMapList<T, K>::clearMap(){
	try{
		_attemptClearingMap();
	}catch(_NativeFrameworkCollectionsException error){
		throw _CollectionsException(string("Error clearing map list: ")+ string(error.toString()));
	}
}

template<typename T, typename K> _NativeFrameworkCollectionsBasicIterator<T> _NativeFrameworkCollectionsMapList<T, K>::getIterator(){
	return keyPair.getIterator();
}

template<typename T, typename K> int _NativeFrameworkCollectionsMapList<T, K>::size(){
	return keyPair.size() == valuePair.size() ? keyPair.size() : throw _CollectionsException("API error occured. key pair and value pair does not contain equal values.");
}


/*
*	The COLLECTIONS ITERATOR CLASS
*/

template<typename T>
class _NativeFrameworkCollectionsIterator{
private:
		/*
		*	holds the current item in the iterator
		*/
		T currentItem;
		/*
		*	holds the current index position on the iterator
		*/
		int currentIndex;
		/*
		*	the last indexing position in the collection being iterated
		*/
		int finalIndex;
		/*
		*	_NativeFrameworkCollectionsLinkedList is an alias for _NativeFrameworkCollections_genericlinkedlist.
		*	this is initialized with the constructor
		*/
		_NativeFrameworkCollectionsLinkedList<T> collectionItem;
		_NativeFrameworkCollectionsLinkedList<T> copyCollection;
	public:

		/*
		*	_NativeFrameworkCollectionsBasicIterator is template alias for _NativeFrameworkCollectionsBasicIterator
		*/

		_NativeFrameworkCollectionsIterator(_NativeFrameworkCollectionsBasicIterator<T>);
		// property methods
		bool hasNext();
		T getCurrent();
		int getIndex();
		int getRemaining();
		void reset();
};

/*iterator items impl*/
template<typename T>
_NativeFrameworkCollectionsIterator<T>::_NativeFrameworkCollectionsIterator(_NativeFrameworkCollectionsBasicIterator<T> it){
	this -> collectionItem = it.getIterableItem();	// the basic iterator
	this -> copyCollection = collectionItem;
	this -> currentIndex = 0;
	this -> finalIndex = collectionItem.size() - 1;
}

template<typename T>
bool _NativeFrameworkCollectionsIterator<T>::hasNext(){
	if(currentIndex <= finalIndex){
		
		currentItem = collectionItem.getItem(currentIndex);
		currentIndex += 1;
		
		return true;
	}
	return false;
}
template<typename T>
T _NativeFrameworkCollectionsIterator<T>::getCurrent(){
	if(currentIndex <= finalIndex+1)
		return currentItem;
	throw _CollectionsException("Iterator already has iterated among all items. Try Reset");
}
template<typename T>
int _NativeFrameworkCollectionsIterator<T>::getIndex(){
	return currentIndex;
}
template<typename T>
int _NativeFrameworkCollectionsIterator<T>::getRemaining(){
	return collectionItem.size() - (currentIndex);
}
template<typename T>
void _NativeFrameworkCollectionsIterator<T>::reset(){
	currentIndex = 0;
}

/*
*	LINQ_NATIVE_COLLECTIONS_BASIC_ITERATOR's impl_
*	This class is different from iterator in the way that it does not iterate the collection,
*	instead passed the and collection's instance as argument converts any collection into 
*	_NativeFrameworkCollectionsLinkedList<T> and then it's reference could be passed to the iterator's constructor
*	which technically uses the _NativeFrameworkCollectionsLinkedList<T> to iterate the collection.
*	ANY COLLECTION IN ORDER TO ITERATE, NEEDS TO BE CONVERTED TO _NativeFrameworkCollectionsLinkedList<T> (if not already).
*	All the collection's instances must contain a method called getIterator() that returns the 
*	instance of iterator.
*	so one can get the instance of iterator() in either way:
*	1.	using _***BasicIterator class.
*	2.	using getIterator() method of collection classes.
*/

template<typename T>
class _NativeFrameworkCollectionsBasicIterator{
	private:
		_NativeFrameworkCollectionsLinkedList<T> collectionItem;
	public:
		_NativeFrameworkCollectionsBasicIterator(_NativeFrameworkCollectionsLinkedList<T>);
		// get the iterator item
		_NativeFrameworkCollectionsLinkedList<T> getIterableItem();
};

/*
*	_NativeFrameworkCollectionsBasicIterator impl_
*/
template<typename T> 
_NativeFrameworkCollectionsBasicIterator<T>::_NativeFrameworkCollectionsBasicIterator(_NativeFrameworkCollectionsLinkedList<T> collectionItem){
	this -> collectionItem = collectionItem;
}

template<typename T>
_NativeFrameworkCollectionsLinkedList<T> _NativeFrameworkCollectionsBasicIterator<T>::getIterableItem(){
	return this -> collectionItem;
}

template<typename T>
class _NativeFrameworkCollectionsFunctionalIterator{
	private:
		_NativeFrameworkCollectionsLinkedList<T> collection;
		function<void(T)> iteratorFunction;
	public:
		_NativeFrameworkCollectionsFunctionalIterator();
		_NativeFrameworkCollectionsFunctionalIterator(_NativeFrameworkCollectionsLinkedList<T>);
		/*
		*	here function is the pointer to the function of the form void(T) where void is the
		*	return type and T is the parameter type i.e the type of value the collection holds.
		*	This constructor simply provides the function to execute for each - T in the collection
		*	where the developer decides what to do with the T value and declare it in the method body.
		*	example:
		*		// let the collection is initialized something like this
		*		collection.addItem(10);
		*		collection.addItem(20);
		*		collection.addItem(30);
		*		// if developer wants to print the values of this collection, then do this,
		*		void printCollectionValues(int value){
		*			Console::printLine(value);
		*		}
		*		FunctionalIterator iterator(collection, printCollection);
		*			iterator.start();	// this will start iteration
		}
		*/
		_NativeFrameworkCollectionsFunctionalIterator(_NativeFrameworkCollectionsLinkedList<T>, function<void(T)>);

		/*
		*	the setter functions
		*/
		void setCollection(_NativeFrameworkCollectionsLinkedList<T>);
		void setIteratorFunction(function<void(T)>);
		void iterate();
};
template<typename T>
_NativeFrameworkCollectionsFunctionalIterator<T>::_NativeFrameworkCollectionsFunctionalIterator(){

}
template<typename T>
_NativeFrameworkCollectionsFunctionalIterator<T>::_NativeFrameworkCollectionsFunctionalIterator(_NativeFrameworkCollectionsLinkedList<T> colection){
	this -> collection = collection;
}
template<typename T>
_NativeFrameworkCollectionsFunctionalIterator<T>::_NativeFrameworkCollectionsFunctionalIterator(
	_NativeFrameworkCollectionsLinkedList<T> collection,
	function<void(T)> iteratorFunction){
	this -> collection = collection;
	this -> iteratorFunction = iteratorFunction;
}
template<typename T>
void _NativeFrameworkCollectionsFunctionalIterator<T>::setCollection(
	_NativeFrameworkCollectionsLinkedList<T> collection){
	this -> collection = collection;
}
template<typename T>
void _NativeFrameworkCollectionsFunctionalIterator<T>::setIteratorFunction(
	function<void(T)> iteratorFunction){
	this -> iteratorFunction = iteratorFunction;
}
template<typename T>
void _NativeFrameworkCollectionsFunctionalIterator<T>::iterate(){
	for(int i=0; i< collection.size() ; i+=1){
		iteratorFunction(collection.getItem(i));
	}
}

/*
*	Generic Array class
*/

template<typename T>
class _NativeFrameworkCollectionsArray{
	private:
		long occupied;
		long maxSize;
		T *arrayInstance;
	public:
		_NativeFrameworkCollectionsArray(long);
		_NativeFrameworkCollectionsArray(int);

		void addItem(int, T);
		void addItem(long, T);
		T getItem(int);
		T getItem(long);
		void clear();
		long size();
		long occupiedCount();
		_NativeFrameworkCollectionsBasicIterator<T> getIterator();

		~_NativeFrameworkCollectionsArray(){
			if(arrayInstance){
				delete [] arrayInstance;
				arrayInstance = nullptr;
			}
		}
};

template<typename T>
_NativeFrameworkCollectionsArray<T>::_NativeFrameworkCollectionsArray(int size){
	this -> maxSize = size;
	occupied = 0;
	arrayInstance = new T[maxSize];
}

template<typename T>
_NativeFrameworkCollectionsArray<T>::_NativeFrameworkCollectionsArray(long size){
	this -> maxSize = size;
	occupied = 0;
	arrayInstance = new T[maxSize];
}

template<typename T>
void _NativeFrameworkCollectionsArray<T>::addItem(int index, T item){
	if(index < 0 || index > maxSize)
		throw _CollectionsException("Array Index out of bounds exception.");
	else{
		arrayInstance[index] = item;
		occupied += 1;
	}
}

template<typename T>
void _NativeFrameworkCollectionsArray<T>::addItem(long index, T item){
	if(index < 0 || index > maxSize)
		throw _CollectionsException("Array Index out of bounds exception.");
	else{
		arrayInstance[index] = item;	
		occupied += 1;
	}
}

template<typename T>
T _NativeFrameworkCollectionsArray<T>::getItem(int index){
	if(index < 0 || index > maxSize)
		throw _CollectionsException("Array Index out of bounds exception.");
	else
		return arrayInstance[index];
}

template<typename T>
T _NativeFrameworkCollectionsArray<T>::getItem(long index){
	if(index < 0 || index > maxSize)
		throw _CollectionsException("Array Index out of bounds exception.");
	else
		return arrayInstance[index];	
}

template<typename T>
void _NativeFrameworkCollectionsArray<T>::clear(){
	delete [] arrayInstance;
	arrayInstance = nullptr;
	occupied = 0;
	maxSize = 0;
}

template<typename T>
long _NativeFrameworkCollectionsArray<T>::size(){
	return maxSize;
}

template<typename T>
long _NativeFrameworkCollectionsArray<T>::occupiedCount(){
	return occupied;
}

template<typename T>
_NativeFrameworkCollectionsBasicIterator<T> _NativeFrameworkCollectionsArray<T>::getIterator(){
	_NativeFrameworkCollectionsLinkedList<T> arrayItems;
	for(int i=0; i< maxSize;i++)
		arrayItems.addItem(getItem(i));

	_NativeFrameworkCollectionsBasicIterator<T> basicIterator = arrayItems;
	return basicIterator;
}