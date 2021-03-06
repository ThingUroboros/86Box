/* Better random number generator by Battler. */

#include <assert.h>
#include <stdio.h>

#include <stdint.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "disc_random.h"

uint32_t preconst = 0x6ED9EBA1;

static __inline__ uint32_t rotl32c (uint32_t x, uint32_t n)
{
  assert (n<32);
  return (x<<n) | (x>>(-n&31));
}

static __inline__ uint32_t rotr32c (uint32_t x, uint32_t n)
{
  assert (n<32);
  return (x>>n) | (x<<(-n&31));
}

#define ROTATE_LEFT rotl32c

#define ROTATE_RIGHT rotr32c

static __inline__ unsigned long long rdtsc(void)
{
    unsigned hi, lo;
#ifdef __MSC__
    __asm {
		rdtsc
		mov hi, edx	; EDX:EAX is already standard return!!
		mov lo, eax
    }
#else
      __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
#endif
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

static uint32_t RDTSC(void)
{
	return (uint32_t) (rdtsc());
}

static void disc_random_twist(uint32_t *val)
{
	*val = ROTATE_LEFT(*val, rand() % 32);
	*val ^= 0x5A827999;
	*val = ROTATE_RIGHT(*val, rand() % 32);
	*val ^= 0x4ED32706;
}

uint8_t disc_random_generate()
{
	uint16_t r = 0;
	r = (rand() ^ ROTATE_LEFT(preconst, rand() % 32)) % 256;
	disc_random_twist(&preconst);
	return (r & 0xff);
}

void disc_random_init()
{
	uint32_t seed = RDTSC();
	srand(seed);
	return;
}
