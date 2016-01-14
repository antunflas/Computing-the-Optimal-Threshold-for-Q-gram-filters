#include <iostream>
#include <limits>
#include <cmath>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

/**
 * Maximum integer value.
 */
static const int MAX_INT = std::numeric_limits<int>::max();
/**
 * Empty vector used for releasing vector memory.
 */
static vector<int*> empty_vector;

int calculateThreshold(int s, int k, int m, int q, int** result);

int calculateThresholdForShapeRecursive(int s, int k, int m, int* arrayQ,
		int lenQ);

int calculateThresholdForShape(int s, int k, int m, int* arrayQ, int lenQ,
		unordered_map<string, int*>& thresholds,
		unordered_map<string, int*>& copy);

int findThreshold(int s, int k, int* arrayQ, int lenQ, int* arrayM, int lenM, int i,
		int j, unordered_map<string, int*>& thresholds, bool* binary);

int getThresholdFor(int s, int k, int j, int* arrayM, int lenM,
		unordered_map<string, int*>& thresholds, bool* binary);

vector<int*> generateShapes(int s, int q);

vector<int*> generateShapesFromPrevious(int s, int q, int k, int m,
		unordered_map<string, int*>& thresholds,
		unordered_map<string, int*>& copy);

bool shapesEqual(int* shape1, int* shape2, int start, int end);

int toBinary(long long int value, bool* array, int size);

void toBinary(int* arrayM, int lenM, bool* binary, int size);

long long int toDecade(bool* binary, int size);

int fillFromBinary(bool* binary, int start, int end, int* arrayM, int offset);

string binaryToString(bool* binary, int size);

void stringToBinary(string str, bool* binary, int size);

string shapeToString(int* shape, int q);

void calculateThresholdForSpanValues(int q, int k);

int* arrayForQ(int q, int k);

/**
 * Program starting point.
 *
 * @param argc number of input arguments.
 * @param argv input arguments array.
 * @return exit code.
 */
int main(int argc, char** argv) {

	if (argc == 3) {
		int k = atoi(argv[1]);
		int q = atoi(argv[2]);
		calculateThresholdForSpanValues(q, k);
	} else if (argc == 5) {
		int m = atoi(argv[1]);
		int k = atoi(argv[2]);
		int q = atoi(argv[3]);
		int s = atoi(argv[4]);
		if(s < q) {
			cerr << "Span must be greater or equal to shape length.";
			exit(1);
		}
		int* result = new int[1];
		int t = calculateThreshold(s, k, m, q, &result);
		cout << t;
		if (t != 0) {
			cout << " " << shapeToString(result, q);
		}
		cout << endl;
	}

	std::string stringManuela;
	std::getline(std::cin, stringManuela);

	return 0;
}

/**
 * Calculates threshold for specified s, k, m and q.
 *
 * @param s span of shape arrayQ.
 * @param k number of differences.
 * @param m string length.
 * @param q length of shape.
 * @param result container for storing calculation result,
 * 		i.e. calculated threshold.
 * @return calculated threshold.
 */
int calculateThreshold(int s, int k, int m, int q, int** result) {
	unordered_map<string, int*> tresholdsMap;
	unordered_map<string, int*> copy;

	/*	generate all possible sets M for j differences and use their binary representation as key in
	 *	thresholds map */
	long long int limit = pow(2, s - 1);
	bool* binary = new bool[s - 1];

	for (long long int counter = 0; counter < limit; counter++) {
		int ones = toBinary(counter, binary, s - 1);
		if (ones < s - 1 - k) {
			continue;
		}

		/*	value for set M is array of all number of differences for which M is
		 * valid combination of matches at last s-1 position*/

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

	//vector<int*> shapes = nextShapes(s, q, k, m, tresholdsMap, copy);
	vector<int*> shapes = generateShapes(s, q);

	int threshold = 0;

	/*	iterate all possible shapes and calculate threshold, choose minimal out of
	 *	all calculated values */
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
		int value = calculateThresholdForShape(s, k, m, shape, q, tresholdsMap,
				copy);
		if (threshold < value) {
			threshold = value;
			*result = shape;
		}
	}
	shapes.clear();
	shapes.swap(empty_vector);
	delete[] binary;
	return threshold;
}

/**
 * Calculates threshold for given shape and specified s, k, m and Q
 * using direct recursive formula.
 *
 * @param s span of shape arrayQ.
 * @param k number of differences.
 * @param m string length.
 * @param arrayQ shape.
 * @param lenQ length of shape.
 * @return calculated threshold.
 */
