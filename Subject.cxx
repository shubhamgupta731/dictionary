#include "Subject.h"
#include "Observer.h"

Subject::Subject() {}

Subject::Subject(const Subject &copy_from) {
   for (std::vector<Observer *>::const_iterator it = copy_from.views.begin();
        it != copy_from.views.end();
        ++it)
      views.push_back(new Observer(**it));
}

Subject::~Subject() {
   for (std::vector<Observer *>::iterator it = views.begin(); it != views.end();
        ++it)
      delete *it;
   views.clear();
}

void Subject::notify() {
   std::vector<Observer *>::iterator it_end = views.end();
   for (std::vector<Observer *>::iterator it = views.begin(); it != it_end;
        ++it)
      (*it)->update();
}

void Subject::attach(Observer *obs) { views.push_back(obs); }
