#include "string_util.h"

namespace mysqlpp {

void strip(std::string& s)
{
	size_t i, j = s.size() - 1;
	if (!s.size()) {
		return;
	}
	for (i = 0; s[i] == ' '; i++) ;
	if (i) {
		s.erase(0, i);
	}
	j = s.size();
	if (!j) {
		return;
	}
	j--;
	for (i = j; i && s[i] == ' '; i--) ;
	if (i != j) {
		s.erase(i + 1, (size_t) - 1);
	}
}

void escape_string(std::string & s)
{
	if (!s.size()) {
		return;
	}

	for (unsigned int i = 0; i < s.size(); i++) {
		switch (s[i]) {
			case '\0':		// Must be escaped for "mysql"
				s[i] = '\\';
				s.insert(i, "0", 1);
				i++;
				break;
			case '\n':		// Must be escaped for logs
				s[i] = '\\';
				s.insert(i, "n", 1);
				i++;
				break;
			case '\r':
				s[i] = '\\';
				s.insert(i, "r", 1);
				i++;
				break;
			case '\\':
				s[i] = '\\';
				s.insert(i, "\\", 1);
				i++;
				break;
			case '\"':
				s[i] = '\\';
				s.insert(i, "\"", 1);
				i++;
				break;
			case '\'':		// Better safe than sorry
				s[i] = '\\';
				s.insert(i, "\'", 1);
				i++;
				break;
			case '\032':	// This gives problems on Win32
				s[i] = '\\';
				s.insert(i, "Z", 1);
				i++;
				break;
			default:
				break;
		}
	}
}

} // end namespace mysqlpp

