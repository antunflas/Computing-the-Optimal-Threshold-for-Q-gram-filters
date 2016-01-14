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

int CalculateThreshold(int s, int k, int m, int q, int** result);

int CalculateThresholdForShapeRecursive(int s, int k, int m, int* array_q,
		int len_q);

int CalculateThresholdForShape(int s, int k, int m, int* array_q, int len_q,
		unordered_map<string, int*>& thresholds,
		unordered_map<string, int*>& copy);

int FindThreshold(int s, int k, int* array_q, int len_q, int* array_m,
		int len_m, int i, int j, unordered_map<string, int*>& thresholds,
		bool* binary);

int GetThresholdFor(int s, int k, int j, int* array_m, int len_m,
		unordered_map<string, int*>& thresholds, bool* binary);

vector<int*> GenerateShapes(int s, int q);

vector<int*> GenerateShapesFromPrevious(int s, int q, int k, int m,
		unordered_map<string, int*>& thresholds,
		unordered_map<string, int*>& copy);

bool ShapesEqual(int* first_shape, int* second_shape, int start, int end);

int ToBinary(long long int value, bool* array, int size);

void ToBinary(int* array_m, int len_m, bool* binary, int size);

long long int ToDecade(bool* binary, int size);

int FillFromBinary(bool* binary, int start, int end, int* array_m, int offset);

string BinaryToString(bool* binary, int size);

void StringToBinary(string str, bool* binary, int size);

string ShapeToString(int* shape, int q);

void CalculateThresholdForSpanValues(int q, int k);

int* ArrayForQ(int q, int k);

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
 * 		i.e. calculated threshold.
 * @return calculated threshold.
 */
