
#ifndef __mysql_tracker_h__
#define __mysql_tracker_h__

#include <set.h>
#include <map.h>

template <class Pointer, class Object> 
class pointer_tracker {
private:
  map<Pointer*, set<Object*> > tracker;
public:
  void insert (Pointer *p, Object *o) {
    tracker[p].insert(o);
  }
  void remove (Pointer *p, Object *o) {
    tracker[p].erase(o); 
    if (tracker[p].empty()) {
      tracker.erase(p);
      o->purge(p);
    }
  }
};

#endif
