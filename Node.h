#ifndef _NODE_H_INCLUDED
#define _NODE_H_INCLUDED
#include "Node.decl.h"
#include "FunctorWrapper.h"
#include <iostream>

template <class A>
A& tmb::Node<A>::get() {
   _val = (*(*_active_get))();
   _active_get = &_get_solved;
   return _val;
}

template <class A>
const A& tmb::Node<A>::get_const_ref() {
   return get();
}

template <class A>
A tmb::Node<A>::get_copy() {
   return get();
}

template <class A>
A* tmb::Node<A>::get_pointer() {
   get();
   return &_val;
}

template <class A>
const A* tmb::Node<A>::get_const_pointer() {
   return get_pointer();
}

template <class A>
template <class B>
A& tmb::Node<A>::set(B val) {
   _val = val;
   _active_get = &_get_solved;
   notify();
   return _val;
}

template <class A>
A& tmb::Node<A>::get_solved() {
   return _val;
}

template <class A>
tmb::Node<A>::Node(std::string name)
    : _active_get(NULL) {
   _name = name;
   _get_solved = new Loki::Functor<A>(this, &Node<A>::get_solved);
   _get_func = new Loki::Functor<A&>(this, &tmb::Node<A>::get);
   _get_copy_func = new Loki::Functor<A>(this, &tmb::Node<A>::get_copy);
   _get_const_ref_func =
       new Loki::Functor<const A&>(this, &tmb::Node<A>::get_const_ref);
   _get_pointer_func = new Loki::Functor<A*>(this, &tmb::Node<A>::get_pointer);
   _get_const_pointer_func =
       new Loki::Functor<const A*>(this, &tmb::Node<A>::get_const_pointer);
}

template <class A>
tmb::Node<A>::Node(const tmb::Node<A>& copy_from)
    : _val(copy_from._val)
    , _name(copy_from._name)
    , _vec_functors(copy_from._vec_functors)
    , _active_get(copy_from._active_get)
    , _get_solved(copy_from._get_solved)
    , _get_func(copy_from._get_func)
    , _get_copy_func(copy_from._get_copy_func)
    , _get_const_ref_func(copy_from._get_const_ref_func)
    , _get_pointer_func(copy_from._get_pointer_func)
    , _get_const_pointer_func(copy_from._get_const_pointer_func) {}

namespace tmb {
   template <bool flag>
   struct Wrap {
      template <class Return, class Arg>
      static Loki::Functor<Return>* doF(Arg arg) {
         return tmb::wrap_var_with_functor<Return>(arg);
      }
   };

   struct ReturnPointer {
      template <class Return, class Arg>
      static Loki::Functor<Return>* doF(Arg arg) {
         return &(arg->get_pointer_func());
      }
   };
   struct ReturnConstPointer {
      template <class Return, class Arg>
      static Loki::Functor<Return>* doF(Arg arg) {
         return &(arg->get_const_pointer_func());
      }
   };
   struct ReturnCopy {
      template <class Return, class Arg>
      static Loki::Functor<Return>* doF(Arg arg) {
         return &(arg->get_copy_func());
      }
   };
   struct ReturnRef {
      template <class Return, class Arg>
      static Loki::Functor<Return>* doF(Arg arg) {
         return &(arg->get_get_func());
      }
   };
   struct ReturnConstRef {
      template <class Return, class Arg>
      static Loki::Functor<Return>* doF(Arg arg) {
         return &(arg->get_const_ref_func());
      }
   };
   template <>
   struct Wrap<true> {
      template <class Return, class Arg>
      static Loki::Functor<Return>* doF(Arg arg) {
         typedef typename Loki::Select<
             Loki::TypeTraits<Return>::isReference,
             typename Loki::Select<Loki::TypeTraits<Return>::isConst,
                                   ReturnConstRef,
                                   ReturnRef>::Result,
             typename Loki::Select<
                 Loki::TypeTraits<Return>::isPointer,
                 typename Loki::Select<Loki::TypeTraits<Return>::isConst,
                                       ReturnConstPointer,
                                       ReturnPointer>::Result,
                 ReturnCopy>::Result>::Result Result;
         return Result::template doF<Return>(arg);
      }
   };

   template <bool flag>
   struct AddObserver {
      template <class Arg1, class Arg2>
      static void doF(Arg1 arg1, Arg2 arg2) {
         arg1->attach(arg2);
      }
   };

   template <>
   struct AddObserver<false> {
      template <class Arg1, class Arg2>
      static void doF(Arg1, Arg2) {}
   };
}

template <class A>
template <class Arg1, class Arg2>
void tmb::Node<A>::addStrategy(Loki::Functor<A, TYPELIST(Arg1)>* functor,
                               Arg2 arg1) {
   Loki::Tuple<TYPELIST(Loki::Functor<Arg1>*)> tuple_args;
   typedef typename Loki::Select<Loki::TypeTraits<Arg2>::isPointer,
                                 typename Loki::TypeTraits<Arg2>::PointeeType,
                                 Arg2>::Result Arg2Type;

   Loki::Field<0>(tuple_args) =
       Wrap<has_is_a_node<Arg2Type>::Result>::template doF<Arg1, Arg2>(arg1);

   Loki::SmartPtr<tmb::FunctorWrapper<A, TYPELIST(Arg1)> > wrap =
       new tmb::FunctorWrapper<A, TYPELIST(Arg1)>(*functor, tuple_args);

   _vec_functors.push_back(new Loki::Functor<A>(
       wrap, &tmb::FunctorWrapper<A, TYPELIST(Arg1)>::return_val));

   // Add a new observer
   tmb::NodeObserver<A>* observer =
       new tmb::NodeObserver<A>(_vec_functors.size() - 1, this);
   // Check if arg1 is a subject or not?
   tmb::AddObserver<Loki::Conversion<Arg2Type, Subject>::exists>::template doF<
       Arg2>(arg1, observer);
   _active_get = &(_vec_functors.back());
}

template <class A>
tmb::NodeObserver<A>::NodeObserver(size_t index, Node<A>* ptr)
    : _index(index), _ptr(ptr) {}

template <class A>
void tmb::NodeObserver<A>::update() {
   _ptr->set_active_strategy(_index);
   _ptr->notify();
}

template <class A>
const std::vector<Loki::SmartPtr<Loki::Functor<A> > >&
    tmb::Node<A>::get_vec_functors() const {
   return _vec_functors;
}

template <class A>
void tmb::Node<A>::set_active_strategy(size_t index) {
   _active_get = &(_vec_functors[index]);
}

template <class A>
tmb::Node<A>::operator A() {
   return get();
}

template <class A>
Loki::Functor<A&>& tmb::Node<A>::get_get_func() {
   return *_get_func;
}

template <class A>
Loki::Functor<A>& tmb::Node<A>::get_copy_func() {
   return *_get_copy_func;
}

template <class A>
Loki::Functor<const A&>& tmb::Node<A>::get_const_ref_func() {
   return *_get_const_ref_func;
}

template <class A>
Loki::Functor<A*>& tmb::Node<A>::get_pointer_func() {
   return *_get_pointer_func;
}

template <class A>
Loki::Functor<const A*>& tmb::Node<A>::get_const_pointer_func() {
   return *_get_const_pointer_func;
}
#endif
