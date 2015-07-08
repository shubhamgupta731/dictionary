#ifndef _OBSERVER_DECL_H_INCLUDED
#define _OBSERVER_DECL_H_INCLUDED
#include <vector>

class Subject;

class Observer {
  protected:
   std::vector<Subject *> _model;
   std::vector<char> _subject_updated;

  public:
   Observer();
   Observer(Subject *mod);
   Observer(const Observer &copy_from);
   void attachObserver(Subject *mod);
   virtual ~Observer();
   virtual void update();
};

#endif
