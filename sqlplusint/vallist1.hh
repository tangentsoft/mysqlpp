#ifndef __vallist1_hh__
#define __vallist1_hh__

#include <string>
#include <vector>
#include "manip1.hh"

template <class Seq, class Manip>
struct value_list_b {
  const Seq          *list;
  const vector<bool> fields;
  const char         *delem;
  Manip              manip;
  value_list_b(const Seq &s, const vector<bool> &f, const char *d, Manip m) 
    : list(&s), fields(f), delem(d), manip(m) {}
};

template <class Seq1, class Seq2, class Manip>
struct equal_list_b {
  const Seq1         *list1;
  const Seq2         *list2;
  const vector<bool> fields;
  const char         *delem;
  const char         *equl;
  Manip              manip;
  equal_list_b(const Seq1 &s1, const Seq2 &s2, const vector<bool> &f,
	       const char *d, const char *e, Manip m) 
    : list1(&s1), list2(&s2), fields(f), delem(d), equl(e), manip(m) {}
};

template <class Seq, class Manip>
struct value_list_ba {
  const Seq          *list;
  const char         *delem;
  Manip              manip;
  value_list_ba(const Seq &s, const char* d, Manip m) 
    : list(&s), delem(d), manip(m) {}
};

template <class Seq1, class Seq2, class Manip>
struct equal_list_ba {
  const Seq1         *list1;
  const Seq2         *list2;
  const char         *delem;
  const char         *equl;
  Manip              manip;
  equal_list_ba(const Seq1 &s1, const Seq2 &s2, const char *d, const char *e,
		Manip m) 
    : list1(&s1), list2(&s2), delem(d), equl(e), manip(m) {}
};

//

template <class Seq, class Manip>
ostream& operator << (ostream &o, const value_list_b<Seq, Manip> &cl) {
  typename Seq::const_iterator i =  cl.list->begin();
  register int k = 0;
  for (;;) {
    if (cl.fields[k])
      o << cl.manip << *i;
    k++;
    if (++i == cl.list->end()) break;
    if (cl.fields[k])
      o << cl.delem;
  }
  return o;
}

template <class Seq1, class Seq2, class Manip>
ostream& operator << (ostream &o, const equal_list_b<Seq1, Seq2, Manip> &el) {
  typename Seq1::const_iterator i = el.list1->begin();
  typename Seq2::const_iterator j = el.list2->begin();
  register int k = 0;
  for (;;) {
    if (el.fields[k]) 
      o << *i << el.equl << el.manip << *j;
    k++;
    j++;
    if (++i == el.list1->end()) break;
    if (el.fields[k])
      o << el.delem;
  }
  return o;
}

template <class Seq, class Manip>
ostream& operator << (ostream &o, const value_list_ba<Seq, Manip> &cl) {
  typename Seq::const_iterator i =  cl.list->begin();
  for (;;) {
    o << cl.manip << *i;
    if (++i == cl.list->end()) break;
    o << cl.delem;
  }
  return o;
}

template <class Seq1, class Seq2, class Manip>
ostream& operator << (ostream &o, const equal_list_ba<Seq1, Seq2, Manip> &el) {
  typename Seq1::const_iterator i = el.list1->begin();
  typename Seq2::const_iterator j = el.list2->begin();
  for (;;) {
    o << *i << el.equl << el.manip << *j;
    j++;
    if (++i == el.list1->end()) break;
    o << el.delem;
  }
  return o;
}

template <class Iter>
class simp_list_b {
public:
  Iter _begin;
  Iter _end;
public:
  typedef Iter const_iterator;
  simp_list_b(Iter b, Iter e) : _begin(b), _end(e) {}
  Iter begin() const {return _begin;}
  Iter end() const {return _end;}
};

//
// create vector
//

void create_vector(int size, vector<bool>& v, bool t0, 
		   bool t1=false, bool t2=false, bool t3=false, bool t4=false,
		   bool t5=false, bool t6=false, bool t7=false, bool t8=false,
		   bool t9=false, bool ta=false, bool tb=false, bool tc=false);

template <class Container>
void create_vector(const Container&c, vector<bool>&v, string s0, string s1, 
		   string s2, string s3, string s4, string s5, string s6,
		   string s7, string s8, string s9, string sa, string sb,
		   string sc);

//
// value list
//

template <class Seq>  
inline
value_list_ba<Seq,do_nothing_type0> 
value_list(const Seq &s, const char *d = ",") {
  return value_list_ba<Seq ,do_nothing_type0> (s, d, do_nothing);
}

template <class Seq, class Manip>  
inline
value_list_ba<Seq,Manip> 
value_list(const Seq &s, const char *d, Manip m) {
  return value_list_ba<Seq,Manip> (s, d, m);
}

template <class Seq, class Manip>
inline
value_list_b<Seq,Manip>
value_list(const Seq &s, const char *d, Manip m, const vector<bool> &vb)
{
  return value_list_b<Seq, Manip>(s, vb, d, m);
}

