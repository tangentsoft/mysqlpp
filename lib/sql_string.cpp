#include "sql_string.h"

#include <sstream>

using namespace std;

namespace mysqlpp {

SQLString::SQLString() :
is_string(false),
dont_escape(false),
processed(false)
{
}

SQLString::SQLString(const string& str) :
string(str),
is_string(true),
dont_escape(false),
processed(false)
{
}

SQLString::SQLString(const char* str) : 
string(str),
is_string(true),
dont_escape(false),
processed(false)
{
}

SQLString::SQLString(char i) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << static_cast<short int>(i);
	assign(outs.str());
}

SQLString::SQLString(unsigned char i) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << static_cast<unsigned short int>(i);
	assign(outs.str());
}

SQLString::SQLString(short int i) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << i;
	assign(outs.str());
}

SQLString::SQLString(unsigned short int i) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << i;
	assign(outs.str());
}

SQLString::SQLString(int i) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << i;
	assign(outs.str());
}

SQLString::SQLString(unsigned int i) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << i;
	assign(outs.str());
}

SQLString::SQLString(longlong i) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << i;
	assign(outs.str());
}

SQLString::SQLString(ulonglong i) :
is_string(false),
dont_escape(false),
processed(false) 
{
	ostringstream outs;
	outs << i;
	assign(outs.str());
}

SQLString::SQLString(float f) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << f;
	assign(outs.str());
}

SQLString::SQLString(double f) :
is_string(false),
dont_escape(false),
processed(false)
{
	ostringstream outs;
	outs << f;
	assign(outs.str());
}

} // end namespace mysqlpp

