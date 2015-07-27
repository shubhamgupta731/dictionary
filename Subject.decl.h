#ifndef _SUBJECT_DECL_H_INCLUDED
#define _SUBJECT_DECL_H_INCLUDED

#include <vector>
class Observer;

class Subject {
  protected:
   Observer *views[2];    // 3. Coupled only to "interface"
   char _number_of_views;

  public:
   Subject();
   Subject(const Subject &copy_from);
   virtual ~Subject();
   virtual void attach(Observer *obs);
   virtual void notify();
};

#endif
