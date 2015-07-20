#ifndef _OBSERVER_DECL_H_INCLUDED
#define _OBSERVER_DECL_H_INCLUDED

class Observer {
  protected:
   // Subject* _model[4];
   // std::vector<char> _subject_updated;

  public:
   // Observer();
   // Observer(Subject *mod);
   // Observer(const Observer &copy_from);
   // void attachObserver(Subject *mod);
   virtual ~Observer();
   virtual void update() = 0;
};

#endif
