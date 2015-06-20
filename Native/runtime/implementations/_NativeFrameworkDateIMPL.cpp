#include "../includes/_NativeFrameworkDate.h"

void _NativeFrameworkDate::initTime(){
	now = time(0);
	local_time = localtime(&now);
	
	day = local_time -> tm_mday;
	month = (local_time -> tm_mon) +1; //a bug
	year = ((local_time -> tm_year) - 100)+ 2000;
	hours = local_time -> tm_hour;
	minutes = local_time -> tm_min;
	seconds = local_time -> tm_sec;
	
	str_month = stringMonthName(month);
}

char* _NativeFrameworkDate::stringMonthName(int month){
	switch(month){
		case 1: return "January";
		case 2: return "February";
		case 3: return "March";
		case 4: return "April";
		case 5: return "May";
		case 6: return "June";
		case 7: return "July";
		case 8: return "August";
		case 9: return "September";
		case 10: return "October";
		case 11: return "November";
		case 12: return "December";
		default: return "";
	}
}

_NativeFrameworkDate::_NativeFrameworkDate(){
	initTime();
}
char* _NativeFrameworkDate::getDate(){
	return (char*)ctime(&now);
}
long _NativeFrameworkDate::getDay(){ 
	return day; 
}
long _NativeFrameworkDate::getMonth(){ 
	return month;
}
long _NativeFrameworkDate::getYear(){ 
	return year;
}
long _NativeFrameworkDate::getHours(){ 
	return hours;
}
long _NativeFrameworkDate::getMinutes(){
	return minutes;
}
long _NativeFrameworkDate::getSeconds(){
	return seconds;
}
char* _NativeFrameworkDate::getMonthName(){
	return str_month;
}