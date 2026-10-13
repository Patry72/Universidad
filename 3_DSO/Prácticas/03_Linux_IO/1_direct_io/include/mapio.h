/*
 * File: mapio.h
 * Author: Sergio Romero Montiel
 * Date: 2018-04-23
 *
 */

/* Prototipe of funtion mmap_io
 *
 * Rationale: maps a physical address provided as unsigned int
 *   to a virtual address in the process page table (one entry).
 *
 * Return value: the virtual address that translates to the
 *   physical. It tries to assign virtual = physical when possible
 *
 * Sample use:
 * volatile unsigned *gpio;
 * gpio = (volatile unsigned *)mmap(0x3F200000);
 * gpio[0] &= 00777777777; // octal numbers
 * gpio[0] |= 01000000000; // set gpio 9 -> output
 * gpio[7]  = 0x200;       // set   gpio 9 -> 1
 * gpio[10] = 0x200;       // clear gpio 9 -> 0
 */

void* mmap_io(unsigned int);
