#include <iostream>
#include <limits>
#include <cmath>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

static const int MAX_INT = std::numeric_limits<int>::max();
static vector<int*> empty_vector;

/*  po�etna funkcija koju zovemo iz maina */
int calculateThreshold(int s, int k, int m, int q, int* result);

/*  generira sve Q koje treba ispitati
 kasnije bi mo�da bilo pametno ovo preoblikovati tako da izra�una koji su pozitivni thresholdi pa vrati samo te Q.
 */
vector<int*> generateShapes(int s, int q);

/*  ra�una threshold za odre�eni Q */
int calculateThresholdForShape(int s, int k, int m, int* arrayQ, int arrayQLen);

int myCalculateThresholdForShape(int s, int k, int m, int* arrayQ,
		int arrayQLen, int** thresholds, long long int offset,
		long long int thresholdLen, int** copy);

/*  ona rekurzivna funkcija */
int findThreshold(int s, int k, int* Q, int lenQ, int* M, int lenM, int i,
		int j, int** tresholds, long long int lenTreshold,
		long long int offset, bool* binary);

int getTresholdFor(int s, int k, int offset, int j, int* M, int lenM,
		int** tresholds, int lenTresholds, bool* binary);

/*	*/
int toBinary(long long int value, bool* array, int size);

void toBinary(int* M, int lenM, bool* binary, int size);

/* */
long long int fromBinary(bool* array, int count);

/*	*/
int fillFromBinary(bool* binary, int start, int end, int* arrayM, int offset);

long long int binomialCoefficient(int m, int n);

long long int calculateThresholdArrayLength(int k, int s);

/*
 Tests
 */
int testThresholdOneShape1();
int testThresholdOneShape2();
void testThresholdForAllShapesWithSomeQAndKVariableS(int q, int k);

int main(int argc, char** argv) {

	/*  Argumenti: m, k, s, q */


	int m = 13;
	int k = 2;
	int s = 4;
	int q = 3;

	int thresholdsArrayLength = calculateThresholdArrayLength(k, s);
	cout << "array length = " << thresholdsArrayLength << endl;

	int* probaM = new int[3] { 2, 3, 5 };
	for (int i = 0; i < 3; i++) {
		cout << probaM[i];
	}
	cout << endl;

	bool* binaryM = new bool[3];
	toBinary(probaM, 3, binaryM, 5);

	for (int i = 0; i < 5; i++) {
		cout << binaryM[i];
	}
	cout << endl;

	long long int tresholdsArrayLength = 0;
	for (int j = 0; j <= k; j++) {
		tresholdsArrayLength += binomialCoefficient(s - 1, j);
	}
	cout << "array length = " << tresholdsArrayLength << endl;

	long long int offset = pow(2, s - 1) - tresholdsArrayLength;
	cout << "offset = " << offset << endl;

	if (argc >= 3) {
		k = atoi(argv[1]);
		q = atoi(argv[2]);
	}
	cout << "binomial(5,3) = " << binomialCoefficient(5, 3) << endl;

	k = atoi(argv[1]);
	q = atoi(argv[2]);

	cout << "Test case 1 - given result: " << testThresholdOneShape1()
			<< " expected result: 1" << endl;
	cout << "Test case 2 - given result: " << testThresholdOneShape2()
			<< " expected result: 2" << endl;

	cout << "k=" << k << " q=" << q << endl;


	testThresholdForAllShapesWithSomeQAndKVariableS(q, k);

	std::string stringManuela;
	std::getline(std::cin, stringManuela);

	return 0;
}

long long int calculateThresholdArrayLength(int k, int s) {
	long long int thresholdsArrayLength = 0;
	for (int j = 0; j <= k; j++) {
		thresholdsArrayLength += binomialCoefficient(s - 1, j);
	}
	return thresholdsArrayLength;
}

