/***********************************************************************
 cpool.cpp - ConnectionPool example.  Works with both Windows native
	threads and POSIX threads.  Shows how to create and use a concrete
	ConnectionPool derivative.

 Copyright (c) 2008 by Educational Technology Resources, Inc.
 Others may also hold copyrights on code in this file.  See the
 CREDITS file in the top directory of the distribution for details.

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
#include "threads.h"

#include <iostream>

using namespace std;


// Define a concrete ConnectionPool derivative.  Takes connection
// parameters as inputs to its ctor, which it uses to create the
// connections we're called upon to make.  Note that we also declare
// a global pointer to an object of this type, which we create soon
// after startup; this should be a common usage pattern, as what use
// are multiple pools?
class SimpleConnectionPool : public mysqlpp::ConnectionPool
{
public:
	// The object's only constructor
	SimpleConnectionPool(const char* db, const char* server,
			const char* user, const char* password) :
	db_(db ? db : ""),
	server_(server ? server : ""),
	user_(user ? user : ""),
	password_(password ? password : "")
	{
	}

	// The destructor.  We _must_ call ConnectionPool::clear() here,
	// because our superclass can't do it for us.
	~SimpleConnectionPool()
	{
		clear();
	}

protected:
	// Superclass overrides
	mysqlpp::Connection* create()
	{
		// Create connection using the parameters we were passed upon
		// creation.  This could be something much more complex, but for
		// the purposes of the example, this suffices.
		cout.put('C'); cout.flush(); // indicate connection creation
		return new mysqlpp::Connection(
				db_.empty() ? 0 : db_.c_str(),
				server_.empty() ? 0 : server_.c_str(),
				user_.empty() ? 0 : user_.c_str(),
				password_.empty() ? "" : password_.c_str());
	}

	void destroy(mysqlpp::Connection* cp)
	{
		// Our superclass can't know how we created the Connection, so
		// it delegates destruction to us, to be safe.
		cout.put('D'); cout.flush(); // indicate connection destruction
		delete cp;
	}

	unsigned int max_idle_time()
	{
		// Set our idle time at an example-friendly 3 seconds.  A real
		// pool would return some fraction of the server's connection
		// idle timeout instead.
		return 3;
	}

private:
	// Our connection parameters
	std::string db_, server_, user_, password_;
};
SimpleConnectionPool* poolptr = 0;


#if defined(HAVE_THREADS)
static thread_return_t CALLBACK_SPECIFIER
worker_thread(thread_arg_t running_flag)
{
	// Ask the underlying C API to allocate any per-thread resources it
	// needs, in case it hasn't happened already.  In this particular
	// program, it's almost guaranteed that the grab() call below will
	// create a new connection the first time through, and thus allocate
	// these resources implicitly, but there's a nonzero chance that
	// this won't happen.  Anyway, this is an example program, meant to
	// show good style, so we take the high road and ensure the
	// resources are allocated before we do any queries.
	mysqlpp::Connection::thread_start();

	// Pull data from the sample table a bunch of times, releasing the
	// connection we use each time.
	for (size_t i = 0; i < 6; ++i) {
		// Go get a free connection from the pool, or create a new one
		// if there are no free conns yet.
		mysqlpp::Connection* cp = poolptr->grab();
		if (!cp) {
			cerr << "Failed to get a connection from the pool!" << endl;
			break;
		}

		// Pull a copy of the sample stock table and print a dot for
		// each row in the result set.
		mysqlpp::Query query(cp->query("select * from stock"));
		mysqlpp::StoreQueryResult res = query.store();
		for (size_t j = 0; j < res.num_rows(); ++j) {
			cout.put('.');
		}

		// Immediately release the connection once we're done using it.
		// If we don't, the pool can't detect idle connections reliably.
		poolptr->release(cp);

		// Delay 1-4 seconds before doing it again.  Because this can
		// delay longer than the idle timeout, we'll occasionally force
		// the creation of a new connection on the next loop.
		sleep(rand() % 4 + 1);	
	}

	// Tell main() that this thread is no longer running
	*reinterpret_cast<bool*>(running_flag) = false;
	
	// Release the per-thread resources before we exit
	mysqlpp::Connection::thread_end();

	return 0;
}
#endif


int
main(int argc, char *argv[])
{
#if defined(HAVE_THREADS)
	// Get database access parameters from command line
    const char* db = 0, *server = 0, *user = 0, *pass = "";
	if (!parse_command_line(argc, argv, &db, &server, &user, &pass)) {
		return 1;
	}

	// Create the pool and grab a connection.  We do it partly to test
	// that the parameters are good before we start doing real work, and
	// partly because we need a Connection object to call thread_aware()
	// on to check that it's okay to start doing that real work.  This
	// latter check should never fail on Windows, but will fail on most
	// other systems unless you take positive steps to build with thread
	// awareness turned on.  See README-*.txt for your platform.
	poolptr = new SimpleConnectionPool(db, server, user, pass);
	try {
		mysqlpp::Connection* cp = poolptr->grab();
		if (!cp->thread_aware()) {
			cerr << "MySQL++ wasn't built with thread awareness!  " <<
					argv[0] << " can't run without it." << endl;
			return 1;
		}
		poolptr->release(cp);
	}
	catch (mysqlpp::Exception& e) {
		cerr << "Failed to set up initial pooled connection: " <<
				e.what() << endl;
		return 1;
	}

	// Setup complete.  Now let's spin some threads...
	cout << endl << "Pool created and working correctly.  Now to do "
			"some real work..." << endl;
	srand(time(0));
	bool running[] = {
			true, true, true, true, true, true, true,
			true, true, true, true, true, true, true };
	const size_t num_threads = sizeof(running) / sizeof(running[0]);
	size_t i;
	for (i = 0; i < num_threads; ++i) {
		if (int err = create_thread(worker_thread, running + i)) {
			cerr << "Failed to create thread " << i <<
					": error code " << err << endl;
			return 1;
		}
	}

	// Test the 'running' flags every second until we find that they're
	// all turned off, indicating that all threads are stopped.
	cout.put('W'); cout.flush(); // indicate waiting for completion
	do {
		sleep(1);
		i = 0;
		while (i < num_threads && !running[i]) ++i;
	}
	while (i < num_threads);
	cout << endl << "All threads stopped!" << endl;

	// Shut it all down...
	delete poolptr;
	cout << endl;
#else
	(void)argc;		// warning squisher
	cout << argv[0] << " requires that threads be enabled!" << endl;
#endif

	return 0;
}
