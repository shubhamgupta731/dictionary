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
   for (std::size_t i = 0; i < views.size(); i++)
      views[i]->update();
}

void Subject::attach(Observer *obs) { views.push_back(obs); }
