#include <iostream>
#include <limits>
#include <cmath>
#include <stdlib.h>
#include <algorithm>
#include <string>

using namespace std;

static const int MAX_INT = std::numeric_limits<int>::max();

int CalculateThreshold(int s, int k, int m, int q, int** result);

int** GenerateShapes(int s, int q, int* len_shapes);

int CalculateThresholdForShape(int s, int k, int m, int* array_q, int len_q);

int FindThreshold(int s, int k, int* array_q, int len_q, int* array_m, int len_m, int i,
		int j);

int ToBinary(long long int value, bool* array, int size);

long long int ToDecade(bool* array, int count);

int FillFromBinary(bool* binary, int start, int end, int* array_m, int offset);

string ShapeToString(int* shape, int q);

void CalculateThresholdForSpanValues(int q, int k);

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
			if(!(k == 4 || k == 5)) {
				cerr << "Distance measure must be 4 or 5.";
				exit(1);
			}
			CalculateThresholdForSpanValues(q, k);
		} else if (argc == 5) {
			int m = atoi(argv[1]);
			int k = atoi(argv[2]);
			int q = atoi(argv[3]);
			int s = atoi(argv[4]);
			if (s < q) {
				cerr << "Span must be greater or equal to shape length.";
				exit(1);
			}
			int* result = new int[1];
			int t = CalculateThreshold(s, k, m, q, &result);
			cout << t;
			if (t != 0) {
				cout << "\t" << ShapeToString(result, q);
			}
			cout << endl;
		} else {
			cerr << "Program expects 2 or 4 arguments. " << endl;
			cerr << "For calculating threshold in general, arguments are: m k q s" << endl;
			cerr << "\tm: string length." << endl;
			cerr << "\tk: distance measure between pattern and string." << endl;
			cerr << "\tq: length of shape." << endl;
			cerr << "\ts: span of shape." << endl;
			cerr << "For running test version for m=50 and all possible spans, arguments are: k q" << endl;
			cerr << "\tk: distance measure between pattern and string. Valid values are 4 or 5." << endl;
			cerr << "\tq: length of shape." << endl;
		}

		return 0;
}

/**
 * Calculates threshold for specified s, k, m and q.
 *
 * @param s span of shape arrayQ.
 * @param k distance measure.
 * @param m string length.
 * @param q length of shape.
 * @param result container for storing calculation result,
 * 		i.e. shape of calculated threshold.
 * @return calculated threshold.
 */
int CalculateThreshold(int s, int k, int m, int q, int** result) {
	int len_shapes = 0;
	int** shapes = GenerateShapes(s, q, &len_shapes);
	int threshold = 0;
	for (int i = 0; i < len_shapes; i++) {
		int* shape = shapes[i];
		int value = CalculateThresholdForShape(s, k, m, shape, q);
		if (threshold < value) {
			threshold = value;
			*result = shape;
		}
	}
	return threshold;
}

/**
 * Calculates threshold for given shape and specified s, k, m and Q
 * using direct recursive formula.
 *
 * @param s span of shape array_q.
 * @param k number of differences.
 * @param m string length.
 * @param array_q shape.
 * @param len_q length of shape.
 * @return calculated threshold.
 */
int CalculateThresholdForShape(int s, int k, int m, int* array_q,
		int len_q) {
	int* array_m = new int[s - 1];
	int len_m = 0;

	long long int limit = pow(2, s - 1);
	bool* binary = new bool[s - 1];
	int result = MAX_INT;
	for (long long int counter = 0; counter < limit; counter++) {
		ToBinary(counter, binary, s - 1);
		len_m = FillFromBinary(binary, 1, s - 1, array_m, 0);
		result = min(
				FindThreshold(s, k, array_q, len_q, array_m, len_m, m, k),
				result);
	}
	free(array_m);
	delete[] binary;
	return result;
}

/**
 * Finds threshold for specified s, k, Q, i, j and M.
 *
 * @param s span of shape array_q.
 * @param k number of differences.
 * @param array_q specified shape.
 * @param len_q length of shape.
 * @param array_m matches at last s-1 position.
 * @param len_m length of array_m.
 * @param i current string length.
 * @param j current number of differences.
 * @return found threshold.
 */
