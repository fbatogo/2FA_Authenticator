#ifndef SHAIMPLCOMMON_H
#define SHAIMPLCOMMON_H

#ifdef _WIN32
// Windows doesn't have endian.h, so assume we are on a little endian system. (Since we
// probably are.)
#define __LITTLE_ENDIAN         1

#define __BYTE_ORDER  __LITTLE_ENDIAN
#else
// On non-Windows systems, include endian.h.
#include <endian.h>
#endif // _WIN32

/* Operators defined in FIPS 180-2:4.1.2.  */
#define Ch(x, y, z) ((x & y) ^ (~x & z))
#define Maj(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define S0(x) (CYCLIC (x, 2) ^ CYCLIC (x, 13) ^ CYCLIC (x, 22))
#define S1(x) (CYCLIC (x, 6) ^ CYCLIC (x, 11) ^ CYCLIC (x, 25))
#define R0(x) (CYCLIC (x, 7) ^ CYCLIC (x, 18) ^ (x >> 3))
#define R1(x) (CYCLIC (x, 17) ^ CYCLIC (x, 19) ^ (x >> 10))

/* It is unfortunate that C does not provide an operator for
cyclic rotation.  Hope the C compiler is smart enough.  */
#define CYCLIC(w, s) ((w >> s) | (w << (32 - s)))

#endif // SHAIMPLCOMMON_H
