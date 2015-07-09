#ifndef _NODE_H_INCLUDED
#define _NODE_H_INCLUDED
#include "Node.decl.h"
#include "FunctorWrapper.h"

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
void tmb::Node<A>::addStrategy(Loki::SmartPtr<Loki::Functor<A> > strategy) {
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

template <bool flag>
struct Wrap {
   template <class A>
   static Loki::Functor<A>* doF(A arg) {
      return tmb::wrap_var_with_functor<A>(arg);
   }
};

template <>
struct Wrap<true> {
   template <class A>
   static Loki::Functor<A>* doF(A arg) {
      return &(arg._func_no_args);
   }
};
template <class A>
template <class Arg1>
void tmb::Node<A>::addStrategy(Loki::Functor<A, TYPELIST(Arg1)>* functor,
                               Arg1 arg1) {
   Loki::Tuple<TYPELIST(Loki::Functor<Arg1>*)> tuple_args;
   Loki::Field<0>(tuple_args) = Loki::Select<has_is_a_node<Arg1>::Result,
                                             Wrap<true>,
                                             Wrap<false> >::Result::doF(arg1);
   tmb::FunctorWrapper<A, TYPELIST(Arg1)>* wrap =
       new tmb::FunctorWrapper<A, TYPELIST(Arg1)>(functor, tuple_args);
   _vec_functors.push_back(wrap->_func_no_args);
}
#endif
