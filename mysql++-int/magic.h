#define virtual_def (oper) \
  virtual operator oper () const = 0; \
  virtual holder* create(const &oper) = 0

#define virtual_int_def (oper) \
  virtual_def (oper); \
  vortual_def (unsigned oper)


class MagicType {
  class holder_base {
  public:
    virtual_int_def (char);
    virtual_int_def (short int);
    virtual_int_def (long ing);
    virtual_int_def (long long int);
    virtual_def (float);
    virtual_def (double);
    virtual_def (string);
  }
  template <class Type, const char* Name, tiny_int Id, unsigned short int Id2,
            unsigned short int Mask>
  class holder : public holder_base {
  public:
    Type value;
    static const name = Name;
    static const tiny_int id = Id;
    static const unsigned short int id2 = Id2;
    static const unsigned short int mask = Mask;
  }

    
  
      
    
