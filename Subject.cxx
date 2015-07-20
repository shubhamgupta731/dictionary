#include "Subject.h"
#include "Observer.h"
#include <cstring>

Subject::Subject() : _number_of_views(0) {}

Subject::Subject(const Subject &copy_from)
    : _number_of_views(copy_from._number_of_views) {
   memcpy(views, copy_from.views, _number_of_views);
   // for (std::vector<Observer *>::const_iterator it = copy_from.views.begin();
   //     it != copy_from.views.end();
   //     ++it)
   //   views.push_back(new Observer(**it));
}

Subject::~Subject() {
   for (char i = 0; i < _number_of_views; ++i)
      delete views[i];
}

void Subject::notify() {
   for (char i = 0; i < _number_of_views; ++i)
      views[i]->update();
}

void Subject::attach(Observer *obs) {
   views[_number_of_views] = obs;
   ++_number_of_views;
}
