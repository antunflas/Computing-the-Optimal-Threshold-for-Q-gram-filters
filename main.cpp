#include <iostream>
#include <limits>
#include <cmath>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

static const int MAX_INT = std::numeric_limits<int>::max();
static vector<int*> empty_vector;

/*  poèetna funkcija koju zovemo iz maina */
int calculateThreshold(int s, int k, int m, int q, int* result, bool newShape);

/*  generira sve Q koje treba ispitati
 kasnije bi možda bilo pametno ovo preoblikovati tako da izraèuna koji su pozitivni thresholdi pa vrati samo te Q.
 */
vector<int*> generateShapes(int s, int q);

/**/
bool AreShapesEqual(int* a, int* b, int start, int end);

/**/
vector<int*> nextShapes(int s, int q, int k, int m, unordered_map<string, int*>& tresholdsMap,
		unordered_map<string, int*>& copy);

/*  raèuna threshold za odreðeni Q */
int calculateThresholdForShape(int s, int k, int m, int* arrayQ, int arrayQLen);

/**/
int myCalculateThresholdForShape(int s, int k, int m, int* arrayQ,
		int arrayQLen, unordered_map<string, int*>& thresholds,
		unordered_map<string, int*>& copy);

/*  ona rekurzivna funkcija */
int findThreshold(int s, int k, int* Q, int lenQ, int* M, int lenM, int i,
		int j, unordered_map<string, int*>& tresholds, bool* binary);
/**/
int getTresholdFor(int s, int k, int j, int* M, int lenM,
		unordered_map<string, int*>& tresholds, bool* binary);

/*	*/
int toBinary(long long int value, bool* array, int size);

/**/
void toBinary(int* M, int lenM, bool* binary, int size);

/* */
long long int fromBinary(bool* array, int count);

/*	*/
int fillFromBinary(bool* binary, int start, int end, int* arrayM, int offset);

/**/
long long int binomialCoefficient(int m, int n);

/**/
long long int calculateThresholdArrayLength(int k, int s);

/**/
string binaryToString(bool* binary, int size);

/**/
bool* stringToBinary(string str, bool* binary, int size);

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

	long long int tresholdsArrayLength = 0;
	for (int j = 0; j <= k; j++) {
		tresholdsArrayLength += binomialCoefficient(s - 1, j);
	}

	long long int offset = pow(2, s - 1) - tresholdsArrayLength;

	if (argc >= 3) {
		k = atoi(argv[1]);
		q = atoi(argv[2]);
	}

	k = atoi(argv[1]);
	q = atoi(argv[2]);

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

int calculateThreshold(int s, int k, int m, int q, int* result, bool newShape) {

	int shapesLen = 0;

	vector<int*> shapes;

	unordered_map<string, int*> tresholdsMap;
	unordered_map<string, int*> copy;

	int* arrayM = new int[s - 1];
	int arrayMLen = 0;

	long long int limit = pow(2, s - 1);
	bool* binary = new bool[s - 1];

	for (long long int counter = 0; counter < limit; counter++) {
		int ones = toBinary(counter, binary, s - 1);
		if (ones < s - 1 - k) {
			continue;
		}

		arrayMLen = 0;

		int* arrayJ = new int[1 + k - (s - 1 - ones) + 1];
		arrayJ[0] = k - (s - 1 - ones) + 1;

		int* copyValue = new int[1 + k - (s - 1 - ones) + 1];
		copyValue[0] = k - (s - 1 - ones) + 1;
		for (int j = 1, jLen = copyValue[0]; j < jLen; j++) {
			arrayJ[j] = 0;
			copyValue[j] = 0;
		}
		string key = binaryToString(binary, s - 1);
		pair<string, int*> copyPair(key, copyValue);
		pair<string, int*> mypair(key, arrayJ);
		copy.insert(copyPair);
		tresholdsMap.insert(mypair);
	}

	shapes = nextShapes(s, q, k, m, tresholdsMap, copy);
	cout << "Završio" << endl;

	int threshold = 0;

	//////////////////////////////

	for (unsigned int i = 0, shapeLen = shapes.size(); i < shapeLen; i++) {
		for (auto iterator = tresholdsMap.begin();
				iterator != tresholdsMap.end(); iterator++) {
			int* copyValue = copy.find(iterator->first)->second;
			int* thresholdValue = iterator->second;
			for (int j = 1, jLen = thresholdValue[0] + 1; j < jLen; j++) {
				iterator->second[j] = 0;
				copyValue[j] = 0;
			}
		}

		int* shape = shapes[i];
		int value = myCalculateThresholdForShape(s, k, m, shape, q,
				tresholdsMap, copy);
		if (threshold < value) {
			threshold = value;
			result = shape;
		}
	}
	shapes.clear();
	shapes.swap(empty_vector);
	delete[] binary;
	return threshold;
}

