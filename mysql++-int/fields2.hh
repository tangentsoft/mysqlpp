
#ifndef __fields2_hh__
#define __fields2_hh__

inline Fields::size_type Fields::size() const {return res->num_fields();}

inline const Field& Fields::operator [] (Fields::size_type i) const {
    res->field_seek(i); 
    return res->fetch_field();
}

#endif
