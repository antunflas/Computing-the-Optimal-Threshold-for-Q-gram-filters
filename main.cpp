#include <iostream>
#include <limits>
#include <cmath>
#include <stdlib.h>
#include <algorithm>
#include <string>

using namespace std;

static const int MAX_INT = std::numeric_limits<int>::max();

/*  poèetna funkcija koju zovemo iz maina */
int calculateThreshold(int s, int k, int m, int q, int* result);

/*  generira sve Q koje treba ispitati
 kasnije bi možda bilo pametno ovo preoblikovati tako da izraèuna koji su pozitivni thresholdi pa vrati samo te Q.
 */
int** generateShapes(int s, int q, int* shapesLen);

/*  raèuna threshold za odreðeni Q */
int calculateThresholdForShape(int s, int k, int m, int* Q, int q);

/*  ona rekurzivna funkcija */
int findThreshold(int s, int k, int* Q, int lenQ, int* M, int lenM, int i,
		int j);

/*	*/
int toBinary(long long int value, bool* array, int size);

/*	*/
int fillFromBinary(bool* binary, int start, int end, int* arrayM, int offset);

/*
 Tests
 */
int testThresholdOneShape1();
int testThresholdOneShape2();
void testThresholdForAllShapesWithSomeQAndKVariableS(int q, int k);

int main() {

	std::cout << "Hello World!" << std::endl;

	std::string s;
	std::getline(std::cin, s);
	return 0;
}

int calculateThreshold(int s, int k, int m, int q, int* result) {
	int shapesLen = 0;
	int** shapes = generateShapes(s, q, &shapesLen);
	int threshold = 0;
	for (int i = 0; i < shapesLen; i++) {
		int* shape = shapes[i];
		int value = calculateThresholdForShape(s, k, m, shape, q);
		if (threshold < value) {
			threshold = value;
			result = shape;
		}
	}
	return threshold;
}

int calculateThresholdForShape(int s, int k, int m, int* arrayQ, int arrayQLen) {
	int* arrayM = new int[s - 1];
	int arrayMLen = 0;

	long long int limit = pow(2, s - 1);
	bool* binary = new bool[s - 1];
	int result = MAX_INT;
	for (long long int counter = 0; counter < limit; counter++) {
		toBinary(counter, binary, s - 1);
		//fillFromBinary(binary, 1, s - 1, &M);
		//cout << "M = ";
		arrayMLen = fillFromBinary(binary, 1, s - 1, arrayM, 0);
		//cout << endl;
		//result = min(findThreshold(s, k, Q, &M, m, k), result);
		result = min(
			findThreshold(s, k, arrayQ, arrayQLen, arrayM, arrayMLen, m, k),
			result);
	}
	return result;
}


int findThreshold(int s, int k, int* Q, int lenQ, int* M, int lenM, int i,
		int j) {

	/*	check constraints */

	if (!(j >= 0 && j <= k)) {
		return MAX_INT;
	}
	for (int c = 0; c < lenM; c++) {
		if (!(M[c] >= 1 && M[c] <= (s - 1))) {
			return MAX_INT;
		}
	}

	if (lenM < (s - 1 - j)) {
		return MAX_INT;
	}
	if (i < s) {
		return 0;
	}

	/*	generate next Ms */
	int* nextM1 = new int[lenM + 1];
	int* nextM2 = new int[lenM];
	int lenNextM1 = 1;
	int lenNextM2 = 0;
	nextM1[0] = 1;
	for (int c = 0; c < lenM; c++) {
		if (M[c] == (s - 1))
			continue;
		nextM1[c + 1] = M[c] + 1;
		lenNextM1++;
		nextM2[c] = M[c] + 1;
		lenNextM2++;
	}

	/*	determine if Q is subset of Mu{0} */
	bool isSubset = true;
	for (int c = 0; c < lenQ; c++) {
		if (Q[c] == 0)
			continue;
		bool found = false;
		for (int d = 0; d < lenM; d++) {
			if (Q[c] == M[d]) {
				found = true;
				break;
			}
		}
		if (!found) {
			isSubset = false;
			break;
		}
	}

	/*	calculate nextJ (j or j-1) */
	int nextJ = j;
	bool contains = false;
	for (int c = 0; c < lenM; c++) {
		if (M[c] == (s - 1)) {
			contains = true;
			break;
		}
	}
	if (!contains)
		nextJ--;

	int result = min(
			findThreshold(s, k, Q, lenQ, nextM1, lenNextM1, i - 1, nextJ)
					+ (isSubset ? 1 : 0),
			findThreshold(s, k, Q, lenQ, nextM2, lenNextM2, i - 1, nextJ));
	free(nextM1);
	free(nextM2);
	return result;
}

/*
 Generate list of shapes...
 */
int** generateShapes(int s, int q, int* shapesLen) {
	int start = 1;
	int end = s - 2;
	int len = end - start + 1;
	long long int countTo = pow(2, len);
	int** shapes = new int*[countTo];
	*shapesLen = 0;
	bool* array = new bool[len];
	for (long long int counter = 0; counter < countTo; counter++) {
		int onesCount = toBinary(counter, array, len);
		if (onesCount == (q - 2)) {
			int* arrayQ = new int[q];
			fillFromBinary(array, start, end, arrayQ, 1);
			arrayQ[0] = 0;
			arrayQ[q - 1] = s - 1;
			shapes[*shapesLen] = arrayQ;
			*shapesLen = *shapesLen + 1;
		}
	}
	return shapes;
}

/*
 Fill arrayM array with numbers from start to end where is one in binary.
 Like there is an array of [start, start + 1, ..., end - 1, end] and in arrayM
 are copied values on indexes where is one in binary array.
 start and end are both inclusive
 */
int fillFromBinary(bool* binary, int start, int end, int* arrayM, int offset) {
	int len = end - start + 1;
	int arrayMLen = offset;
	for (int c = 0; c < len; c++) {
		if (binary[c] == 1) {
			arrayM[arrayMLen++] = start + c;
		}
	}
	return arrayMLen;
}

/*
Converts long number to binary array.
Returns number of ones in array.
*/
int toBinary(long long int value, bool* array, int size) {
	int counter = 0;
	for (int i = 0; i < size; i++) {
		counter += array[size - i - 1] = value & (1 << i);
	}
	return counter;
}
