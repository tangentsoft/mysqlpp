////////////////////////////////////////////////////////////////////////
// platform.h - This file includes things that help the rest of MySQL++
//		remain unaware of platform differences.
////////////////////////////////////////////////////////////////////////

#if defined(__WIN32__) || defined(_WIN32)
// Windows compiler support: one of the WIN32 macros is defined by
// Microsoft Visual C++, and the other by Borland C++ Builder.
#	include <winsock.h>

	// Disable whining about using 'this' as a member initializer on VC++.
#	pragma warning(disable: 4355)

	// Windows compilers support 64-bit integers in a different way than
	// GCC, so disable 'long long' stuff for these systems.
	// Eventually, this issue needs to be avoided entirely, by using a
	// platform-independent 64-bit integer representation.
#	define NO_LONG_LONGS

	// Uncomment this if you are using MySQL 4.1.3 or higher, or 5.0.1
	// or higher.
//#	define HAVE_MYSQL_SHUTDOWN_LEVEL_ARG
#else
	// If not Windows, we assume some sort of Unixy build environment,
	// where autotools is used.  #include the config.h file only if this
	// file was included from a non-header file, because headers must
	// not be dependent on config.h.
#	if defined(MYSQLPP_NOT_HEADER)
#		include "config.h"
#	endif
#endif

