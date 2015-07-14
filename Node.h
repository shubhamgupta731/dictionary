#ifndef _NODE_H_INCLUDED
#define _NODE_H_INCLUDED
#include "Node.decl.h"
#include "FunctorWrapper.h"
#include <iostream>
#include <fstream>
#include <numeric>
#include <iterator>

template <class A>
std::string& tmb::Node<A>::get_name() {
   return _name;
}

template <class A>
A& tmb::Node<A>::get() {
   *_val = (*(*_active_get))();
   _active_get = &_get_solved;
   return *_val;
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
   return _val;
}

template <class A>
const A* tmb::Node<A>::get_const_pointer() {
   return get_pointer();
}

template <class A>
template <class B>
A& tmb::Node<A>::set(B val) {
   *_val = val;
   _active_get = &_get_solved;
   notify();
   return *_val;
}

template <class A>
A& tmb::Node<A>::get_solved() {
   return *_val;
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
   _val = new A();
}

template <class A>
tmb::Node<A>::Node(const tmb::Node<A>& copy_from)
    : _name(copy_from._name)
    , _vec_functors(copy_from._vec_functors)
    , _vec_functors_stream(copy_from._vec_functors_stream)
    , _active_get(copy_from._active_get) {
   _get_solved = new Loki::Functor<A>(this, &Node<A>::get_solved);
   _get_func = new Loki::Functor<A&>(this, &tmb::Node<A>::get);
   _get_copy_func = new Loki::Functor<A>(this, &tmb::Node<A>::get_copy);
   _get_const_ref_func =
       new Loki::Functor<const A&>(this, &tmb::Node<A>::get_const_ref);
   _get_pointer_func = new Loki::Functor<A*>(this, &tmb::Node<A>::get_pointer);
   _get_const_pointer_func =
       new Loki::Functor<const A*>(this, &tmb::Node<A>::get_const_pointer);
   _val = new A();
   *_val = *(copy_from._val);
}

template <class A>
tmb::Node<A>::~Node() {
   delete _val;
   delete _get_solved;
   delete _get_func;
   delete _get_copy_func;
   delete _get_const_ref_func;
   delete _get_pointer_func;
   delete _get_const_pointer_func;
   for (typename std::vector<Loki::Functor<A>*>::iterator it =
            _vec_functors.begin();
        it != _vec_functors.end();
        ++it)
      delete (*it);
   for (
       typename std::vector<Loki::Functor<std::vector<std::string> >*>::iterator
           it = _vec_functors_stream.begin();
       it != _vec_functors_stream.end();
       ++it)
      delete (*it);
   for (std::vector<tmb::BaseFunctorWrapper*>::iterator it =
            _vec_functor_wrappers.begin();
        it != _vec_functor_wrappers.end();
        ++it)
      delete (*it);
}

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
         arg2->attachObserver(arg1);
      }
   };

   template <>
   struct AddObserver<false> {
      template <class Arg1, class Arg2>
      static void doF(Arg1, Arg2) {}
   };

   template <bool flag>
   struct AddNodeName {
      template <class A>
      static void doF(A arg1, std::vector<std::string>& vec_str) {
         vec_str.push_back(arg1->get_name());
      }
   };

   template <>
   struct AddNodeName<false> {
      template <class A>
      static void doF(A, std::vector<std::string>& vec_str) {
         vec_str.push_back("uknown");
      }
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

   tmb::FunctorWrapper<A, TYPELIST(Arg1)>* wrap =
       new tmb::FunctorWrapper<A, TYPELIST(Arg1)>(*functor, tuple_args);

   _vec_functors.push_back(new Loki::Functor<A>(
       wrap, &tmb::FunctorWrapper<A, TYPELIST(Arg1)>::return_val));
   _vec_functors_stream.push_back(new Loki::Functor<std::vector<std::string> >(
       wrap, &tmb::FunctorWrapper<A, TYPELIST(Arg1)>::args_as_stream));
   _vec_subjects.resize(_vec_subjects.size() + 1);

   _vec_dependencies.push_back(std::vector<unsigned>(1, 0));

   // Add a new observer
   tmb::NodeObserver<A>* observer =
       new tmb::NodeObserver<A>(_vec_functors.size() - 1, 0, this);
   // Check if arg1 is a subject or not?
   tmb::AddObserver<Loki::Conversion<Arg2Type, Subject>::exists>::template doF<
       Arg2>(arg1, observer);
   tmb::AddNodeName<Loki::Conversion<Arg2Type, Subject>::exists>::template doF<
       Arg2>(arg1, _vec_subjects.back());
   _active_get = &(_vec_functors.back());
}

