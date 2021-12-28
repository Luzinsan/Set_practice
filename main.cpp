#include <iostream>
#include "Set.h"
#define SIZE_SET 15

int main()
{
	int* array1 = (int*)malloc(SIZE_SET * sizeof(int));
	for (int i = 0; i < SIZE_SET; i++)
		array1[i] = rand()%10;
	Set<int> set1(SIZE_SET, array1);
	std::cout << set1;

	int *array2 = new int[] {-1,-5,4,6,4,6,6,3,12,9,4,5,1,0,5};
	Set<int> set2(SIZE_SET, array2);
	std::cout << set2;

	Set<int> set3(set1 * set2);
	std::cout << set3;
	std::cout << set1 + set2;
	return 0;
}
