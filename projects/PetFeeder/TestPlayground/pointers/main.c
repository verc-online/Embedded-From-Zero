/*
 * pointers.c
 *
 * Created: 18.06.2026 15:46:17
 * Author : HP
 */ 

#include <stdint.h>

void Swap(int *a, int *b)
{
	int temp;
	temp = *b;
	*b = *a;
	*a = temp;
}

int FindMax(const int *array, uint8_t size)
{
	int maxValue = *array;
	for (uint8_t i = 0; i < size; i++)
	{
		maxValue = array[i] > maxValue ? array[i] : maxValue;
		
		// либо можно 
		// int maxValue = *(array + i) > maxValue ? array[i] : maxValue;
	}
	return maxValue;
}

void MemCopy(uint8_t *dst,
			const uint8_t *src,
			uint16_t size)
{
	for (uint8_t i = 0; i < size; i++)
	{
		dst[i] = src[i];
		// *(dst + i) = *(src + i);
	}
	// 	while (size > 0)
	// 	{
	// 		*dst = *src;
	// 
	// 		dst++;
	// 		src++;
	// 
	// 		size--;
	// 	}
}

int main(void)
{
	int x = 10;
	int y = 20;

	Swap(&x, &y);
	int values[] = {4, 7, 1, 15, 3};

	FindMax(values, 5);
	
	uint8_t source[] = {1, 2, 3, 4, 5};
	uint8_t destination[5];

	MemCopy(destination, source, 5);
	
	while (1)
	{

	}
}