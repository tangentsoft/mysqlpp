
// defs.h

#ifndef __define_short_h__
#define __define_short_h__

#define use_exceptions mysql_use_exceptions

// base.h           
#define BadQuery    MysqlBadQuery
#define MysqlCon    MysqlConnection
#define Connection  MysqlConnection

// result.h
#define ResNSel     MysqlResNSel
#define ResUse      MysqlResUse
#define Result      MysqlRes
#define Field       MysqlField
#define Fields      MysqlFields

// resiter.h
#define ResIter     MysqlResIter
#define ResultIter  MysqlResIter

// row.h
#define Row          MysqlRow
// #define MutableRow   MysqlMutableRow

// field_names.h
#define FieldNames  MysqlFieldNames

//query.h
#define Query       MysqlQuery

//string.h
#define BadConversion  MysqlBadConversion
#define ColData        MysqlColData
#define MutableColData MysqlMutableColData

// manip.h
#define quote              mysql_quote
#define quote_only         mysql_quote_only
#define quote_double_only  mysql_quote_double_only
#define escape             mysql_escape
#define do_nothing         mysql_do_nothing
#define ignore             mysql_ignore

// datetime.h
#define Date        MysqlDate
#define Time        MysqlTime
#define DateTime    MysqlDateTime

// set.h
#define Set         MysqlSet

// null.h
#define Null        MysqlNull
#define null_type   mysql_null_type
#define null        mysql_null
#define NullisNull  MysqlNullisNull
#define NullisZero  MysqlNullisZero
#define NullisBlank MysqlNullisBlank

#endif
