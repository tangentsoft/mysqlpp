#ifndef __connection0_hh__
#define __connection0_hh__

class MysqlChild {
public:
  virtual void parent_leaving() = 0;
  virtual ~MysqlChild() {}
};

#endif



