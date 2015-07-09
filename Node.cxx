#include "Node.h"

template <class A>
tmb::NodeObserver<A>::NodeObserver(size_t index, Node<A>* ptr)
    : _index(index), _ptr(ptr) {}

template <class A>
void tmb::NodeObserver<A>::update() {
   _ptr->_active_get = _ptr->_vec_functors[_index];
   _ptr->notify();
}
