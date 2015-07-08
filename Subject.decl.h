#ifndef _SUBJECT_DECL_H_INCLUDED
#define _SUBJECT_DECL_H_INCLUDED

#include <vector>
class Observer;

class Subject {
  protected:
   std::vector<Observer *> views;    // 3. Coupled only to "interface"
  public:
   Subject();
   Subject(const Subject &copy_from);
   virtual ~Subject();
   virtual void attach(Observer *obs);
   virtual void notify();
};

#endif
