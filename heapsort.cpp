#include <iostream>
using namespace std;

void printTable(int *table, unsigned long size){
	for(unsigned long i = 0; i < size; i++){
		cout << table[i]  << " ";
	}
	cout << endl;
}

void swapEls(int* table, unsigned long idx1, unsigned long idx2){
	int tmp = 0;
	tmp = table[idx1];
	table[idx1] = table[idx2];
	table[idx2] = tmp;
}


void sortTree(int *table, unsigned long size){
	for(unsigned long i = size-1; i > 0; i--){
		if(table[i] > table[(i-1)/2])
			swapEls(table, i, (i-1)/2);
	}
}
// This version works longer, but the output will be the same as given in assignment
/*

void sortTree(int *table, unsigned long size){
	for(unsigned long i = 1; i < size; i++){
		unsigned long iParent = (i-1)/2;
		if(table[i] > table[iParent]){
			swapEls(table, i, iParent);
			unsigned long j = iParent;
			while(j){
				unsigned long jParent = (j-1)/2;
				if(table[j] > table[jParent]){
					swapEls(table, j, jParent);
					j = jParent;
				} else {
					j = 0;
				}
			}
		}
	}
}
*/

int main(){
	unsigned long casesNum;
	cin >> casesNum;
	for(unsigned long i = 0; i < casesNum; i++){
		unsigned long arraySize;
		cin >> arraySize;
		// Read heap
		int *table = new int[arraySize];
		int currentEl;
		for(unsigned long i = 0; i < arraySize; i++){
			cin >> currentEl;
			table[i] = currentEl;
		}
		// Sort heap
		for( unsigned long i = arraySize; i > 0; i--){
			sortTree(table, i);
			if(i > 1){
				printTable(table, i);
				swapEls(table, 0, i-1);
			}
		}
		printTable(table, arraySize);
		cout << endl;

		delete [] table;
	}

	return 0;
}
