#ifndef __row1_hh__
#define __row1_hh__

#include <vector>
#include <string>
#include "exceptions.hh"
#include "defs"
#include "define_short"
#include "coldata1.hh"
#include "resiter1.hh"
#include "vallist1.hh"

template <class ThisType, class Res>
class RowTemplate {
protected:
  virtual ThisType& self() = 0;
  virtual const ThisType& self() const = 0;
public:
  virtual const Res & parent() const = 0;
  
  // 
  // value_list
  //
  value_list_ba<ThisType,quote_type0> 
  value_list(const char *d = ",") const {
    return value_list_ba<ThisType,quote_type0> 
      (self(), d, quote);
  }

  template <class Manip>  
  value_list_ba<ThisType,Manip> 
  value_list(const char *d, Manip m) const {
    return value_list_ba<ThisType,Manip> (self(), d, m);
  }

  template <class Manip>  
  value_list_b<ThisType,Manip> 
  value_list(const char *d, Manip m, const vector<bool> &vb) const {
    return value_list_b<ThisType,Manip> (self(), vb, d, m);
  }

  value_list_b<ThisType,quote_type0> 
  value_list(const char *d, const vector<bool> &vb) const {
    return value_list_b<ThisType,quote_type0> (self(), vb, d, quote);
  }

  value_list_b<ThisType,quote_type0> 
  value_list(const vector<bool> &vb) const {
    return value_list_b<ThisType,quote_type0> (self(), vb, ",", quote);
  }

  template <class Manip>  
  value_list_b<ThisType,Manip> 
  value_list(const char *d, Manip m, bool t0,
	     bool t1=false, bool t2=false, bool t3=false, bool t4=false,
	     bool t5=false, bool t6=false, bool t7=false, bool t8=false,
	     bool t9=false, bool ta=false, bool tb=false, bool tc=false) const {
    vector<bool> vb;
    create_vector(self().size(), vb, t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,ta,tb,tc);
    return value_list_b<ThisType, Manip>(self(), vb, d, m);
  }

  value_list_b<ThisType,quote_type0> 
  value_list(const char *d, bool t0,
	     bool t1=false, bool t2=false, bool t3=false, bool t4=false,
	     bool t5=false, bool t6=false, bool t7=false, bool t8=false,
	     bool t9=false, bool ta=false, bool tb=false, bool tc=false) const {
    vector<bool> vb;
    create_vector(self().size(), vb, t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,ta,tb,tc);
    return value_list_b<ThisType, quote_type0>(self(), vb, d, quote);
  }

  value_list_b<ThisType,quote_type0> 
  value_list(bool t0,
	     bool t1=false, bool t2=false, bool t3=false, bool t4=false,
	     bool t5=false, bool t6=false, bool t7=false, bool t8=false,
	     bool t9=false, bool ta=false, bool tb=false, bool tc=false) const {
    vector<bool> vb;
    create_vector(self().size(), vb, t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,ta,tb,tc);
    return value_list_b<ThisType, quote_type0>(self(), vb, ",", quote);
  }

  template <class Manip>
  value_list_b<ThisType,Manip> 
  value_list(const char *d, Manip m, string s0,
	     string s1="", string s2="", string s3="", string s4="",
	     string s5="", string s6="", string s7="", string s8="",
	     string s9="", string sa="", string sb="", string sc="") const {
    vector<bool> vb;
    create_vector(self(), vb, s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,sa,sb,sc);
    return value_list_b<ThisType, Manip>(self(), vb, d, m);
  }

  value_list_b<ThisType,quote_type0> 
  value_list(const char *d, string s0,
	     string s1="", string s2="", string s3="", string s4="",
	     string s5="", string s6="", string s7="", string s8="",
	     string s9="", string sa="", string sb="", string sc="") const {
    vector<bool> vb;
    create_vector(self(), vb, s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,sa,sb,sc);
    return value_list_b<ThisType, quote_type0>(self(), vb, d, quote);
  }

  value_list_b<ThisType,quote_type0> 
  value_list(string s0,
	     string s1="", string s2="", string s3="", string s4="",
	     string s5="", string s6="", string s7="", string s8="",
	     string s9="", string sa="", string sb="", string sc="") const {
    vector<bool> vb;
    create_vector(self(), vb, s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,sa,sb,sc);
    return value_list_b<ThisType, quote_type0>(self(), vb, ",", quote);
  }

  //
  // field_list
  //
  value_list_ba<FieldNames, do_nothing_type0> 
  field_list(const char *d = ",") const {
    return value_list_ba<FieldNames, do_nothing_type0> 
      (parent().names(), d, do_nothing);
  }

  template <class Manip>  
  value_list_ba<FieldNames,Manip> 
  field_list(const char *d, Manip m) const {
    return value_list_ba<FieldNames, Manip>
      (parent().names(), d, m);
  }

  template <class Manip>  
  value_list_b<FieldNames,Manip> 
  field_list(const char *d, Manip m, const vector<bool> &vb) const {
    return value_list_b<FieldNames,Manip> (parent().names(), vb, d, m);
  }

  value_list_b<FieldNames,quote_type0> 
  field_list(const char *d, const vector<bool> &vb) const {
    return value_list_b<FieldNames,quote_type0> (parent().names(), vb, d, quote);
  }

  value_list_b<FieldNames,quote_type0> 
  field_list(const vector<bool> &vb) const {
    return value_list_b<FieldNames,quote_type0> (parent().names(), vb, ",", quote);
  }

