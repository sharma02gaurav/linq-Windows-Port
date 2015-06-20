#include "../includes/_NativeFrameworkMaths.h"

double _NativeFrameworkMaths::_squareRootApproximation(double number){
	double precisionFactor = 0.001d;	// 3-digits precision

	// Finding the intermediate value and checking it's value i.e +ve or -ve
	double initialApproximation = (double)number/2;

	/* 	finding the most precise value. i.e. iterating the median
	*	value until the initialApproximation becomes -ve and the last +ve
	*	median value is taken as initialApproximation. This calculation
	*	removes the further claculations and gives precise results.
	*/

	while(_evaluateSquareExpression(initialApproximation, number) > 0)
		initialApproximation /= 2;

	/*
	*	finally calculate the actual root by using the precision factor and evaluate
	*	until the first approximation which turns the expression > 0
	*/
	while(_evaluateSquareExpression(initialApproximation, number) <= 0)
		initialApproximation += precisionFactor;

	initialApproximation -= precisionFactor;

	return initialApproximation;
}

double _NativeFrameworkMaths::_evaluateSquareExpression(double approximation, double rootNumber){
	return _squareOf(approximation) - rootNumber;
}

double _NativeFrameworkMaths::_squareOf(double number){
	return number*number;
}

double _NativeFrameworkMaths::squareRootOf(double number){
	return _squareRootApproximation(number);
}
double _NativeFrameworkMaths::squareRootOf(int number){
	return _squareRootApproximation((double)number);
}
double _NativeFrameworkMaths::squareRootOf(long number){
	return  _squareRootApproximation((double)number);
}
long _NativeFrameworkMaths::squareOf(int number){
	return (long)_squareOf((double)number);
}
long _NativeFrameworkMaths::squareOf(long number){
	return (long)_squareOf((double)number);
}
double _NativeFrameworkMaths::squareOf(double number){
	return _squareOf(number);
}
