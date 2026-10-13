
// FUNCIÓN PARA CAMBIAR BITS
// uint8_t equivale a unsigned char

#include <avr/io.h>

unint_8_t changeBits (uint8_t input, uint8_t mask, uint8_t value)
{
	input &= ~mask;
	input |= value & mask;
	return input;
}