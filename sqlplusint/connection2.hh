#ifndef __connection2_hh__
#define __connection2_hh__

#include "connection1.hh"
#include "query2.hh"

inline Query Connection::query() {return Query(this, throw_exceptions);}

template <class Sequence>
void Connection::storein_sequence (Sequence &seq, const string &str) {
  ResUse result = use(str);
  Row row;
  while (row = result.fetch_row()) {
    seq.push_back(typename Sequence::value_type(row));
  }
}

template <class Set>
void Connection::storein_set (Set &sett, const string &str) {
  ResUse result = use(str);
  Row row;
  while (row = result.fetch_row()) {
    sett.insert(typename Set::value_type(row));
  }
}


#endif
