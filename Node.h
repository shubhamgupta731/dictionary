#ifndef _NODE_H_INCLUDED
#define _NODE_H_INCLUDED
#include "Node.decl.h"

template <class A>
A& tmb::Node<A>::get() {
   _val = *(*_active_get);
   _active_get = _get_solved;
   return _val;
}

template <class A>
template <class B>
A& tmb::Node<A>::set(B val) {
   _val = val;
   _active_get = _get_solved;
   return _val;
}

template <class A>
void tmb::Node<A>::addStrategy(Loki::SmartPtr<FunctorWrapper<A&> > strategy) {
   _vec_functors.push_back(strategy);
}

template <class A>
A& tmb::Node<A>::get_solved() {
   return _val;
}

template <class A>
tmb::Node<A>::Node(std::string name)
    : _active_get(NULL) {
   _name = name;
}

template <class A>
tmb::Node<A>::Node(const tmb::Node<A>& copy_from)
    : _val(copy_from.val)
    , _name(copy_from.name)
    , _vec_functors(copy_from._vec_functors)
    , _active_get(copy_from._active_get) {}
#endif
