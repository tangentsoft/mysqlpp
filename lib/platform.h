////////////////////////////////////////////////////////////////////////
// platform.h - This file includes things that help the rest of MySQL++
//		remain unaware of platform differences.
////////////////////////////////////////////////////////////////////////

#if defined(__WIN32__) || defined(_WIN32)
	// Windows compiler support.  Tested with Microsoft Visual C++,
	// Borland C++ Builder, and MinGW GCC.
#	include <winsock.h>

	// Uncomment this if you are using MySQL 4.1.3 or higher, or 5.0.1
	// or higher.
//#	define HAVE_MYSQL_SHUTDOWN_LEVEL_ARG

	// Stuff for Visual C++ only
#	if defined(_MSC_VER)
		// Disable whining about using 'this' as a member initializer on VC++.
#		pragma warning(disable: 4355)
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

