#include <datetime1.hh>
#include <set1.hh>
#include <type_info3.hh>

#include <mysql++-windows.hh>
#include <mysql.h>

#include <string>

using namespace std;

typedef string Enum;

const mysql_type_info::sql_type_info mysql_type_info::types[62] = {
  sql_type_info("DECIMAL NOT NULL", typeid(double), 0),
  sql_type_info("TINYINT NOT NULL", typeid(signed char), 1, true),
  sql_type_info("SMALLINT NOT NULL", typeid(short int), 2, true),
  sql_type_info("INT NOT NULL", typeid(int), 3, true),
  sql_type_info("FLOAT NOT NULL", typeid(float), 4, true),
  sql_type_info("DOUBLE NOT NULL", typeid(double), 5, true),
  sql_type_info("NULL NOT NULL", typeid(void), 6),
  sql_type_info("TIMESTAMP NOT NULL", typeid(Time), 7),
  sql_type_info("BIGINT NOT NULL", typeid(longlong), 8, true),
  sql_type_info("MEDIUMINT NOT NULL", typeid(int), 9),
  sql_type_info("DATE NOT NULL", typeid(Date), 10, true),
  sql_type_info("TIME NOT NULL", typeid(Time), 11, true),
  sql_type_info("DATETIME NOT NULL", typeid(DateTime), 12, true),
  sql_type_info("ENUM NOT NULL", typeid(Enum), 13, true),
  sql_type_info("SET NOT NULL", typeid(Set<string>), 14, true),
  sql_type_info("TINYBLOB NOT NULL", typeid(string), 15),
  sql_type_info("MEDIUMBLOB NOT NULL", typeid(string), 16),
  sql_type_info("LONGBLOB NOT NULL", typeid(string), 17),
  sql_type_info("BLOB NOT NULL", typeid(string), 18),
  sql_type_info("VARCHAR NOT NULL", typeid(string), 19, true),
  sql_type_info("CHAR NOT NULL", typeid(string), 20),
  sql_type_info("CHAR NOT NULL", typeid(string), 21),
  sql_type_info("TINYINT UNSIGNED NOT NULL", typeid(unsigned char), 22, true),
  sql_type_info("SMALLINT UNSIGNED NOT NULL", typeid(unsigned short int), 23, true),
  sql_type_info("INT UNSIGNED NOT NULL", typeid(unsigned int), 24, true),
  sql_type_info("INT UNSIGNED NOT NULL", typeid(unsigned int), 25, true),
  sql_type_info("INT UNSIGNED NOT NULL", typeid(unsigned int), 26, true),
  sql_type_info("INT UNSIGNED NOT NULL", typeid(unsigned int), 27, true),
  sql_type_info("INT UNSIGNED NOT NULL", typeid(unsigned int), 28, true),
  sql_type_info("BIGINT UNSIGNED NOT NULL", typeid(ulonglong), 29, true),
  sql_type_info("MEDIUMINT UNSIGNED NOT NULL", typeid(unsigned int), 30),

  sql_type_info("DECIMAL NULL", typeid(Null<double>), 0),
  sql_type_info("TINYINT NULL", typeid(Null<signed char>), 1, true),
  sql_type_info("SMALLINT NULL", typeid(Null<short int>), 2, true),
  sql_type_info("INT NULL", typeid(Null<int>), 3, true),
  sql_type_info("FLOAT NULL", typeid(Null<float>), 4, true),
  sql_type_info("DOUBLE NULL", typeid(Null<double>), 5, true),
  sql_type_info("NULL NULL", typeid(Null<void>), 6),
  sql_type_info("TIMESTAMP NULL", typeid(Null<Time>), 7),
  sql_type_info("BIGINT NULL", typeid(Null<longlong>), 8, true),
  sql_type_info("MEDIUMINT NULL", typeid(Null<int>), 9), 
  sql_type_info("DATE NULL", typeid(Null<Date>), 10, true),
  sql_type_info("TIME NULL", typeid(Null<Time>), 11, true),
  sql_type_info("DATETIME NULL", typeid(Null<DateTime>), 12, true),
  sql_type_info("ENUM NULL", typeid(Null<Enum>), 13, true),
  sql_type_info("SET NULL", typeid(Null<Set<string> >), 14, true),
  sql_type_info("TINYBLOB NULL", typeid(Null<string>), 15),
  sql_type_info("MEDIUMBLOB NULL", typeid(Null<string>), 16),
  sql_type_info("LONGBLOB NULL", typeid(Null<string>), 17),
  sql_type_info("BLOB NULL", typeid(Null<string>), 18),
  sql_type_info("VARCHAR NULL", typeid(Null<string>), 19, true),
  sql_type_info("CHAR NULL", typeid(Null<string>), 20),
  sql_type_info("CHAR NULL", typeid(Null<string>), 21),
  sql_type_info("TINYINT UNSIGNED NULL", typeid(Null<unsigned char>), 22, true),
  sql_type_info("SMALLINT UNSIGNED NULL", typeid(Null<unsigned short int>),23, true),
  sql_type_info("INT UNSIGNED NULL", typeid(Null<unsigned int>), 24, true),
  sql_type_info("INT UNSIGNED NULL", typeid(Null<unsigned int>), 25, true),
  sql_type_info("INT UNSIGNED NULL", typeid(Null<unsigned int>), 26, true),
  sql_type_info("INT UNSIGNED NULL", typeid(Null<unsigned int>), 27, true),
  sql_type_info("INT UNSIGNED NULL", typeid(Null<unsigned int>), 28, true),
  sql_type_info("BIGINT UNSIGNED NULL", typeid(Null<ulonglong>),29, true),
  sql_type_info("MEDIUMINT UNSIGNED NULL", typeid(Null<unsigned int>), 30),
};

const mysql_type_info::sql_type_info_lookup 
  mysql_type_info::lookups(mysql_type_info::types, 62);

mysql_ti_sql_type_info_lookup::
mysql_ti_sql_type_info_lookup(const sql_type_info types[], const int size) {
  for (int i = 0; i != size; i++) {
    if (types[i]._default)
      _map[types[i]._c_type] = i;
  }
}

unsigned char mysql_type_info::type(enum_field_types t, 
			              bool _unsigned, bool _null) {
  if (_null) {
    if (_unsigned) {
      return unsigned_null_offset + t;
    } else {
      if (t < 200) return null_offset + t;
      else return null_offset + (t-234);
    }
  } else {
    if (_unsigned) {
      return unsigned_offset + t;
    } else {
      if (t < 200) return offset + t;
      else return offset + (t-234);
    }
  }
}

bool mysql_type_info::quote_q() const {
  if (base_type().c_type() == typeid(string) ||
      base_type().c_type() == typeid(Date) ||
      base_type().c_type() == typeid(Time) ||
      base_type().c_type() == typeid(DateTime) ||
      base_type().c_type() == typeid(Enum) ||
      base_type().c_type() == typeid(Set<string>)) 
    {
      return true;
    } else {
      return false;
    }
}

bool mysql_type_info::escape_q() const {
  if (c_type() == typeid(string)) 
    return true;
  else 
    return false;
}
