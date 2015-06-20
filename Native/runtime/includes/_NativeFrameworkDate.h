#pragma once

#include <iostream>
#include <string>
#include <ctime>

using namespace std;

class _NativeFrameworkDate{
	private:
		long day, month, year, hours, minutes, seconds;
		char *str_month;
		time_t now;
		tm *local_time;
		/*
		* init the ctime structs
		*/
		void initTime();
		char* stringMonthName(int);
	public:
		_NativeFrameworkDate();
		//one time values
		char* getDate();
		long getDay();
		long getMonth();
		long getYear();
		long getHours();
		long getMinutes();
		long getSeconds();
		char* getMonthName();
};