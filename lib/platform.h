/// \file platform.h
/// \brief This file includes things that help the rest of MySQL++
//	remain unaware of platform differences.
///
/// This file includes all of the platform-specific definitions that
/// allow the rest of the code to be aware only of platform features,
/// rather than aware of specific platforms.  On Unixy systems, it
/// #includes the autoconf-generated header config.h, and on all other
/// platforms it includes the tests for platform features directly.

/***********************************************************************
 Copyright (c) 1998 by Kevin Atkinson, (c) 1999, 2000 and 2001 by
 MySQL AB, and (c) 2004, 2005 by Educational Technology Resources, Inc.
 Others may also hold copyrights on code in this file.  See the CREDITS
 file in the top directory of the distribution for details.

 This file is part of MySQL++.

 MySQL++ is free software; you can redistribute it and/or modify it
 under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 MySQL++ is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with MySQL++; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
 USA
***********************************************************************/

#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for the following stuff.

#if defined(__WIN32__) || defined(_WIN32)
#	define MYSQLPP_PLATFORM_WINDOWS

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

	// Define DLL import/export tags for Windows compilers, where we build
	// the library into a DLL, for LGPL license compatibility reasons.
	// (This is based on a similar mechanism in wxWindows.)

	#ifdef MYSQLPP_MAKING_DLL
		// When making the DLL, export tagged symbols, so they appear
		// in the import library.
		#define MYSQLPP_EXPORT __declspec(dllexport)
	#elif !defined(MYSQLPP_NO_DLL)
		// We must be _using_ the DLL, so import symbols instead.
		#define MYSQLPP_EXPORT __declspec(dllimport)
	#else
		// Not making a DLL at all, so no-op these declspecs
		#define MYSQLPP_EXPORT
	#endif
#else
	// If not Windows, we assume some sort of Unixy build environment,
	// where autotools is used.  (This includes Cygwin!)  #include the
	// config.h file only if this file was included from a non-header
	// file, because headers must not be dependent on config.h.
#	if defined(MYSQLPP_NOT_HEADER)
#		include "config.h"
#	endif

	// Make DLL stuff a no-op on this platform.
	#define MYSQLPP_EXPORT
#endif

#endif // !defined(DOXYGEN_IGNORE)
