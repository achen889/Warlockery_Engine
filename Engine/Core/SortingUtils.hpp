//==============================================================================================================
//SortingUtils.hpp
//by Albert Chen Jul-16-2016.
//==============================================================================================================

#pragma once

#ifndef _included_SortingUtils__
#define _included_SortingUtils__

#include <vector>
#include "Performance.hpp"

//===========================================================================================================

//merge sort and quick sort for any vector of type T where T is an object with <= and = operators

///----------------------------------------------------------------------------------------------------------
///merge sort

template<typename T>
void MergeSort(std::vector<T>& A) {
	PROFILE_SECTION(); //avg of 0.2 ms per call

	int length = A.size();

	int halfLength = (int)((float)length * 0.5f);

	if (!(length > 1)) {
		return;
	}

	std::vector<T> B;
	std::vector<T> C;
	B.reserve(halfLength);
	C.reserve(halfLength);

	int i = 0;
	for (std::vector<T>::iterator it = A.begin(); it != A.end(); ++it) {
		T& value = (*it);

		if (i < halfLength) B.push_back(value);
		else C.push_back(value);

		//inc i
		i++;
	}

	MergeSort(B);
	MergeSort(C);

	//merge B and C back into A
	Merge(B, C, A);

}

//-----------------------------------------------------------------------------------------------------------

template<typename T>
void Merge(std::vector<T>&B, std::vector<T>&C, std::vector<T>&A) {
	//PROFILE_SECTION(); //avg 0.017 ms per call

	int i = 0;
	int j = 0;
	int k = 0;

	int lengthB = B.size();
	int lengthC = C.size();
	//std::vector<T>:iterator it = A.begin();

	//push one or other into A
	while (i < lengthB && j < lengthC) {
		if (B[i] <= C[j]) {
			A[k] = B[i];
			i++;
		}
		else {
			A[k] = C[j];
			j++;
		}
		k++;
	}

	//fill up the rest
	if (i == lengthB) {
		for (j; j < lengthC; j++) {
			A[k] = C[j];
			k++;
		}
	}
	else {
		for (i; i < lengthB; i++) {
			A[k] = B[i];
			k++;
		}
	}

}

//===========================================================================================================

template<typename T>
void QuickSort(std::vector<T>& A) { 
	QuickSort(A, 0, A.size() - 1);
}

//-----------------------------------------------------------------------------------------------------------

template<typename T>
void QuickSort(std::vector<T>& A, int first, int last) {
	PROFILE_SECTION();
	if (first < last) {
		int pivotIndex = Partition(A, first, last);
		QuickSort(A, first, pivotIndex - 1);
		QuickSort(A, pivotIndex + 1, last);
	}
}

//-----------------------------------------------------------------------------------------------------------

template<typename T>
void Swap(T& a, T& b) {
	T temp = a;
	a = b;
	b = temp;
}

//-----------------------------------------------------------------------------------------------------------

//partitions A, from first to last and returns pivot index
template<typename T>
int Partition(std::vector<T>& A, int first, int last) {
	PROFILE_SECTION();
	int  pivotIndex = first;
	T pivot = A[first];

	for (int i = first + 1; i <= last; i++) {
		/* If you want to sort the list in the other order, change "<=" to ">" */
		if (A[i] <= pivot) {
			pivotIndex++;

			Swap(A[i], A[pivotIndex]);
		}
	}

	Swap(A[pivotIndex], A[first]);

	return pivotIndex;
}

//===========================================================================================================

#endif //__includedSortingUtils__

