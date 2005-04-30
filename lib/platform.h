/// \file platform.h
/// \brief This file includes things that help the rest of MySQL++
//	remain unaware of platform differences.
///
/// This file includes all of the platform-specific definitions that
/// allow the rest of the code to be aware only of platform features,
/// rather than aware of specific platforms.  On Unixy systems, it
/// #includes the autoconf-generated header config.h, and on all other
/// platforms it includes the tests for platform features directly.

#if defined(__WIN32__) || defined(_WIN32)
	// Windows compiler support.  Tested with Microsoft Visual C++,
	// Borland C++ Builder, and MinGW GCC.
#	include <winsock.h>

	// The shutdown_level argument was added in MySQL 4.1.3 and in 5.0.1.
#	if ((MYSQL_VERSION_ID > 40103) && (MYSQL_VERSION_ID < 49999)) || (MYSQL_VERSION_ID > 50001)
#		define HAVE_MYSQL_SHUTDOWN_LEVEL_ARG
#	endif

	// Stuff for Visual C++ only
#	if defined(_MSC_VER)
		// Disable whining about using 'this' as a member initializer on VC++.
#		pragma warning(disable: 4355)
		// Disable whining about implicit conversions to bool
#		pragma warning(disable: 4800)
		// Call _snprintf() for VC++ version of snprintf() function
#		define snprintf _snprintf
#	endif
#else
	// If not Windows, we assume some sort of Unixy build environment,
	// where autotools is used.  (This includes Cygwin!)  #include the
	// config.h file only if this file was included from a non-header
	// file, because headers must not be dependent on config.h.
#	if defined(MYSQLPP_NOT_HEADER)
#		include "config.h"
#	endif
#endif

// Define DLL import/export tags for Windows compilers, where we build
// the library into a DLL, for LGPL license compatibility reasons.
// (This is based on a similar mechanism in wxWindows, which inspired
// this code, but which does things quite differently.)
#if defined(__WIN32__) || defined(_WIN32)
	#ifdef MYSQLPP_MAKING_DLL
		// When making the DLL, export tagged symbols, so they appear
		// in the import library.
		#define MYSQLPP_DLLEXPORT __declspec(dllexport)
		#define MYSQLPP_DLLEXPORT_DATA(type) __declspec(dllexport) type
	#else
		// We must be using the DLL, so import symbols instead.
		#define MYSQLPP_DLLEXPORT __declspec(dllimport)
		#define MYSQLPP_DLLEXPORT_DATA(type) __declspec(dllimport) type
	#endif
#else
	// Not on a platform where DLL is made, so make everything a no-op.
	#define MYSQLPP_DLLEXPORT
	#define MYSQLPP_DLLEXPORT_DATA(type) type
#endif

