#ifndef __connection2_hh__
#define __connection2_hh__

#include "connection1.hh"
#include "query2.hh"

inline Query Connection::query() {return Query(this, throw_exceptions);}

template <class Sequence>
void Connection::storein_sequence (Sequence &seq, const string &str) {
  ResUse result = use(str);
  while (1) {
	  MYSQL_ROW d = mysql_fetch_row(result.mysql_res);
		if (!d) break;
	  Row row(d,&result,(unsigned int *)mysql_fetch_lengths(result.mysql_res),true);
		if (!row) break;
    seq.push_back(typename Sequence::value_type(row));
	}
}

template <class Set>
void Connection::storein_set (Set &sett, const string &str) {
  ResUse result = use(str);
	while (1) {
	  MYSQL_ROW d = mysql_fetch_row(result.mysql_res);	
		if (!d) return;
	  Row row(d,&result,(unsigned int *)mysql_fetch_lengths(result.mysql_res),true);
		if (!row) break;
    sett.insert(typename Set::value_type(row));
	}
}


#endif
