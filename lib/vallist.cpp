#include "vallist.h"

#include "result.h"
#include "row.h"

using std::string;

namespace mysqlpp {

void
create_vector(int size, std::vector<bool> &v, bool t0, bool t1, bool t2,
		bool t3, bool t4, bool t5, bool t6, bool t7, bool t8, bool t9,
		bool ta, bool tb, bool tc)
{
	v.reserve(size);

	v.push_back(t0);
	if (size == 1) return;

	v.push_back(t1);
	if (size == 2) return;

	v.push_back(t2);
	if (size == 3) return;

	v.push_back(t3);
	if (size == 4) return;

	v.push_back(t4);
	if (size == 5) return;

	v.push_back(t5);
	if (size == 6) return;

	v.push_back(t6);
	if (size == 7) return;

	v.push_back(t7);
	if (size == 8) return;

	v.push_back(t8);
	if (size == 9) return;

	v.push_back(t9);
	if (size == 10) return;

	v.push_back(ta);
	if (size == 11) return;

	v.push_back(tb);
	if (size == 12) return;

	v.push_back(tc);
}

template<class Container>
void create_vector(const Container& c, std::vector<bool>& v, string s0,
		string s1, string s2, string s3, string s4, string s5,
		string s6, string s7, string s8, string s9, string sa,
		string sb, string sc)
{
	v.insert(v.begin(), c.size(), false);

	v[c.parent().field_num(s0)] = true;
	if (s1.empty()) return;

	v[c.parent().field_num(s1)] = true;
	if (s2.empty()) return;

	v[c.parent().field_num(s2)] = true;
	if (s3.empty()) return;

	v[c.parent().field_num(s3)] = true;
	if (s4.empty()) return;

	v[c.parent().field_num(s4)] = true;
	if (s5.empty()) return;

	v[c.parent().field_num(s5)] = true;
	if (s6.empty()) return;

	v[c.parent().field_num(s6)] = true;
	if (s7.empty()) return;

	v[c.parent().field_num(s7)] = true;
	if (s8.empty()) return;

	v[c.parent().field_num(s8)] = true;
	if (s9.empty()) return;

	v[c.parent().field_num(s9)] = true;
	if (sa.empty()) return;

	v[c.parent().field_num(sa)] = true;
	if (sb.empty()) return;

	v[c.parent().field_num(sb)] = true;
	if (sc.empty()) return;

	v[c.parent().field_num(sc)] = true;
}

template void
create_vector(const Row& c, std::vector<bool>& v, string s0,
		string s1, string s2, string s3, string s4, string s5,
		string s6, string s7, string s8, string s9, string sa,
		string sb, string sc);

};								// end namespace mysqlpp