int calculateThreshold(int s, int k, int m, int q, int* result) {

	int shapesLen = 0;
	cout << "generating" << endl;
	vector<int*> shapes = generateShapes(s, q);
	cout << "SHAPES LEN: " << shapesLen << endl;
	int threshold = 0;
	long long int thresholdArrayLength = calculateThresholdArrayLength(k, s);
	cout << "tu" << endl;
	int** thresholds = new int*[thresholdArrayLength];
	cout << "created with " << thresholdArrayLength << endl;
	int** copy = new int*[thresholdArrayLength];
	bool* binary = new bool[s - 1];
	long long int offset = pow(2, s - 1) - thresholdArrayLength;
	for (long long int index = 0; index < thresholdArrayLength; index++) {
		int ones = toBinary(index + offset, binary, s - 1);
		int* arrayJ = new int[ones + 1];
		arrayJ[0] = ones;
		copy[index] = new int[ones + 1];
		copy[index][0] = ones;
		for (int j = 1, jLen = ones + 1; j < jLen; j++) {
			arrayJ[j] = 0;
			copy[index][j] = 0;
		}
		thresholds[index] = arrayJ;
	}

	for (unsigned int i = 0; i < shapes.size(); i++) {
		int* shape = shapes[i];
		int value = myCalculateThresholdForShape(s, k, m, shape, q, thresholds,
				offset, thresholdArrayLength, copy);
		if (threshold < value) {
			threshold = value;
			result = shape;
		}
	}
	cout << "Deleting" << endl;
	for (long long int index = 0; index < thresholdArrayLength; index++) {
		delete[] thresholds[index];
		delete[] copy[index];
	}
	cout << "Deleted 2d" << endl;
	delete[] thresholds;
	cout << "Deleted threshold" << endl;
	delete[] copy;
	cout << "Deleted copy" << endl;
	cout << "Deleted 2d shapes" << endl;
	shapes.clear();
	shapes.swap(empty_vector);
	cout << "Deleted shapes" << endl;
	delete[] binary;
	cout << "Deleted all" << endl;
	cout << "Free finish" << endl;
	return threshold;
}

int calculateThresholdForShape(int s, int k, int m, int* arrayQ,
		int arrayQLen) {

	//NE SLIJEPO VJEROVATI
	//za i = s do m:
	//		treba pro� po polju thresholds
	//		za svaku poziciju izra�unati M, j:
	//M = (index + offset) -> toBinary -> fillBinary
	//j = k - index
	//		treshold = pozvati findTreshold s tim M i j
	// 		zapisati taj treshold na tu pozciju

	//sada na kraju treba pro�i cijelo polje (ovu prvu dim) i sa indeksa 1 (
	//na prvom je duljina) u polju druge dim
	//pro�itati vrijednost praga i od svih njih treba uzeti min

	int* arrayM = new int[s - 1];
	int arrayMLen = 0;

	long long int limit = pow(2, s - 1);
	bool* binary = new bool[s - 1];
	int result = MAX_INT;
	for (long long int counter = 0; counter < limit; counter++) {
		toBinary(counter, binary, s - 1);
		arrayMLen = fillFromBinary(binary, 1, s - 1, arrayM, 0);
		/*
		 result = min(
		 findThreshold(s, k, arrayQ, arrayQLen, arrayM, arrayMLen, m, k),
		 result);
		 */
	}
	return result;
}

int myCalculateThresholdForShape(int s, int k, int m, int* arrayQ,
		int arrayQLen, int** thresholds, long long int offset,
		long long int thresholdLen, int** copy) {
	int* arrayM = new int[s - 1];
	int arrayMLen = 0;
	bool* binary = new bool[s - 1];

	for (int i = s; i <= m; i++) {
		for (long long int index = 0; index < thresholdLen; index++) {
			toBinary(index + offset, binary, s - 1);
			arrayMLen = fillFromBinary(binary, 1, s - 1, arrayM, 0);
			for (int j = 1, jLen = thresholds[index][0] + 1; j < jLen; j++) {
				copy[index][j] = findThreshold(s, k, arrayQ, arrayQLen, arrayM,
						arrayMLen, i, k - j + 1, thresholds, thresholdLen, offset, binary);
			}
		}
		for (long long int index = 0; index < thresholdLen; index++) {
			for (int j = 1, jLen = thresholds[index][0] + 1; j < jLen; j++) {
				thresholds[index][j] = copy[index][j];
			}
		}
	}
	int result = MAX_INT;
	for (long long int index = 0; index < thresholdLen; index++) {
		for (int j = 1, jLen = thresholds[index][0] + 1; j < jLen; j++) {
			if (result > thresholds[index][j]) {
				result = thresholds[index][j];
			}
		}
	}
	free(arrayM);
	delete[] binary;
	return result;

	/*
	 long long int limit = pow(2, s - 1);

	 int result = MAX_INT;
	 for (long long int counter = 0; counter < limit; counter++) {
	 toBinary(counter, binary, s - 1);
	 arrayMLen = fillFromBinary(binary, 1, s - 1, arrayM, 0);
	 result = min(
	 findThreshold(s, k, arrayQ, arrayQLen, arrayM, arrayMLen, m, k),
	 result);
	 }
	 return result;
	 */
}
/*	check constraints */
int findThreshold(int s, int k, int* Q, int lenQ, int* M, int lenM, int i,
		int j, int** tresholds, long long int lenTreshold,
		long long int offset, bool* binaryM) {

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

	return min(
			getTresholdFor(s, k, offset, nextJ, nextM1, lenNextM1, tresholds,
					lenTreshold, binaryM) + (isSubset ? 1 : 0),
			getTresholdFor(s, k, offset, nextJ, nextM2, lenNextM2, tresholds,
					lenTreshold, binaryM));
}