template <class Seq, class Manip>
value_list_b<Seq,Manip>
value_list(const Seq &s, const char *d, Manip m, bool t0, 
	   bool t1=false, bool t2=false, bool t3=false, bool t4=false,
	   bool t5=false, bool t6=false, bool t7=false, bool t8=false,
	   bool t9=false, bool ta=false, bool tb=false, bool tc=false)
{
  vector<bool> vb;
  create_vector(s.size(), vb, t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,ta,tb,tc);
  return value_list_b<Seq, Manip>(s, vb, d, m);
}

template <class Seq>
value_list_b<Seq,do_nothing_type0>
value_list(const Seq &s, const char *d, bool t0, 
	   bool t1=false, bool t2=false, bool t3=false, bool t4=false,
	   bool t5=false, bool t6=false, bool t7=false, bool t8=false,
	   bool t9=false, bool ta=false, bool tb=false, bool tc=false)
{
  vector<bool> vb;
  create_vector(s.size(), vb, t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,ta,tb,tc);
  return value_list_b<Seq, do_nothing_type0> (s, vb, d, do_nothing);
}

template <class Seq>
value_list_b<Seq,do_nothing_type0>
value_list(const Seq &s, bool t0, 
	   bool t1=false, bool t2=false, bool t3=false, bool t4=false,
	   bool t5=false, bool t6=false, bool t7=false, bool t8=false,
	   bool t9=false, bool ta=false, bool tb=false, bool tc=false)
{
  vector<bool> vb;
  create_vector(s.size(), vb, t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,ta,tb,tc);
  return value_list_b<Seq, do_nothing_type0> (s, vb, ",",do_nothing);
}

//
// equal list
//

template <class Seq1, class Seq2>  
inline
equal_list_ba<Seq1,Seq2,do_nothing_type0> 
equal_list(const Seq1 &s1, const Seq2 &s2, const char *d = ",", 
	   const char *e = " = ") {
  return equal_list_ba<Seq1,Seq2,do_nothing_type0> (s1, s2, d, e, do_nothing);
}

template <class Seq1, class Seq2, class Manip>
inline
equal_list_ba<Seq1,Seq2,Manip> 
equal_list(const Seq1 &s1, const Seq2 &s2, 
	   const char *d, const char *e, Manip m) {
  return equal_list_ba<Seq1,Seq2,Manip> (s1, s2, d, e, m);
}

template <class Seq1, class Seq2, class Manip>
inline
equal_list_b<Seq1, Seq2, Manip>
equal_list(const Seq1 &s1, const Seq2 &s2, const char *d, const char *e, 
	   Manip m, const vector<bool> &vb)
{
  return equal_list_b<Seq1,Seq2,Manip>(s1, s2, vb, d, e, m);
}

// complete
template <class Seq1, class Seq2, class Manip>
equal_list_b<Seq1, Seq2, Manip>
equal_list(const Seq1 &s1, const Seq2 &s2, const char *d, const char *e, 
	   Manip m, bool t0, 
	   bool t1=false, bool t2=false, bool t3=false, bool t4=false,
	   bool t5=false, bool t6=false, bool t7=false, bool t8=false,
	   bool t9=false, bool ta=false, bool tb=false, bool tc=false)
{
  vector<bool> vb;
  create_vector(s1.size(), vb, t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,ta,tb,tc);
  return equal_list_b<Seq1,Seq2,Manip>(s1, s2, vb, d, e, m);
}

// delem, equl
template <class Seq1, class Seq2>
equal_list_b<Seq1, Seq2, do_nothing_type0>
equal_list(const Seq1 &s1, const Seq2 &s2, const char *d, const char *e, 
	   bool t0, 
	   bool t1=false, bool t2=false, bool t3=false, bool t4=false,
	   bool t5=false, bool t6=false, bool t7=false, bool t8=false,
	   bool t9=false, bool ta=false, bool tb=false, bool tc=false)
{
  vector<bool> vb;
  create_vector(s1.size(), vb, t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,ta,tb,tc);
  return equal_list_b<Seq1,Seq2,do_nothing_type0>
    (s1, s2, vb, d, e, do_nothing);
}

// delem
template <class Seq1, class Seq2>
equal_list_b<Seq1,Seq2,do_nothing_type0>
equal_list(const Seq1 &s1, const Seq2 &s2, const char *d, bool t0, 
	   bool t1=false, bool t2=false, bool t3=false, bool t4=false,
	   bool t5=false, bool t6=false, bool t7=false, bool t8=false,
	   bool t9=false, bool ta=false, bool tb=false, bool tc=false)
{
  vector<bool> vb;
  create_vector(s1.size(), vb, t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,ta,tb,tc);
  return equal_list_b<Seq1,Seq2,do_nothing_type0> 
    (s1, s2, vb, d, " = ", do_nothing);
}

// nothing
template <class Seq1, class Seq2>
equal_list_b<Seq1,Seq2,do_nothing_type0>
equal_list(const Seq1 &s1, const Seq2 &s2, bool t0, 
	   bool t1=false, bool t2=false, bool t3=false, bool t4=false,
	   bool t5=false, bool t6=false, bool t7=false, bool t8=false,
	   bool t9=false, bool ta=false, bool tb=false, bool tc=false)
{
  vector<bool> vb;
  create_vector(s1.size(), vb, t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,ta,tb,tc);
  return equal_list_b<Seq1,Seq2,do_nothing_type0> 
    (s1, s2, vb,"," ," = " ,do_nothing);
}

#endif

