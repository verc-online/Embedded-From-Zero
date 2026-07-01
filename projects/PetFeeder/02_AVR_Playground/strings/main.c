/*
 * strings.c
 *
 * Created: 18.06.2026 20:32:31
 * Author : HP
 */ 

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

uint16_t StringLength(const char *str)
{
	uint16_t length = 0;

	while (*str != '\0')
	{
		length++;
		str++;
	}

	return length;
}

bool StringCompare(const char *a,
					const char *b)
{
	while ((*a == *b) &&
	(*a != '\0') &&
	(*b != '\0'))
	{
		a++;
		b++;
	}

	return (*a == *b);
}

void StringCopy(char *dst,
				const char *src)
{
	while (*src != '\0')
	{
		*dst = *src;

		dst++;
		src++;
	}

	*dst = '\0';
}

int main(void)
{
	char text[] = "Hello";
	volatile uint8_t length = StringLength(text);
	
	char dst[6];
	StringCopy(dst, text);
	volatile bool compare = StringCompare(dst, text);
    while (1) 
    {
		length++;
    }
}

