#include <iostream>
#include <limits>
#include <cmath>

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
	return 0;
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
