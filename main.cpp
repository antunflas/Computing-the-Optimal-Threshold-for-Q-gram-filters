#include <iostream>

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