int getTresholdFor(int s, int k, int offset, int j, int* M, int lenM,
		int** tresholds, int lenTresholds, bool* mBinary) {

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

	toBinary(M, lenM, mBinary, s - 1);
	long long int indexM = fromBinary(mBinary, s - 1) - offset;
	int indexJ = k - j + 1;

	return tresholds[indexM][indexJ];
}

/*
 Generate list of shapes...
 */
vector<int*> generateShapes(int s, int q) {
	int start = 1;
	int end = s - 2;
	int len = end - start + 1;
	long long int countTo = pow(2, len);
	vector<int*> shapes;
	bool* array = new bool[len];
	for (long long int counter = 0; counter < countTo; counter++) {
		int onesCount = toBinary(counter, array, len);
		if (onesCount == (q - 2)) {
			int* arrayQ = new int[q];
			fillFromBinary(array, start, end, arrayQ, 1);
			arrayQ[0] = 0;
			arrayQ[q - 1] = s - 1;
			shapes.push_back(arrayQ);
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

long long int fromBinary(bool* array, int count) {
	long long int ret = 0;
	int tmp;
	for (int i = 0; i < count; i++) {
		tmp = array[i];
		ret |= tmp << (count - i - 1);
	}
	return ret;
}

void toBinary(int* M, int lenM, bool* binary, int size) {
	for (int i = 0; i < size; i++) {
		binary[i] = false;
	}
	for (int i = 0; i < lenM; i++) {
		binary[M[i] - 1] = true;
	}
}

long long int binomialCoefficient(int m, int n) {
	long long int result = 1;
	for (int i = m; i > m - n; i--) {
		result *= i;
	}
	for (int i = 2; i <= n; i++) {
		result /= i;
	}
	return result;
}

/*	======================== TESTS ========================= */

int testThresholdOneShape1() {
	// shape is #-##
	int* Q = new int[3] { 0, 2, 3 };
	int s = 4;
	//int q = 3;
	int k = 3;
	int m = 11;
	return calculateThresholdForShape(s, k, m, Q, 3);
}

int testThresholdOneShape2() {
	int* Q = new int[3] { 0, 1, 3 };
	int s = 4;
	//int q = 3;
	int k = 3;
	int m = 13;
	return calculateThresholdForShape(s, k, m, Q, 3);
}

int* arrayForQ(int q, int k) {
	if (k == 5) {
		if (q == 2) {
			return new int[45] { 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28,
					27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 18, 19, 20, 19, 18,
					17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
		} else if (q == 3) {
			return new int[44] { 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22,
					21, 20, 19, 18, 17, 16, 15, 15, 15, 15, 15, 15, 14, 13, 12,
					11, 10, 10, 10, 11, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
		} else if (q == 4) {
			return new int[43] { 27, 26, 25, 24, 23, 22, 21, 20, 19, 19, 17, 17,
					16, 15, 14, 14, 13, 12, 12, 12, 13, 13, 14, 14, 13, 12, 11,
					10, 9, 8, 7, 6, 6, 7, 7, 7, 6, 5, 4, 3, 2, 1 };
		} else if (q == 5) {
			return new int[42] { 21, 20, 19, 18, 18, 18, 16, 16, 15, 14, 13, 13,
					12, 11, 11, 10, 9, 9, 9, 9, 9, 9, 9, 8, 8, 7, 7, 7, 6, 6, 6,
					5, 4, 4, 4, 4, 5, 4, 3, 2, 1 };
		} else if (q == 6) {
			return new int[41] { 15, 14, 13, 14, 13, 13, 12, 12, 11, 10, 10, 9,
					8, 8, 7, 7, 6, 6, 7, 7, 8, 9, 8, 8, 6, 7, 5, 6, 6, 6, 5, 4,
					3, 3, 2, 3, 3, 3, 2, 1 };
		} else if (q == 7) {
			return new int[40] { 9, 8, 9, 10, 10, 9, 9, 8, 8, 7, 7, 6, 6, 5, 5,
					4, 4, 4, 4, 5, 6, 5, 5, 4, 4, 3, 3, 3, 3, 2, 3, 2, 2, 2, 1,
					1, 2, 2, 1 };
		} else if (q == 8) {
			return new int[39] { 3, 5, 6, 7, 7, 6, 6, 5, 5, 5, 4, 4, 3, 3, 2, 2,
					2, 3, 3, 4, 4, 5, 4, 4, 3, 2, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1,
					1, 1 };
		} else if (q == 9) {
			return new int[38] { 0, 3, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1,
					1, 2, 2, 3, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0,
					0 };
		} else if (q == 10) {
			return new int[37] { 0, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 1,
					1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };
		} else if (q == 11) {
			return new int[36] { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
					1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0 };
		} else if (q == 12) {
			return new int[35] { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		}
	} else if (k == 4) {
		if (q == 2) {
			return new int[45] { 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30,
					29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 20, 21, 20, 19,
					18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2,
					1 };
		} else if (q == 3) {
			return new int[44] { 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25,
					24, 23, 22, 21, 20, 19, 18, 17, 17, 17, 17, 17, 16, 15, 14,
					13, 12, 11, 12, 12, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
		} else if (q == 4) {
			return new int[43] { 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20,
					19, 18, 17, 17, 16, 15, 15, 14, 15, 15, 16, 16, 15, 14, 13,
					12, 11, 10, 9, 8, 7, 8, 8, 8, 7, 6, 5, 4, 3, 2, 1 };
		} else if (q == 5) {
			return new int[42] { 26, 25, 24, 23, 22, 21, 20, 20, 19, 18, 17, 16,
					16, 15, 14, 13, 12, 12, 12, 12, 12, 12, 12, 11, 10, 9, 9, 9,
					8, 8, 8, 7, 6, 5, 5, 5, 6, 5, 4, 3, 2, 1 };
		} else if (q == 6) {
			return new int[41] { 21, 20, 19, 18, 17, 17, 17, 16, 15, 14, 14, 13,
					12, 12, 11, 10, 9, 9, 10, 10, 11, 12, 11, 10, 9, 9, 7, 8, 8,
					8, 7, 6, 5, 4, 3, 4, 4, 4, 3, 2, 1 };
		} else if (q == 7) {
			return new int[40] { 16, 15, 14, 13, 14, 13, 13, 12, 12, 11, 11, 10,
					9, 9, 8, 7, 7, 7, 7, 8, 8, 8, 8, 7, 6, 5, 5, 5, 4, 4, 4, 4,
					4, 3, 2, 2, 3, 3, 2, 1 };
		} else if (q == 8) {
			return new int[39] { 11, 10, 9, 10, 10, 10, 10, 9, 9, 9, 8, 7, 7, 6,
					6, 5, 5, 5, 6, 7, 7, 7, 6, 6, 5, 4, 4, 4, 4, 4, 4, 4, 3, 2,
					2, 2, 2, 2, 1 };
		} else if (q == 9) {
			return new int[38] { 6, 5, 7, 8, 8, 8, 7, 7, 7, 6, 6, 5, 5, 4, 4, 4,
					4, 4, 5, 6, 5, 4, 4, 3, 3, 4, 4, 4, 3, 3, 2, 2, 2, 2, 1, 1,
					1, 1 };
		} else if (q == 10) {
			return new int[37] { 1, 4, 5, 6, 5, 5, 5, 5, 5, 4, 4, 3, 3, 2, 2, 3,
					3, 4, 4, 4, 4, 4, 3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 0,
					1 };
		} else if (q == 11) {
			return new int[36] { 0, 2, 3, 4, 3, 3, 4, 3, 3, 3, 2, 2, 2, 1, 2, 2,
					3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0, 0, 0 };
		} else if (q == 12) {
			return new int[35] { 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 21, 1, 1, 1, 1,
					2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0 };
		} else if (q == 13) {
			return new int[34] { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1,
					1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 };
		} else if (q == 14) {
			return new int[33] { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
					1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		}
	}
	return new int[45];
}

void testThresholdForAllShapesWithSomeQAndKVariableS(int q, int k) {
	int* result;
	int* array = arrayForQ(q, k);
	for (int s = q; s <= (50 - k); s++) {
		int value = calculateThreshold(s, k, 50, q, result);
		bool sat = array[s - q] == value;
		cout << "s: " << s << " threshold: " << value << " satisfied: " << sat
				<< endl;
	}
}
