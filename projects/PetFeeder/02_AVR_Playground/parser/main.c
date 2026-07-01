/*
 * parser.c
 *
 * Created: 19.06.2026 14:53:51
 * Author : HP
 */ 

#include <avr/io.h>
#include <stdbool.h>

bool ParseNumber(const char *str, uint8_t *value)
{
	uint16_t result = 0;

	if (*str == '\0')
	{
		return false;
	}

	while (*str != '\0')
	{
		if ((*str < '0') || (*str > '9'))
		{
			return false;
		}

		uint8_t digit = *str - '0';

		result = result * 10 + digit;

		if (result > 255)
		{
			return false;
		}

		str++;
	}

	*value = (uint8_t)result;
	return true;
}

int main(void)
{
	uint8_t number = 0;
    bool checkNumber = ParseNumber("123", &number);
    while (1) 
    {
		number++;
    }
}

