#ifndef _OBSERVER_DECL_H_INCLUDED
#define _OBSERVER_DECL_H_INCLUDED

class Observer {
  protected:

  public:
   virtual ~Observer();
   virtual void update() = 0;
};

#endif
