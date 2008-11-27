/***********************************************************************
 cmdline.cpp - Utility functions for printing out data in common
    formats, required by most of the example programs.

 Copyright (c) 2007-2008 by Educational Technology Resources, Inc.
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

#include "cmdline.h"

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//// globals and constants /////////////////////////////////////////////

bool dtest_mode = false;		// true when running under dtest
int run_mode = 0;				// -m switch's value
const char* kpcSampleDatabase = "mysql_cpp_data";


//// att_getopt ////////////////////////////////////////////////////////
// An implementation of getopt(), included here so we don't have to
// limit ourselves to platforms that provide this natively.  It is
// adapted from the public domain getopt() implementation presented at
// the 1985 UNIFORUM conference in Dallas, Texas.  It's been reformatted
// and reworked a bit to fit in with MySQL++.

static const char* ag_optarg;
int ag_optind = 1;

static int
att_getopt(int argc, char* const argv[], const char* ag_opts)
{
	static int optopt;
	static int sp = 1;
	register int c;
	register const char *cp;

	if (sp == 1) {
		/* If all args are processed, finish */
		if (ag_optind >= argc) {
			return EOF;
		}
		if (argv[ag_optind][0] != '-' || argv[ag_optind][1] == '\0') {
			return EOF;
		}
	}
	else if (!strcmp(argv[ag_optind], "--")) {
		/* No more ag_options to be processed after this one */
		ag_optind++;
		return EOF;
	}

	optopt = c = argv[ag_optind][sp];

	/* Check for invalid ag_option */
	if (c == ':' || (cp = strchr(ag_opts, c)) == NULL) {
		fprintf(stderr, "%s: illegal option -- %c\n", argv[0], c);
		if (argv[ag_optind][++sp] == '\0') {
			ag_optind++;
			sp = 1;
		}

		return '?';
	}

	/* Does this ag_option require an argument? */
	if (*++cp == ':') {
		/* If so, get argument; if none provided output error */
		if (argv[ag_optind][sp + 1] != '\0') {
			ag_optarg = &argv[ag_optind++][sp + 1];
		}
		else if (++ag_optind >= argc) {
			fprintf(stderr,
					"%s: option requires an argument -- %c\n", argv[0], c);
			sp = 1;
			return '?';
		}
		else {
			ag_optarg = argv[ag_optind++];
		}
		sp = 1;
	}
	else {
		if (argv[ag_optind][++sp] == '\0') {
			sp = 1;
			ag_optind++;
		}
		ag_optarg = NULL;
	}

	return c;
}


//// print_usage ///////////////////////////////////////////////////////
// Show the program's usage message

void
print_usage(const char* program_name, const char* extra_parms)
{
	std::cout << "usage: " << program_name <<
			" [-s server_addr] [-u user] [-p password] " <<
			extra_parms << std::endl;
	std::cout << std::endl;
	std::cout << "    If no options are given, connects to database "
			"server on localhost" << std::endl;
	std::cout << "    using your user name and no password." << std::endl;
	if (strlen(extra_parms) > 0) {
		std::cout << std::endl;
		std::cout << "    The extra parameter " << extra_parms <<
				" is required, regardless of which" << std::endl;
		std::cout << "    other arguments you pass." << std::endl;
	}
	std::cout << std::endl;
}


//// parse_command_line ////////////////////////////////////////////////
// Wrapper around att_getopt() to return the parameters needed to
// connect to a database server and select the database itself.

bool
parse_command_line(int argc, char *argv[], const char** ppdb,
        const char** ppserver, const char** ppuser, const char** pppass,
        const char* extra_parms)
{
	if (argc < 1) {
		std::cerr << "Bad argument count: " << argc << '!' << std::endl;
		return false;
	}

	if (ppdb && !*ppdb) {
		*ppdb = "mysql_cpp_data";       // use default DB
	}

	int ch;
	while ((ch = att_getopt(argc, argv, "m:p:s:u:D")) != EOF) {
		switch (ch) {
			case 'm': run_mode = atoi(ag_optarg); break;
			case 'p': *pppass = ag_optarg;        break;
			case 's': *ppserver = ag_optarg;      break;
			case 'u': *ppuser = ag_optarg;        break;
			case 'D': dtest_mode = true;          break;
			default:
				print_usage(argv[0], extra_parms);
				return false;
		}
	}

	return true;
}