int calculateThresholdForShape(int s, int k, int m, int* arrayQ,
		int arrayQLen) {

	int* arrayM = new int[s - 1];
	int arrayMLen = 0;

	long long int limit = pow(2, s - 1);
	bool* binary = new bool[s - 1];
	int result = MAX_INT;
	for (long long int counter = 0; counter < limit; counter++) {
		toBinary(counter, binary, s - 1);
		arrayMLen = fillFromBinary(binary, 1, s - 1, arrayM, 0);
	}
	return result;
}

int myCalculateThresholdForShape(int s, int k, int m, int* arrayQ,
		int arrayQLen, unordered_map<string, int*>& thresholds,
		unordered_map<string, int*>& copy) {

	int* arrayM = new int[s - 1];
	int arrayMLen = 0;
	bool* binary = new bool[s - 1];

	for (int i = s; i <= m; i++) {
		for (auto iterator = thresholds.begin(); iterator != thresholds.end();
				iterator++) {

			string key = iterator->first;
			bool* bin = stringToBinary(iterator->first, binary, s - 1);

			arrayMLen = fillFromBinary(bin, 1, s - 1, arrayM, 0);

			int* copyValue = copy.find(iterator->first)->second;
			int* thresholdsValue = iterator->second;

			for (int j = 1, jLen = thresholdsValue[0] + 1; j < jLen; j++) {

				copyValue[j] = findThreshold(s, k, arrayQ, arrayQLen, arrayM,
						arrayMLen, i, k - j + 1, thresholds, binary);
			}
		}
		for (auto iterator = thresholds.begin(); iterator != thresholds.end();
				iterator++) {

			int* copyValue = copy.find(iterator->first)->second;
			int* thresholdValue = iterator->second;
			for (int j = 1, jLen = thresholdValue[0] + 1; j < jLen; j++) {
				thresholdValue[j] = copyValue[j];
			}
		}
	}

	int result = MAX_INT;

	for (auto iterator = thresholds.begin(); iterator != thresholds.end();
			iterator++) {

		int* thresholdValue = iterator->second;
		for (int j = 1, jLen = thresholdValue[0] + 1; j < jLen; j++) {
			if (result > thresholdValue[j]) {
				result = thresholdValue[j];
			}
		}
	}
	free(arrayM);
	delete[] binary;
	return result;
}
/*	check constraints */
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

