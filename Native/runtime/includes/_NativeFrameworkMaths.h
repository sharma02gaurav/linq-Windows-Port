#pragma once

#include <iostream>

using namespace std;

class _NativeFrameworkMaths{
	private:
		/*
		*	This class uses the Bisection (or Bolzano method) for root-finding. It is
		*	based on repeated application of intermediate value theorem.
		*/
		static double _squareRootApproximation(double);
		static double _evaluateSquareExpression(double ,double);
		static double _squareOf(double);
	public:
		/*
		*	square root finding
		*/
		static double squareRootOf(double);
		static double squareRootOf(int);
		static double squareRootOf(long);
		/*
		*	square finding
		*/
		static long squareOf(long);
		static double squareOf(double);
		static long squareOf(int);
};