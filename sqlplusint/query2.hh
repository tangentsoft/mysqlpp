
#ifndef __query2_hh__
#define __query2_hh__

#include "query1.hh"
#include "connection2.hh"
#include "coldata2.hh"

inline bool Query::success() {
  if (!Success) return false;
  return mysql->success();
}

inline ResNSel Query::execute(const char* str) {
  return mysql->execute(str);
}

inline ResNSel Query::execute(parms &p, query_reset r)
{
  r = (parsed.size()) ? DONT_RESET : RESET_QUERY;
  return mysql->execute(str(p,r));
}

inline ResUse Query::use(const char* str) {
  return mysql->use(str);
}

inline ResUse Query::use(parms &p, query_reset r) {
  r = (parsed.size()) ? DONT_RESET: RESET_QUERY;
  return mysql->use(str(p,r));
}

inline Result Query::store(const char* str) {
  return mysql->store(str);
}

inline Result Query::store(parms &p, query_reset r) {
  r = (parsed.size()) ? DONT_RESET: RESET_QUERY;
  return mysql->store(str(p,r));
}

inline my_ulonglong Query::affected_rows() const {
  return mysql->affected_rows();
}

inline my_ulonglong Query::insert_id () {
  return mysql->insert_id();
}

inline std::string Query::info() {
  return mysql->info();
}

inline std::string Query::error () {
  if (errmsg) return std::string(errmsg);
  return mysql->error();
}

inline void Query::unlock() {
  mysql->unlock();
}

template <class Seq>
inline void Query::storein_sequence (Seq &seq,parms &p, query_reset r) {
  r = (parsed.size()) ? DONT_RESET : RESET_QUERY;
  mysql->storein_sequence (seq, str(p,r));
}

template <class Set>
inline void Query::storein_set (Set &sett, parms &p, query_reset r) {
  r = (parsed.size()) ? DONT_RESET : RESET_QUERY;
  mysql->storein_set (sett, str(p,r));
}

template <class Sequence>
inline void Query::storein_sequence (Sequence &seq, const char *s) {
  mysql->storein_sequence (seq, s);
}

template <class Set>
inline void Query::storein_set (Set &sett, const char * s) {
  mysql->storein_set (sett, s);
}

template <class T>
inline void Query::storein (T &con, parms &p, query_reset r) {
  r = (parsed.size()) ? DONT_RESET : RESET_QUERY;
  mysql->storein (con, str(p,r));
}

template <class T>
inline void Query::storein (T &con, const char *s) {
  mysql->storein (con, s);
}

#endif