int calculateThresholdForShapeRecursive(int s, int k, int m, int* arrayQ,
		int lenQ) {

	int* arrayM = new int[s - 1];

	long long int limit = pow(2, s - 1);
	bool* binary = new bool[s - 1];
	int result = MAX_INT;
	for (long long int counter = 0; counter < limit; counter++) {
		toBinary(counter, binary, s - 1);
		fillFromBinary(binary, 1, s - 1, arrayM, 0);
	}
	return result;
}
/**
 * Calculates threshold for given shape and specified s, k, m and Q
 * by alternating all possible combinations of matches at last s-1
 * position.
 *
 * @param s span of shape arrayQ.
 * @param k number of differences.
 * @param m string length.
 * @param arrayQ specified shape.
 * @param lenQ length of shape.
 * @param lenM length of arrayM.
 * @param i current string length.
 * @param j current number of differences.
 * @thresholds current threshold values for all error values below or
 * 			equal to k, for all possible sets M describing the matches
 * 			in the last s-1 positions and for i<=m where i is current
 * 			string length.
 * @copy copies of current threshold values.
 * @return calculated threshold.
 */
int calculateThresholdForShape(int s, int k, int m, int* arrayQ, int lenQ,
		unordered_map<string, int*>& thresholds,
		unordered_map<string, int*>& copy) {

	int* arrayM = new int[s - 1];
	int arrayMLen = 0;
	bool* binary = new bool[s - 1];

	/*	for all lengts up to m update threshold values */
	for (int i = s; i <= m; i++) {
		/*	for each entry in threshold values map, update threshold value. */
		for (auto iterator = thresholds.begin(); iterator != thresholds.end();
				iterator++) {

			/*	convert key which is string representation of M to array of integers. */
			string key = iterator->first;
			stringToBinary(iterator->first, binary, s - 1);

			arrayMLen = fillFromBinary(binary, 1, s - 1, arrayM, 0);

			/**	calculate threshold and temporarily save result to copy. */
			int* copyValue = copy.find(iterator->first)->second;
			int* thresholdsValue = iterator->second;

			for (int j = 1, jLen = thresholdsValue[0] + 1; j < jLen; j++) {

				copyValue[j] = findThreshold(s, k, arrayQ, lenQ, arrayM,
						arrayMLen, i, k - j + 1, thresholds, binary);
			}
		}
		/*	update all values in thresholds with calculated threshold values for length i
		 * stored in copy */
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

	/*	iterate over positions in thresholds that represent k values and choose
	 * minimum as resulted threshold */
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

/**
 * Finds threshold for specified s, k, Q, i, j and M by
 * using calculated thresholds for lower string length.
 *
 * @param s span of shape arrayQ.
 * @param k number of differences.
 * @param arrayQ specified shape.
 * @param lenQ length of shape.
 * @param arrayM matches at last s-1 position.
 * @param lenM length of arrayM.
 * @param i current string length.
 * @param j current number of differences.
 * @param thresholds current threshold values for all error values below or
 * 			equal to k, for all possible sets M describing the matches
 * 			in the last s-1 positions and for i<=m where i is current
 * 			string length.
 * @param binary array for converting values.
 * @return found threshold.
 */
int findThreshold(int s, int k, int* arrayQ, int lenQ, int* arrayM, int lenM, int i,
		int j, unordered_map<string, int*>& thresholds, bool* binary) {

	if (!(j >= 0 && j <= k)) {
		return MAX_INT;
	}

	/*	check if all values in M are in range [1, s-1] */
	for (int c = 0; c < lenM; c++) {
		if (!(arrayM[c] >= 1 && arrayM[c] <= (s - 1))) {
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
		if (arrayM[c] == (s - 1))
			continue;
		nextM1[c + 1] = arrayM[c] + 1;
		lenNextM1++;
		nextM2[c] = arrayM[c] + 1;
		lenNextM2++;
	}

	/*	determine if arrayQ is subset of M U {0} */
	bool isSubset = true;
	for (int c = 0; c < lenQ; c++) {
		if (arrayQ[c] == 0)
			continue;
		bool found = false;
		for (int d = 0; d < lenM; d++) {
			if (arrayQ[c] == arrayM[d]) {
				found = true;
				break;
			}
		}
		if (!found) {
			isSubset = false;
			break;
		}
	}

	/*	calculate nextJ:
	 *  j	- if there is match at position i, i.e. M contains s-1
	 *  j-1	- otherwise*/
	int nextJ = j;
	bool contains = false;
	for (int c = 0; c < lenM; c++) {
		if (arrayM[c] == (s - 1)) {
			contains = true;
			break;
		}
	}
	if (!contains)
		nextJ--;

	int val = min(
			getThresholdFor(s, k, nextJ, nextM1, lenNextM1, thresholds, binary)
					+ (isSubset ? 1 : 0),
			getThresholdFor(s, k, nextJ, nextM2, lenNextM2, thresholds,
					binary));
	free(nextM1);
	free(nextM2);
	return val;
}

/**
 * Retrieves threshold from currently stored threshold values
 * for given number of differences j and given set of matches
 * at last s-1 positions M.
 *
 * @param s span of shape.
 * @param k number of differences.
 * @param j current number of differences.
 * @param arrayM matches at last s - 1 position.
 * @param lenM length of arrayM.
 * @param thresholds current threshold values for all error values below or
 * 			equal to k, for all possible sets M describing the matches
 * 			in the last s-1 positions and for i<=m where i is current
 * 			string length.
 * @param binary array for converting values.
 * @return threshold for specified j and M.
 */
int getThresholdFor(int s, int k, int j, int* arrayM, int lenM,
		unordered_map<string, int*>& thresholds, bool* binary) {

	/*	check if j is out of bound */
	if (!(j >= 0 && j <= k)) {
		return MAX_INT;
	}

	/*	check if all values in M are in range [1, s-1] */
	for (int c = 0; c < lenM; c++) {
		if (!(arrayM[c] >= 1 && arrayM[c] <= (s - 1))) {
			return MAX_INT;
		}
	}

	/*	check if lenM is out of bound */
	if (lenM < (s - 1 - j)) {
		return MAX_INT;
	}

	toBinary(arrayM, lenM, binary, s - 1);
	int indexJ = k - j + 1;
	string str = binaryToString(binary, s - 1);
	int val = thresholds.find(str)->second[indexJ];
	return val;
}

/**
 * Generates all possible shapes for specified s and q.
 *
 * @param s span of shape.
 * @param q size of shape.
 * @return generated shapes.
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

/**
 * Checks whether two shapes are equal at defined positions.
 *
 * @param shape1 First shape.
 * @param shape2 Second shape.
 * @return true if shapes are equal at defined positions,
 * 		false otherwise.
 */
bool shapesEqual(int* shape1, int* shape2, int start, int end) {
	for (int i = start; i < end; i++) {
		if (shape1[i] != shape2[i]) {
			return false;
		}
	}
	return true;
}

/**
 * Generates (q,s)-shapes from (q-1,s)-shapes considering only those that gave
 * positive thresholds.
 *
 * @param s span of shape Q.
 * @param q size of shape Q.
 * @param k number of differences.
 * @param m string length.
 * @param thresholds current threshold values for all error values below or
 * 			equal to k, for all possible sets M describing the matches
 * 			in the last s-1 positions and for i<=m where i is current
 * 			string length.
 * @param copy copies of current threshold values.
 * @return generated shapes.
 */
vector<int*> generateShapesFromPrevious(int s, int q, int k, int m,
		unordered_map<string, int*>& thresholds,
		unordered_map<string, int*>& copy) {

	if (q < 4) {
		return generateShapes(s, q);
	}

	vector<int*> shapesQS = generateShapesFromPrevious(s, q - 1, k, m,
			thresholds, copy);

	/*	Extract shapes that gave positive threshold. */

	vector<int*> shapePositive;
	for (unsigned int i = 0; i < shapesQS.size(); i++) {
		for (auto iterator = thresholds.begin(); iterator != thresholds.end();
				iterator++) {
			int* copyValue = copy.find(iterator->first)->second;
			int* thresholdValue = iterator->second;
			for (int j = 1, jLen = thresholdValue[0] + 1; j < jLen; j++) {
				iterator->second[j] = 0;
				copyValue[j] = 0;
			}
		}

		if (calculateThresholdForShape(s, k, m, shapesQS[i], q - 1, thresholds,
				copy) > 0) {
			shapePositive.push_back(shapesQS[i]);
		}
	}

	/*	Construct groups of shapes that only differ in second to last
	 * position. */

	vector<vector<int*>> shapeSets;

	while (shapePositive.size() > 0) {
		vector<int*> tempSet;
		tempSet.push_back(shapePositive[0]);
		for (unsigned int j = shapePositive.size() - 1; j > 0; j--) {
			if ((shapePositive[0][q - 3] != shapePositive[j][q - 3])
					&& shapesEqual(shapePositive[0], shapePositive[j], 1,
							q - 3)) {
				tempSet.push_back(shapePositive[j]);
				shapePositive.erase(shapePositive.begin() + j);
			}
		}
		shapePositive.erase(shapePositive.begin());
		shapeSets.push_back(tempSet);
	}

	/*	From each group of (q-1,s)-shapes we generate new shape for all pairs in group by making union
	 * of two paired shapes. That will result in new (q,s)-shape since each pair in group only
	 * differs in one position thus adding one new element to union. */

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

/**
 * Fills arrayM with numbers from start to end shifted by index of binary value.
 * Only ones in binary are considered, positions with zero value are skipped.
 * That corresponds to having an array of [start, start + 1, ..., end] and only
 * copy those values from array at positions where is one in binary.
 * Start and end are both inclusive.
 *
 * @param binary value.
 * @start first value to be set in arrayM.
 * @return length of constructed arrayM.
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

/**
 * Converts long number to binary array.
 *
 * @param value number to convert.
 * @param array binary array to be filled with corresponding binary values.
 * @return number of ones in array.
 */
int toBinary(long long int value, bool* array, int size) {
	int counter = 0;
	for (int i = 0; i < size; i++) {
		counter += array[size - i - 1] = value & (1 << i);
	}
	return counter;
}

/**
 * Converts binary to string representation of zero and ones.
 *
 * @param binary array from which string is created.
 * @param size size of binary array.
 * @return string representation of specified binary.
 */
string binaryToString(bool* binary, int size) {
	string str;
	str.reserve(size);
	for (int i = 0; i < size; i++) {
		str += (binary[i] ? "1" : "0");
	}
	return str;
}

/**
 * Converts string to binary array. String is expected to be filled
 * with zeros and ones.
 *
 * @param str String to be converted to binary.
 * @param binary array to be filled with binary values.
 */
void stringToBinary(string str, bool* binary, int size) {
	for (unsigned int i = 0; i < str.length(); i++) {
		if (str[i] == '1') {
			binary[i] = true;
		} else {
			binary[i] = false;
		}
	}
}

/**
 * Converts binary to decade number.
 *
 * @param binary array representing binary values.
 * @param size size of binary.
 * @return decade value of binary.
 */
long long int toDecade(bool* binary, int size) {
	long long int ret = 0;
	int tmp;
	for (int i = 0; i < size; i++) {
		tmp = binary[i];
		ret |= tmp << (size - i - 1);
	}
	return ret;
}

/**
 * Creates binary array from set M with integer values. Values in binary are set
 * at positions defined in M decreased by 1 since minimal value defined in M is 1.
 *
 * @param arrayM matches at last s - 1 position.
 * @param lenM length of arrayM.
 * @param binary array to be filled.
 * @param size size of binary.
 *
 */
void toBinary(int* arrayM, int lenM, bool* binary, int size) {
	for (int i = 0; i < size; i++) {
		binary[i] = false;
	}
	for (int i = 0; i < lenM; i++) {
		binary[arrayM[i] - 1] = true;
	}
}

/**
 * Converts shape to string representation.
 *
 * @param shape to convert.
 * @param q length of shape.
 * @return string representation of shape.
 */
string shapeToString(int* shape, int q) {
	string str = "#";
	int index = 1;
	for (int counter = 1, len = shape[q - 1]; counter <= len; counter++) {
		if (shape[index] == counter) {
			str += "#";
			index++;
		} else {
			str += "-";
		}
	}
	return str;
}

/**
 * Retrieves expected threshold for string of length 50 and specified k and q
 * and all possible span values.
 *
 * @param q length of shape.
 * @param k number of differences.
 * @return threshold values.
 */
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

/**
 * Calculates threshold for string of length 50 and specified k and q
 * by varying all possible span values.
 *
 * @param q length of shape.
 * @param k number of differences.
 */
void calculateThresholdForSpanValues(int q, int k) {
	int* result = new int[1];
	int* array = arrayForQ(q, k);
	for (int s = q, len = 50 - k; s <= len; s++) {
		int value = calculateThreshold(s, k, 50, q, &result);
		bool sat = array[s - q] == value;
		cout << "s: " << s << " threshold: " << value << " satisfied: " << sat
				<< endl;
	}
}