int CalculateThreshold(int s, int k, int m, int q, int** result) {
	unordered_map<string, int*> thresholds;
	unordered_map<string, int*> copy;

	/*	generate all possible sets M for j differences and use their binary representation as key in
	 *	thresholds map */
	long long int limit = pow(2, s - 1);
	bool* binary = new bool[s - 1];

	for (long long int counter = 0; counter < limit; counter++) {
		int ones = ToBinary(counter, binary, s - 1);
		if (ones < s - 1 - k) {
			continue;
		}

		/*	value for set M is array of all number of differences for which M is
		 *	valid combination of matches at last s-1 position.
		 *	There are k - (s - 1 - ones) + 1 of them.
		 *	This length is stored on first position at value array. */

		int* array_j = new int[1 + k - (s - 1 - ones) + 1];
		array_j[0] = k - (s - 1 - ones) + 1;

		int* copy_value = new int[1 + k - (s - 1 - ones) + 1];
		copy_value[0] = k - (s - 1 - ones) + 1;
		for (int j = 1, j_len = copy_value[0]; j < j_len; j++) {
			array_j[j] = 0;
			copy_value[j] = 0;
		}
		string key = BinaryToString(binary, s - 1);
		pair<string, int*> copy_pair(key, copy_value);
		pair<string, int*> thresholds_pair(key, array_j);
		copy.insert(copy_pair);
		thresholds.insert(thresholds_pair);
	}

	//vector<int*> shapes = GenerateShapesFromPrevious(s, q, k, m, thresholds, copy);
	vector<int*> shapes = GenerateShapes(s, q);

	int threshold = 0;

	/*	iterate all possible shapes and calculate threshold, choose minimal out of
	 *	all calculated values */
	for (unsigned int i = 0, shapes_len = shapes.size(); i < shapes_len; i++) {
		for (auto iterator = thresholds.begin(); iterator != thresholds.end();
				iterator++) {
			int* copy_value = copy.find(iterator->first)->second;
			int* threshold_value = iterator->second;
			for (int j = 1, j_len = threshold_value[0] + 1; j < j_len; j++) {
				iterator->second[j] = 0;
				copy_value[j] = 0;
			}
		}

		int* shape = shapes[i];
		int value = CalculateThresholdForShape(s, k, m, shape, q, thresholds,
				copy);
		if (threshold < value) {
			threshold = value;
			*result = shape;
		}
	}
	shapes.clear();
	shapes.swap(empty_vector);
	delete[] binary;

	for (auto iterator = thresholds.begin(); iterator != thresholds.end();
					iterator++) {
		delete[] iterator->second;
	}
	for (auto iterator = copy.begin(); iterator != copy.end();
					iterator++) {
		delete[] iterator->second;
	}

	thresholds.clear();
	copy.clear();
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
int CalculateThresholdForShapeRecursive(int s, int k, int m, int* array_q,
		int len_q) {

	int* array_m = new int[s - 1];

	long long int limit = pow(2, s - 1);
	bool* binary = new bool[s - 1];
	int result = MAX_INT;
	for (long long int counter = 0; counter < limit; counter++) {
		ToBinary(counter, binary, s - 1);
		FillFromBinary(binary, 1, s - 1, array_m, 0);
	}
	return result;
}
/**
 * Calculates threshold for given shape and specified s, k, m and Q
 * by alternating all possible combinations of matches at last s-1
 * position.
 *
 * @param s span of shape array_q.
 * @param k distance measure.
 * @param m string length.
 * @param array_q specified shape.
 * @param len_q length of shape.
 * @thresholds current threshold values for all error values below or
 * 			equal to k, for all possible sets M describing the matches
 * 			in the last s-1 positions and for i<=m where i is current
 * 			string length.
 * @copy copies of current threshold values.
 * @return calculated threshold.
 */
int CalculateThresholdForShape(int s, int k, int m, int* array_q, int len_q,
		unordered_map<string, int*>& thresholds,
		unordered_map<string, int*>& copy) {

	int* array_m = new int[s - 1];
	int array_m_len = 0;
	bool* binary = new bool[s - 1];

	/*	for all lengths up to m update threshold values */
	for (int i = s; i <= m; i++) {
		/*	for each entry in threshold values map, update threshold value. */
		for (auto iterator = thresholds.begin(); iterator != thresholds.end();
				iterator++) {

			/*	convert key which is string representation of M to array of integers. */
			string key = iterator->first;
			StringToBinary(iterator->first, binary, s - 1);

			array_m_len = FillFromBinary(binary, 1, s - 1, array_m, 0);

			/**	calculate threshold and temporarily save result to copy. */
			int* copy_value = copy.find(iterator->first)->second;
			int* thresholds_value = iterator->second;

			for (int j = 1, j_len = thresholds_value[0] + 1; j < j_len; j++) {

				copy_value[j] = FindThreshold(s, k, array_q, len_q, array_m,
						array_m_len, i, k - j + 1, thresholds, binary);
			}
		}
		/*	update all values in thresholds with calculated threshold values for length i
		 * stored in copy */
		for (auto iterator = thresholds.begin(); iterator != thresholds.end();
				iterator++) {

			int* copy_value = copy.find(iterator->first)->second;
			int* threshold_value = iterator->second;
			for (int j = 1, j_len = threshold_value[0] + 1; j < j_len; j++) {
				threshold_value[j] = copy_value[j];
			}
		}
	}

	int result = MAX_INT;

	/*	iterate over positions in thresholds that represent k values and choose
	 * minimum as resulted threshold */
	for (auto iterator = thresholds.begin(); iterator != thresholds.end();
			iterator++) {

		int* threshold_value = iterator->second;
		for (int j = 1, j_len = threshold_value[0] + 1; j < j_len; j++) {
			if (result > threshold_value[j]) {
				result = threshold_value[j];
			}
		}
	}
	free(array_m);
	delete[] binary;
	return result;
}

/**
 * Finds threshold for specified s, k, Q, i, j and M by
 * using calculated thresholds for lower string length.
 *
 * @param s span of shape array_q.
 * @param k number of differences.
 * @param array_q specified shape.
 * @param len_q length of shape.
 * @param array_m matches at last s-1 position.
 * @param len_m length of array_m.
 * @param i current string length.
 * @param j current number of differences.
 * @param thresholds current threshold values for all error values below or
 * 			equal to k, for all possible sets M describing the matches
 * 			in the last s-1 positions and for i<=m where i is current
 * 			string length.
 * @param binary array for converting values.
 * @return found threshold.
 */
int FindThreshold(int s, int k, int* array_q, int len_q, int* array_m,
		int len_m, int i, int j, unordered_map<string, int*>& thresholds,
		bool* binary) {

	if (!(j >= 0 && j <= k)) {
		return MAX_INT;
	}

	/*	check if all values in M are in range [1, s-1] */
	for (int c = 0; c < len_m; c++) {
		if (!(array_m[c] >= 1 && array_m[c] <= (s - 1))) {
			return MAX_INT;
		}
	}

	if (len_m < (s - 1 - j)) {
		return MAX_INT;
	}

	/*	generate next Ms */
	int* next_m_first = new int[len_m + 1];
	int* next_m_second = new int[len_m];
	int len_next_m_first = 1;
	int len_next_m_second = 0;
	next_m_first[0] = 1;
	for (int c = 0; c < len_m; c++) {
		if (array_m[c] == (s - 1))
			continue;
		next_m_first[c + 1] = array_m[c] + 1;
		len_next_m_first++;
		next_m_second[c] = array_m[c] + 1;
		len_next_m_second++;
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

	/*	calculate nextJ:
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

	int val = min(
			GetThresholdFor(s, k, next_j, next_m_first, len_next_m_first,
					thresholds, binary) + (is_subset ? 1 : 0),
			GetThresholdFor(s, k, next_j, next_m_second, len_next_m_second,
					thresholds, binary));
	free(next_m_first);
	free(next_m_second);
	return val;
}

/**
 * Retrieves threshold from currently stored threshold values
 * for given number of differences j and given set of matches
 * at last s-1 positions M.
 *
 * @param s span of shape.
 * @param k distance measure.
 * @param j current number of differences.
 * @param array_m matches at last s - 1 position.
 * @param len_m length of array_m.
 * @param thresholds current threshold values for all error values below or
 * 			equal to k, for all possible sets M describing the matches
 * 			in the last s-1 positions and for i<=m where i is current
 * 			string length.
 * @param binary array for converting values.
 * @return threshold for specified j and M.
 */
int GetThresholdFor(int s, int k, int j, int* array_m, int len_m,
		unordered_map<string, int*>& thresholds, bool* binary) {

	/*	check if j is out of bound */
	if (!(j >= 0 && j <= k)) {
		return MAX_INT;
	}

	/*	check if all values in M are in range [1, s-1] */
	for (int c = 0; c < len_m; c++) {
		if (!(array_m[c] >= 1 && array_m[c] <= (s - 1))) {
			return MAX_INT;
		}
	}

	/*	check if len_m is out of bound */
	if (len_m < (s - 1 - j)) {
		return MAX_INT;
	}

	ToBinary(array_m, len_m, binary, s - 1);
	int index_j = k - j + 1;
	string key = BinaryToString(binary, s - 1);
	int val = thresholds.find(key)->second[index_j];
	return val;
}

/**
 * Generates all possible shapes for specified s and q.
 *
 * @param s span of shape.
 * @param q size of shape.
 * @return generated shapes.
 */
vector<int*> GenerateShapes(int s, int q) {
	int start = 1;
	int end = s - 2;
	int len = end - start + 1;
	long long int count_to = pow(2, len);
	vector<int*> shapes;
	bool* array = new bool[len];
	for (long long int counter = 0; counter < count_to; counter++) {
		int ones_count = ToBinary(counter, array, len);
		if (ones_count == (q - 2)) {
			int* array_q = new int[q];
			FillFromBinary(array, start, end, array_q, 1);
			array_q[0] = 0;
			array_q[q - 1] = s - 1;
			shapes.push_back(array_q);
		}
	}
	return shapes;
}

/**
 * Checks whether two shapes are equal at defined positions.
 *
 * @param first_shape First shape.
 * @param second_shape Second shape.
 * @param start starting position.
 * @param end terminating position.
 * @return true if shapes are equal at defined positions,
 * 		false otherwise.
 */
bool ShapesEqual(int* first_shape, int* second_shape, int start, int end) {
	for (int i = start; i < end; i++) {
		if (first_shape[i] != second_shape[i]) {
			return false;
		}
	}
	return true;
}

/**
 * Generates (q,s)-shapes from (q-1,s)-shapes considering only those that gave
 * positive thresholds.
 *
 * @param s span of shape.
 * @param q size of shape.
 * @param k number of differences.
 * @param m string length.
 * @param thresholds current threshold values for all error values below or
 * 			equal to k, for all possible sets M describing the matches
 * 			in the last s-1 positions and for i<=m where i is current
 * 			string length.
 * @param copy copies of current threshold values.
 * @return generated shapes.
 */
vector<int*> GenerateShapesFromPrevious(int s, int q, int k, int m,
		unordered_map<string, int*>& thresholds,
		unordered_map<string, int*>& copy) {

	if (q < 4) {
		return GenerateShapes(s, q);
	}

	vector<int*> previous_shapes = GenerateShapesFromPrevious(s, q - 1, k, m,
			thresholds, copy);

	/*	Extract shapes that gave positive threshold. */

	vector<int*> positive_threshold_shapes;
	for (unsigned int i = 0, len_prev_shapes = previous_shapes.size();
			i < len_prev_shapes; i++) {
		for (auto iterator = thresholds.begin(); iterator != thresholds.end();
				iterator++) {
			int* copy_value = copy.find(iterator->first)->second;
			int* threshold_value = iterator->second;
			for (int j = 1, j_len = threshold_value[0] + 1; j < j_len; j++) {
				iterator->second[j] = 0;
				copy_value[j] = 0;
			}
		}

		int value = CalculateThresholdForShape(s, k, m, previous_shapes[i], q - 1,
				thresholds, copy);
		if (value > 0) {
			positive_threshold_shapes.push_back(previous_shapes[i]);
		}
	}

	/*	Construct groups of shapes that only differ in second to last
	 * position. */

	vector<vector<int*>> shape_groups;

	while (positive_threshold_shapes.size() > 0) {
		vector<int*> temp_group;
		temp_group.push_back(positive_threshold_shapes[0]);
		for (unsigned int j = positive_threshold_shapes.size() - 1; j > 0;
				j--) {
			if ((positive_threshold_shapes[0][q - 3]
					!= positive_threshold_shapes[j][q - 3])
					&& ShapesEqual(positive_threshold_shapes[0],
							positive_threshold_shapes[j], 1, q - 3)) {
				temp_group.push_back(positive_threshold_shapes[j]);
				positive_threshold_shapes.erase(
						positive_threshold_shapes.begin() + j);
			}
		}
		positive_threshold_shapes.erase(positive_threshold_shapes.begin());
		shape_groups.push_back(temp_group);
	}

	/*	From each group of (q-1,s)-shapes we generate new shape for all pairs in group by making union
	 * of two paired shapes. That will result in new (q,s)-shape since each pair in group only
	 * differs in one position thus adding one new element to union. */

	vector<int*> result;

	for (unsigned int i = 0, len_groups = shape_groups.size(); i < len_groups;
			i++) {
		for (unsigned int j = 0, len_shape = shape_groups[i].size();
				j < len_shape; j++) {

			int* base_shape = shape_groups[i][j];
			for (unsigned int k = j + 1, len_shape_groups =
					shape_groups[i].size(); k < len_shape_groups; k++) {

				int* temp = new int[s + 1];
				for (int e = 0, len = q - 3; e < len; e++) {
					temp[e] = base_shape[e];
				}
				if (base_shape[q - 3] > shape_groups[i][k][q - 3]) {
					temp[q - 3] = shape_groups[i][k][q - 3];
					temp[q - 2] = base_shape[q - 3];
				} else {
					temp[q - 3] = base_shape[q - 3];
					temp[q - 2] = shape_groups[i][k][q - 3];
				}
				temp[q - 1] = base_shape[q - 2];
				result.push_back(temp);
			}
		}
	}
	return result;
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
	int len_m = offset;
	for (int c = 0; c < len; c++) {
		if (binary[c] == 1) {
			array_m[len_m++] = start + c;
		}
	}
	return len_m;
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
 * Converts binary to string representation of zero and ones.
 *
 * @param binary array from which string is created.
 * @param size size of binary array.
 * @return string representation of specified binary.
 */
string BinaryToString(bool* binary, int size) {
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
void StringToBinary(string str, bool* binary, int size) {
	for (int i = 0; i < size; i++) {
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
long long int ToDecade(bool* binary, int size) {
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
 * @param array_m matches at last s - 1 position.
 * @param len_m length of array_m.
 * @param binary array to be filled.
 * @param size size of binary.
 *
 */
void ToBinary(int* array_m, int len_m, bool* binary, int size) {
	for (int i = 0; i < size; i++) {
		binary[i] = false;
	}
	for (int i = 0; i < len_m; i++) {
		binary[array_m[i] - 1] = true;
	}
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
	for (int s = q, len = 50 - k; s <= len; s++) {
		int value = CalculateThreshold(s, k, 50, q, &result);
		bool sat = array[s - q] == value;
		cout << "s: " << s << " threshold: " << value << " satisfied: " << sat;
		if (value > 0) {
			cout << " for shape: " << ShapeToString(result, q);
		}
		cout << endl;
	}
}