int findThreshold(int s, int k, int* Q, int lenQ, int* M, int lenM, int i,
		int j, unordered_map<string, int*>& tresholds, bool* binaryM) {

	if (!(j >= 0 && j <= k)) {	// check if j is out of bound
		return MAX_INT;
	}
	for (int c = 0; c < lenM; c++) {
		if (!(M[c] >= 1 && M[c] <= (s - 1))) {	// check if M is out of bound
			return MAX_INT;
		}
	}

	if (lenM < (s - 1 - j)) {	// check if lenM is out of bound
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

	int val = min(
			getTresholdFor(s, k, nextJ, nextM1, lenNextM1, tresholds, binaryM)
					+ (isSubset ? 1 : 0),
			getTresholdFor(s, k, nextJ, nextM2, lenNextM2, tresholds, binaryM));
	free(nextM1);
	free(nextM2);
	return val;
}

int getTresholdFor(int s, int k, int j, int* M, int lenM,
		unordered_map<string, int*>& tresholds, bool* mBinary) {

	if (!(j >= 0 && j <= k)) {	// check if j is out of bound
		return MAX_INT;
	}
	for (int c = 0; c < lenM; c++) {
		if (!(M[c] >= 1 && M[c] <= (s - 1))) {	// check if M is out of bound
			return MAX_INT;
		}
	}

	if (lenM < (s - 1 - j)) {	// check if lenM is out of bound
		return MAX_INT;
	}

	toBinary(M, lenM, mBinary, s - 1);
	int indexJ = k - j + 1;
	string str = binaryToString(mBinary, s - 1);
	int val = tresholds.find(str)->second[indexJ];
	return val;
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

bool AreShapesEqual(int* a, int* b, int start, int end) {
	for (int i = start; i < end; i++) {
		if (a[i] != b[i]) {
			return false;
		}
	}
	return true;
}

vector<int*> nextShapes(int s, int q, int k, int m, unordered_map<string, int*>& tresholdsMap,
		unordered_map<string, int*>& copy) {

	if (q < 4) {
		return generateShapes(s, q);
	}

	vector<int*> shapesQS = nextShapes(s, q - 1, k, m, tresholdsMap, copy);


	vector<int*> shapePositive;
	for (unsigned int i = 0; i < shapesQS.size(); i++) {
		for (auto iterator = tresholdsMap.begin();
				iterator != tresholdsMap.end(); iterator++) {
			int* copyValue = copy.find(iterator->first)->second;
			int* thresholdValue = iterator->second;
			for (int j = 1, jLen = thresholdValue[0] + 1; j < jLen; j++) {
				iterator->second[j] = 0;
				copyValue[j] = 0;
			}
		}

		if (myCalculateThresholdForShape(s, k, m, shapesQS[i], q - 1,
				tresholdsMap, copy) > 0) {
			shapePositive.push_back(shapesQS[i]);
		}
	}
	vector<vector<int*>> shapeSets;

	while (shapePositive.size() > 0) {
		vector<int> indexes;
		vector<int*> tempSet;
		tempSet.push_back(shapePositive[0]);
		indexes.push_back(0);
		for (unsigned int j = shapePositive.size() - 1; j > 0; j--) {
			if ((shapePositive[0][q - 3] != shapePositive[j][q - 3])
					&& AreShapesEqual(shapePositive[0], shapePositive[j], 1,
							q - 3)) {
				tempSet.push_back(shapePositive[j]);
				shapePositive.erase(shapePositive.begin() + j);
			}
		}
		shapePositive.erase(shapePositive.begin());
		shapeSets.push_back(tempSet);
	}

	vector<int*> result;

	for (unsigned int i = 0, lenGroups = shapeSets.size(); i < lenGroups; i++) {

		for (unsigned int j = 0, lenShape = shapeSets[i].size(); j < lenShape;
				j++) {

			int* baseShape = shapeSets[i][j];
			for (unsigned int k = j + 1; k < shapeSets[i].size(); k++) {
				int* temp = new int[s + 1];
				for (int e = 0; e < q - 3; e++) {
					temp[e] = baseShape[e];
				}
				if (baseShape[q - 3] > shapeSets[i][k][q - 3]) {
					temp[q - 3] = shapeSets[i][k][q - 3];
					temp[q - 2] = baseShape[q - 3];
				} else {
					temp[q - 3] = baseShape[q - 3];
					temp[q - 2] = shapeSets[i][k][q - 3];
				}
				temp[q - 1] = baseShape[q - 2];
				result.push_back(temp);
			}
		}
	}

	return result;
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

string binaryToString(bool* binary, int size) {
	string str;
	str.reserve(size);
	for (int i = 0; i < size; i++) {
		str += (binary[i] ? "1" : "0");
	}
	return str;
}

bool* stringToBinary(string str, bool* binary, int size) {
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == '1') {
			binary[i] = true;
		} else {
			binary[i] = false;
		}
	}

	return binary;
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

/*	======================== TESTS ========================= */

int testThresholdOneShape1() {
	// shape is #-##
	int* Q = new int[3] { 0, 2, 3 };
	int s = 4;
	int k = 3;
	int m = 11;
	return calculateThresholdForShape(s, k, m, Q, 3);
}

int testThresholdOneShape2() {
	int* Q = new int[3] { 0, 1, 3 };
	int s = 4;
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
	int* result = new int[1];
	int* array = arrayForQ(q, k);
	for (int s = 20; s <= (50 - k); s++) {
		int value = calculateThreshold(s, k, 50, q, result, false);
		bool sat = array[s - q] == value;
		cout << "s: " << s << " threshold: " << value << " satisfied: " << sat
				<< endl;
	}
}
