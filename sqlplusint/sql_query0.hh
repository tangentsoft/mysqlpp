#ifndef __sql_query0_hh__
#define __sql_query0_hh__

#define mysql_query_define0(RETURN, FUNC)\
  RETURN FUNC (ss a)\
    {return FUNC (parms() << a);}\
  RETURN FUNC (ss a, ss b)\
    {return FUNC (parms() << a << b);}\
  RETURN FUNC (ss a, ss b, ss c)\
    {return FUNC (parms() << a << b << c);}\
  RETURN FUNC (ss a, ss b, ss c, ss d)\
    {return FUNC (parms() << a << b << c << d);}\
  RETURN FUNC (ss a, ss b, ss c, ss d, ss e)\
    {return FUNC (parms() << a << b << c << d << e);} \
  RETURN FUNC (ss a, ss b, ss c, ss d, ss e, ss f)\
    {return FUNC (parms() << a << b << c << d << e << f);}\
  RETURN FUNC (ss a, ss b, ss c, ss d, ss e, ss f, ss g)\
    {return FUNC (parms() << a << b << c << d << e << f << g);}\
  RETURN FUNC (ss a, ss b, ss c, ss d, ss e, ss f, ss g, ss h)\
    {return FUNC (parms() << a << b << c << d << e << f << g << h);}\
  RETURN FUNC (ss a, ss b, ss c, ss d, ss e, ss f, ss g, ss h, ss i)\
    {return FUNC (parms() << a << b << c << d << e << f << g << h << i);}\
  RETURN FUNC (ss a,ss b,ss c,ss d,ss e,ss f,ss g,ss h,ss i,ss j)\
    {return FUNC (parms() <<a <<b <<c <<d <<e <<f <<g <<h <<i <<j);}\
  RETURN FUNC (ss a,ss b,ss c,ss d,ss e,ss f,ss g,ss h,ss i,ss j,ss k)\
    {return FUNC (parms() <<a <<b <<c <<d <<e <<f <<g <<h <<i <<j <<k);}\
  RETURN FUNC (ss a,ss b,ss c,ss d,ss e,ss f,ss g,ss h,ss i,ss j,ss k,\
		       ss l)\
    {return FUNC (parms() <<a <<b <<c <<d <<e <<f <<g <<h <<i <<j <<k <<l);}\

#define mysql_query_define1(RETURN, FUNC) \
  RETURN FUNC (query_reset r = RESET_QUERY) {return FUNC (def,r);} \
  RETURN FUNC (const char* str); \
  RETURN FUNC (parms &p, query_reset r = RESET_QUERY);\
  mysql_query_define0(RETURN,FUNC) \

#define mysql_query_define2(FUNC) \
  template <class T1> void FUNC (T1 &con, query_reset r = RESET_QUERY) {FUNC (con, def,r);} \
  template <class T1> void FUNC (T1 &con, const char* str); \
  template <class T1> void FUNC (T1 &con, parms &p, query_reset r = RESET_QUERY);\
  template <class T1> void FUNC (T1 &con, ss a)\
    {FUNC (con, parms() << a);}\
  template <class T1> void FUNC (T1 &con, ss a, ss b)\
    {FUNC (con, parms() << a << b);}\
  template <class T1> void FUNC (T1 &con, ss a, ss b, ss c)\
    {FUNC (con, parms() << a << b << c);}\
  template <class T1> void FUNC (T1 &con, ss a, ss b, ss c, ss d)\
    {FUNC (con, parms() << a << b << c << d);}\
  template <class T1> void FUNC (T1 &con, ss a, ss b, ss c, ss d, ss e)\
    {FUNC (con, parms() << a << b << c << d << e);} \
  template <class T1> void FUNC (T1 &con, ss a, ss b, ss c, ss d, ss e, ss f)\
    {FUNC (con, parms() << a << b << c << d << e << f);}\
  template <class T1> void FUNC (T1 &con,ss a,ss b,ss c,ss d,ss e,ss f,ss g)\
    {FUNC (con, parms() << a << b << c << d << e << f << g);}\
  template <class T1> void FUNC (T1 &con,ss a,ss b,ss c,ss d,ss e,ss f,ss g,ss h)\
    {FUNC (con, parms() << a << b << c << d << e << f << g << h);}\
  template <class T1> void FUNC (T1 &con, ss a, ss b, ss c, ss d, ss e, ss f, ss g, ss h, ss i)\
    {FUNC (con, parms() << a << b << c << d << e << f << g << h << i);}\
  template <class T1> void FUNC (T1 &con, ss a,ss b,ss c,ss d,ss e,ss f,ss g,ss h,ss i,ss j)\
    {FUNC (con, parms() <<a <<b <<c <<d <<e <<f <<g <<h <<i <<j);}\
  template <class T1> void FUNC (T1 &con, ss a,ss b,ss c,ss d,ss e,ss f,ss g,ss h,ss i,ss j,ss k)\
    {FUNC (con, parms() <<a <<b <<c <<d <<e <<f <<g <<h <<i <<j <<k);}\
  template <class T1> void FUNC (T1 &con, ss a,ss b,ss c,ss d,ss e,ss f,ss g,ss h,ss i,ss j,ss k,\
		       ss l)\
    {FUNC (con, parms() <<a <<b <<c <<d <<e <<f <<g <<h <<i <<j <<k <<l);}\


#endif