int FindThreshold(int s, int k, int* array_q, int len_q, int* array_m, int len_m, int i,
		int j) {

	/*	check constraints */

	if (!(j >= 0 && j <= k)) {
		return MAX_INT;
	}
	for (int c = 0; c < len_m; c++) {
		if (!(array_m[c] >= 1 && array_m[c] <= (s - 1))) {
			return MAX_INT;
		}
	}

	if (len_m < (s - 1 - j)) {
		return MAX_INT;
	}
	if (i < s) {
		return 0;
	}

	/*	generate next Ms */
	int* next_m_first = new int[len_m + 1];
	int* next_m_second = new int[len_m];
	int len_next_m_first = 1;
	int len_next_m_2 = 0;
	next_m_first[0] = 1;
	for (int c = 0; c < len_m; c++) {
		if (array_m[c] == (s - 1))
			continue;
		next_m_first[c + 1] = array_m[c] + 1;
		len_next_m_first++;
		next_m_second[c] = array_m[c] + 1;
		len_next_m_2++;
	}

	/*	determine if arrayQ is subset of M U {0} */
	bool is_subset = true;
	for (int c = 0; c < len_q; c++) {
		if (array_q[c] == 0)
			continue;
		bool found = false;
		for (int d = 0; d < len_m; d++) {
			if (array_q[c] == array_m[d]) {
				found = true;
				break;
			}
		}
		if (!found) {
			is_subset = false;
			break;
		}
	}

	/*	calculate next_j:
	 *  j	- if there is match at position i, i.e. M contains s-1
	 *  j-1	- otherwise*/
	int next_j = j;
	bool contains = false;
	for (int c = 0; c < len_m; c++) {
		if (array_m[c] == (s - 1)) {
			contains = true;
			break;
		}
	}
	if (!contains)
		next_j--;

	int result = min(
			FindThreshold(s, k, array_q, len_q, next_m_first, len_next_m_first, i - 1, next_j)
					+ (is_subset ? 1 : 0),
			FindThreshold(s, k, array_q, len_q, next_m_second, len_next_m_2, i - 1, next_j));
	free(next_m_first);
	free(next_m_second);
	return result;
}

/**
 * Generates all possible shapes for specified s and q.
 *
 * @param s span of shape.
 * @param q size of shape.
 * @param len_shapes length of shapes.
 * @return generated shapes.
 */
int** GenerateShapes(int s, int q, int* len_shapes) {
	int start = 1;
	int end = s - 2;
	int len = end - start + 1;
	long long int countTo = pow(2, len);
	int** shapes = new int*[countTo];
	*len_shapes = 0;
	bool* array = new bool[len];
	for (long long int counter = 0; counter < countTo; counter++) {
		int onesCount = ToBinary(counter, array, len);
		if (onesCount == (q - 2)) {
			int* arrayQ = new int[q];
			FillFromBinary(array, start, end, arrayQ, 1);
			arrayQ[0] = 0;
			arrayQ[q - 1] = s - 1;
			shapes[*len_shapes] = arrayQ;
			*len_shapes = *len_shapes + 1;
		}
	}
	return shapes;
}

/**
 * Fills array_m with numbers from start to end shifted by index of binary value.
 * Only ones in binary are considered, positions with zero value are skipped.
 * That corresponds to having an array of [start, start + 1, ..., end] and only
 * copy those values from array at positions where is one in binary.
 * Start and end are both inclusive.
 *
 * @param binary value.
 * @param start first value that could be set in array_m.
 * @param end last value that could be set in array_m.
 * @param array_m matches at last s - 1 position.
 * @param offset offset applied to values.
 * @return length of constructed array_m.
 */
int FillFromBinary(bool* binary, int start, int end, int* array_m, int offset) {
	int len = end - start + 1;
	int arrayMLen = offset;
	for (int c = 0; c < len; c++) {
		if (binary[c] == 1) {
			array_m[arrayMLen++] = start + c;
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
int ToBinary(long long int value, bool* array, int size) {
	int counter = 0;
	for (int i = 0; i < size; i++) {
		counter += array[size - i - 1] = value & (1 << i);
	}
	return counter;
}

/**
 * Converts binary to decade number.
 *
 * @param binary array representing binary values.
 * @param size size of binary.
 * @return decade value of binary.
 */
long long int ToDecade(bool* array, int count) {
	long long int ret = 0;
	int tmp;
	for (int i = 0; i < count; i++) {
		tmp = array[i];
		ret |= tmp << (count - i - 1);
	}
	return ret;
}

/**
 * Converts shape to string representation.
 *
 * @param shape to convert.
 * @param q length of shape.
 * @return string representation of shape.
 */
string ShapeToString(int* shape, int q) {
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
int* ArrayForQ(int q, int k) {
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
void CalculateThresholdForSpanValues(int q, int k) {
	int* result = new int[1];
	int* array = ArrayForQ(q, k);
	for (int s = q; s <= (50 - k); s++) {
		int value = CalculateThreshold(s, k, 50, q, &result);
		bool sat = array[s - q] == value;
		cout << "s: " << s << " threshold: " << value << " satisfied: " << sat
				<< endl;
	}
}
