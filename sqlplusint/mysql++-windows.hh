#if defined(__WIN32__) || defined(_WIN32)
#include <winsock.h>

// Disable whining about using 'this' as a member initializer on VC++.
#pragma warning(disable: 4355)

#define NO_LONG_LONGS
#endif

