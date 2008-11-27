/***********************************************************************
 cmdline.h - Declares the interface to the command line parsing logic.
	It is not important for a new MySQL++ user to understand any of what
	goes on inside this module.  The examples just pass in the command 
	line arguments and magically get out the proper database server
	login parameters and the name of the database itself.

 Copyright (c) 2007 by Educational Technology Resources, Inc.
 Others may also hold copyrights on code in this file.  See the
 CREDITS.txt file in the top directory of the distribution for details.

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

#if !defined(MYSQLPP_CMDLINE_H)
#define MYSQLPP_CMDLINE_H

extern bool parse_command_line(int argc, char *argv[],
		const char** ppdb, const char** ppserver, const char** ppuser,
		const char** pppass, const char* extra_parms = "");
extern void print_usage(const char* program_name,
		const char* extra_parms = "");

#endif // !defined(MYSQLPP_CMDLINE_H)