  template <class Manip>  
  value_list_b<FieldNames,Manip> 
  field_list(const char *d, Manip m, bool t0,
	     bool t1=false, bool t2=false, bool t3=false, bool t4=false,
	     bool t5=false, bool t6=false, bool t7=false, bool t8=false,
	     bool t9=false, bool ta=false, bool tb=false, bool tc=false) const {
    vector<bool> vb;
    create_vector(parent().names().size(), vb, t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,ta,tb,tc);
    return value_list_b<FieldNames, Manip>(parent().names(), vb, d, m);
  }

  value_list_b<FieldNames,quote_type0> 
  field_list(const char *d, bool t0,
	     bool t1=false, bool t2=false, bool t3=false, bool t4=false,
	     bool t5=false, bool t6=false, bool t7=false, bool t8=false,
	     bool t9=false, bool ta=false, bool tb=false, bool tc=false) const {
    vector<bool> vb;
    create_vector(parent().names().size(), vb, t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,ta,tb,tc);
    return value_list_b<FieldNames, quote_type0>(parent().names(), vb, d, quote);
  }

  value_list_b<FieldNames,quote_type0> 
  field_list(bool t0,
	     bool t1=false, bool t2=false, bool t3=false, bool t4=false,
	     bool t5=false, bool t6=false, bool t7=false, bool t8=false,
	     bool t9=false, bool ta=false, bool tb=false, bool tc=false) const {
    vector<bool> vb;
    create_vector(parent().names().size(), vb, t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,ta,tb,tc);
    return value_list_b<FieldNames, quote_type0>(parent().names(), vb, ",", quote);
  }

  template <class Manip>
  value_list_b<FieldNames,Manip> 
  field_list(const char *d, Manip m, string s0,
	     string s1="", string s2="", string s3="", string s4="",
	     string s5="", string s6="", string s7="", string s8="",
	     string s9="", string sa="", string sb="", string sc="") const {
    vector<bool> vb;
    create_vector(parent().names(), vb, s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,sa,sb,sc);
    return value_list_b<FieldNames, Manip>(parent().names(), vb, d, m);
  }

  value_list_b<FieldNames,quote_type0> 
  field_list(const char *d, string s0,
	     string s1="", string s2="", string s3="", string s4="",
	     string s5="", string s6="", string s7="", string s8="",
	     string s9="", string sa="", string sb="", string sc="") const {
    vector<bool> vb;
    create_vector(parent().names(), vb, s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,sa,sb,sc);
    return value_list_b<FieldNames, quote_type0>(parent().names(), vb, d, quote);
  }

  value_list_b<FieldNames,quote_type0> 
  field_list(string s0,
	     string s1="", string s2="", string s3="", string s4="",
	     string s5="", string s6="", string s7="", string s8="",
	     string s9="", string sa="", string sb="", string sc="") const {
    vector<bool> vb;
    create_vector(parent().names(), vb, s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,sa,sb,sc);
    return value_list_b<FieldNames, quote_type0>(parent().names(), vb, ",", quote);
  }

  //
  // equal_list
  //
  equal_list_ba<FieldNames,ThisType,quote_type0> 
  equal_list(const char *d = ",", const char *e = " = ") const {
    return equal_list_ba<FieldNames,ThisType,quote_type0> 
      (parent().names(), self(), d, e, quote);
  }

  template <class Manip>
  equal_list_ba<FieldNames,ThisType,Manip> 
  equal_list(const char *d, const char *e, Manip m) const {
    return equal_list_ba<FieldNames,ThisType,Manip> 
      (parent().names(), self(), d, e, m);
  }

  virtual ~RowTemplate() {}
};

//: This class handles the actual rows in an intelligent manner.
class Row : public const_subscript_container<Row,ColData,const ColData>,
	    public RowTemplate<Row, ResUse>
{
private:
  vector <string> data;
	vector <bool> is_nulls;
  const ResUse *res;
  bool         throw_exceptions, initialized;

public:
  Row() {}
  Row(MYSQL_ROW d, const ResUse *r, unsigned int *jj, bool te = false) 
    : res(r), throw_exceptions(te), initialized(false)
		{
		  if (!d || !r) {
				if (throw_exceptions) throw BadQuery("ROW or RES is NULL");
				else return;
			}
		  data.clear(); is_nulls.clear(); initialized=true;
		  for (unsigned int i=0;i<size();i++) 
			{
			  data.insert(data.end(),(d[i]) ? string(d[i],jj[i]) : (string)"NULL");
				is_nulls.insert(is_nulls.end(),d[i] ? false : true);
			}
		}
  const Row& self() const {return *this;}
  Row& self() {return *this;}

  const ResUse&  parent() const {return *res;}
  inline size_type     size() const;
  //: Returns the number of columns.
  const ColData   operator [] (int i) const {
	  if (!initialized) {
				if (throw_exceptions) throw BadQuery("Row not initialized");
				else return ColData();
			}
		return operator[](size_type(i));
	}
  inline const ColData   operator [] (size_type i) const;
  //: Returns the value of the field with the index of i.
  inline const ColData   operator [] (const char *) const;
  //: Returns the value of the field with the field name of i.
  // This method is not nearly as efficient as using the index number. Use sparingly. 
  inline const ColData   operator [] (const string &i) const;
  //: Returns the value of the field with the field name of i.
  // This method is not nearly as effecent as using the index number. Use sparingly. 

  const char *raw_data(int i) const {return data[i].data();}

  operator bool() const {return (data.size()) ? true : false;}
  //: Returns true if there is data in the row.
	~Row() {data.clear(); is_nulls.clear(); initialized = false;}	
}; 


#endif


