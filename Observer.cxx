#include "Observer.h"
#include "Subject.h"

Observer::Observer() {}

Observer::Observer(Subject *mod) {
   _model.push_back(mod);
   _subject_updated.push_back(false);
   mod->attach(this);
}
Observer::Observer(const Observer &copy_from) {
   _model = copy_from._model;
   _subject_updated = copy_from._subject_updated;
}
void Observer::attachObserver(Subject *mod) {
   _model.push_back(mod);
   _subject_updated.push_back(false);
}
Observer::~Observer() {}
void Observer::update() {}