template <class A>
template <class Arg1, class Arg2, class Arg1_param, class Arg2_param>
void tmb::Node<A>::addStrategy(Loki::Functor<A, TYPELIST(Arg1, Arg2)>* functor,
                               Arg1_param arg1,
                               Arg2_param arg2) {
   Loki::Tuple<TYPELIST(Loki::Functor<Arg1>*, Loki::Functor<Arg2>*)> tuple_args;
   typedef typename Loki::Select<
       Loki::TypeTraits<Arg1_param>::isPointer,
       typename Loki::TypeTraits<Arg1_param>::PointeeType,
       Arg1_param>::Result Arg1Type;
   typedef typename Loki::Select<
       Loki::TypeTraits<Arg2_param>::isPointer,
       typename Loki::TypeTraits<Arg2_param>::PointeeType,
       Arg2_param>::Result Arg2Type;

   Loki::Field<0>(tuple_args) =
       Wrap<has_is_a_node<Arg1Type>::Result>::template doF<Arg1, Arg1_param>(
           arg1);

   Loki::Field<1>(tuple_args) =
       Wrap<has_is_a_node<Arg2Type>::Result>::template doF<Arg2, Arg2_param>(
           arg2);

   tmb::FunctorWrapper<A, TYPELIST(Arg1, Arg2)>* wrap =
       new tmb::FunctorWrapper<A, TYPELIST(Arg1, Arg2)>(*functor, tuple_args);

   _vec_functors.push_back(new Loki::Functor<A>(
       wrap, &tmb::FunctorWrapper<A, TYPELIST(Arg1, Arg2)>::return_val));
   _vec_functors_stream.push_back(new Loki::Functor<std::vector<std::string> >(
       wrap, &tmb::FunctorWrapper<A, TYPELIST(Arg1, Arg2)>::args_as_stream));
   _vec_subjects.resize(_vec_subjects.size() + 1);

   _vec_dependencies.push_back(std::vector<unsigned>(2, 0));

   _vec_functor_wrappers.push_back(wrap);

   // Add a new observer
   tmb::NodeObserver<A>* observer1 =
       new tmb::NodeObserver<A>(_vec_functors.size() - 1, 0, this);
   tmb::NodeObserver<A>* observer2 =
       new tmb::NodeObserver<A>(_vec_functors.size() - 1, 1, this);
   // Check if arg1 is a subject or not?
   tmb::AddObserver<Loki::Conversion<Arg1Type, Subject>::exists>::template doF<
       Arg1_param>(arg1, observer1);
   tmb::AddObserver<Loki::Conversion<Arg2Type, Subject>::exists>::template doF<
       Arg2_param>(arg2, observer2);

   tmb::AddNodeName<Loki::Conversion<Arg2Type, Subject>::exists>::template doF<
       Arg1_param>(arg1, _vec_subjects.back());
   tmb::AddNodeName<Loki::Conversion<Arg2Type, Subject>::exists>::template doF<
       Arg2_param>(arg2, _vec_subjects.back());
   _active_get = &(_vec_functors.back());
}

template <class A>
tmb::NodeObserver<A>::NodeObserver(size_t index, size_t key, Node<A>* ptr)
    : _index(index), _key(key), _ptr(ptr) {}

template <class A>
void tmb::NodeObserver<A>::update() {
   _ptr->set_active_strategy(_index, _key);
}

template <class A>
const std::vector<Loki::Functor<A>*>& tmb::Node<A>::get_vec_functors() const {
   return _vec_functors;
}

template <class A>
void tmb::Node<A>::set_active_strategy(size_t index, size_t key) {
   _vec_dependencies[index][key] = 1;
   if (std::accumulate(_vec_dependencies[index].begin(),
                       _vec_dependencies[index].end(),
                       0) == _vec_dependencies[index].size()) {
      _active_get = &(_vec_functors[index]);
      notify();
   }
}

template <class A>
void tmb::Node<A>::reset_dependencies() {
   typedef std::vector<std::vector<unsigned> > dependencies;
   for (dependencies::iterator it = _vec_dependencies.begin();
        it != _vec_dependencies.end();
        ++it) {
      std::fill(it->begin(), it->end(), 0);
   }
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

template <class A>
void tmb::draw_dot_graph(tmb::Node<A>* node) {
   std::ofstream fs;
   fs.open("dependeny_graph.dot");
   fs << "digraph G {" << std::endl;
   std::vector<std::vector<std::string> > functor_args_vals =
       node->val_of_functor_wrappers();
   unsigned count = 0;
   fs << count << "[label=\"" << node->get_name() << " \\n";
   fs << "value: " << node->get() << "\"";
   fs << ",shape=\"rectangle\",style=filled,fillcolor=\"turquoise\"];"
      << "\n";
   for (unsigned i = 0; i < node->vector_of_strings().size(); ++i) {
      std::vector<std::string>& tokens(functor_args_vals[i]);
      for (unsigned j = 0; j < tokens.size(); ++j) {
         ++count;
         fs << count << "[label=\"" << node->vector_of_strings()[i][j]
            << " \\n";
         fs << "value: " << tokens[j] << "\"";
         fs << ",shape=\"rectangle\",style=filled,fillcolor=\"turquoise\"];"
            << "\n";
      }
   }
   count = 0;
   for (unsigned i = 0; i < node->vector_of_strings().size(); ++i) {
      std::vector<std::string>& tokens(functor_args_vals[i]);
      for (unsigned j = 0; j < tokens.size(); ++j) {
         ++count;
         fs << "0->" << count << ";" << std::endl;
      }
   }
   fs << "}";
   fs.close();
}

template <class A>
const std::vector<std::vector<std::string> >& tmb::Node<A>::vector_of_strings()
    const {
   return _vec_subjects;
}

template <class A>
std::vector<std::vector<std::string> > tmb::Node<A>::val_of_functor_wrappers() {
   std::vector<std::vector<std::string> > return_val;
   for (unsigned i = 0; i < _vec_functors_stream.size(); ++i) {
      return_val.push_back((*(_vec_functors_stream[i]))());
   }
   return return_val;
}
#endif
