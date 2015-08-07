#ifndef _NODE_H_INCLUDED
#define _NODE_H_INCLUDED
#include "Node.decl.h"
#include "FunctorWrapper.h"
#include <iostream>
#include <fstream>
#include <numeric>
#include <iterator>
#include "loki/Typelist.h"
#include <cstring>
#include <cassert>

template <class A>
A& tmb::Node<A>::get() {
   return *(_val_ptr->_val);
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
   return &get();
}

template <class A>
const A* tmb::Node<A>::get_const_pointer() {
   return get_pointer();
}

template <class A>
template <class B>
A& tmb::NodeSetAttributes<A>::set(B val) {
   *_val = val;
#ifdef DEBUG
   _value_set_using = "using set";
#endif
   notify();
   return *_val;
}

template <class A>
template <class B>
A& tmb::Node<A>::set(B val) {
   return _set_ptr->set(val);
}

template <class A>
tmb::NodeSetAttributes<A>::NodeSetAttributes(std::string name) {
   memset(_vec_functors, 0, 4);
#ifdef DEBUG
   _name = name;
   _value_set_using = "using set";
#endif
}

template <class A>
tmb::Node<A>::Node(std::string name) {
   _val_ptr = new NodeVal<A>();
   _set_ptr = new NodeSetAttributes<A>(name);
   _set_ptr->set_pointer(_val_ptr->_val);
}

template <class A>
tmb::NodeSetAttributes<A>::NodeSetAttributes(
    const tmb::NodeSetAttributes<A>& copy_from)
    : BaseNodeFeatures(copy_from) {
   memcpy(_vec_functors, copy_from._vec_functors, _number_of_strategies);
#ifdef DEBUG
   _name = copy_from._name;
#endif
}

template <class A>
tmb::Node<A>::Node(const tmb::Node<A>& copy_from) {
   _val_ptr = new NodeVal<A>(*(copy_from._val_ptr));
   _set_ptr = new NodeSetAttributes<A>(*(copy_from._set_ptr));
   _set_ptr->set_pointer(_val_ptr->_val);
}

template <class A>
tmb::NodeVal<A>::NodeVal(const tmb::NodeVal<A>& copy_from) {
   _val = new A();
   *_val = *(copy_from._val);
}

template <class A>
tmb::NodeVal<A>::NodeVal() {
   _val = new A();
}

template <class A>
tmb::Node<A>::~Node() {}

template <class A>
tmb::NodeSetAttributes<A>::~NodeSetAttributes() {
   delete _val;
   for (unsigned i = 0; i < _number_of_strategies; ++i)
      delete _vec_functors[i];
#ifdef DEBUG
   for (typename std::vector<
            Loki::Functor<std::vector<std::string> >*>::iterator it =
            _vec_functors_stream.begin();
        it != _vec_functors_stream.end();
        ++it)
      delete (*it);
   for (std::vector<tmb::BaseFunctorWrapper*>::iterator it =
            _vec_functor_wrappers->begin();
        it != _vec_functor_wrappers->end();
        ++it)
      delete (*it);
#endif
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
         // arg2->attachObserver(arg1);
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
      static void doF(A arg1,
                      std::vector<std::string>& vec_str,
                      std::vector<BaseNodeFeatures*>& depends_on) {
         vec_str.push_back(arg1->get_name());
         depends_on.push_back(arg1->get_observable());
      }
   };

   template <>
   struct AddNodeName<false> {
      template <class A>
      static void doF(A,
                      std::vector<std::string>& vec_str,
                      std::vector<BaseNodeFeatures*>& depends_on) {
         vec_str.push_back("uknown");
         depends_on.push_back(NULL);
      }
   };
}

namespace tmb {
   namespace Private {
      template <class List>
      struct CreateFunctorPointerListFromArgs;

      template <class Head, class Tail>
      struct CreateFunctorPointerListFromArgs<Loki::Typelist<Head, Tail> > {
         typedef Loki::Typelist<
             Loki::Functor<Head>*,
             typename CreateFunctorPointerListFromArgs<Tail>::Result> Result;
      };

      template <class Tail>
      struct CreateFunctorPointerListFromArgs<
          Loki::Typelist<Tail, Loki::NullType> > {
         typedef Loki::Typelist<Loki::Functor<Tail>*, Loki::NullType> Result;
      };

      template <class List, int N>
      struct SetTupleArgs;

      template <class Head, class Tail, int N>
      struct SetTupleArgs<Loki::Typelist<Head, Tail>, N> {
         template <class TupleArgsParams, class TupleArgs>
         static void doF(Loki::Tuple<TupleArgsParams>& tuple_of_args,
                         Loki::Tuple<TupleArgs>& tuple_args) {
            typedef typename Loki::TL::TypeAt<TupleArgsParams, N>::Result
                ArgTypeRaw;
            typedef typename Loki::Select<
                Loki::TypeTraits<ArgTypeRaw>::isPointer,
                typename Loki::TypeTraits<ArgTypeRaw>::PointeeType,
                ArgTypeRaw>::Result ArgType;
            Loki::Field<N>(tuple_args) =
                Wrap<has_is_a_node<ArgType>::Result>::template doF<Head>(
                    Loki::Field<N>(tuple_of_args));
            SetTupleArgs<Tail, N + 1>::doF(tuple_of_args, tuple_args);
         }
      };

      template <class Tail, int N>
      struct SetTupleArgs<Loki::Typelist<Tail, Loki::NullType>, N> {
         template <class TupleArgsParams, class TupleArgs>
         static void doF(Loki::Tuple<TupleArgsParams>& tuple_of_args,
                         Loki::Tuple<TupleArgs>& tuple_args) {
            typedef typename Loki::TL::TypeAt<TupleArgsParams, N>::Result
                ArgTypeRaw;
            typedef typename Loki::Select<
                Loki::TypeTraits<ArgTypeRaw>::isPointer,
                typename Loki::TypeTraits<ArgTypeRaw>::PointeeType,
                ArgTypeRaw>::Result ArgType;
            Loki::Field<N>(tuple_args) =
                Wrap<has_is_a_node<ArgType>::Result>::template doF<Tail>(
                    Loki::Field<N>(tuple_of_args));
         }
      };

      template <class A, int N, bool flag>
      struct CreateObserver {
         template <class Ptr, int I>
         static Observer* doF(Ptr*, int index) {
            std::cerr << "Recursion depth of 256 reached while searching for "
                      << index << std::endl;
            assert(false);
         }
      };

      template <class A, int N>
      struct CreateObserver<A, N, true> {
         template <class Ptr, int I>
         static Observer* doF(Ptr* ptr, int index) {
            if (index == I) {
               return new tmb::NodeObserver<A, I, N>(ptr);
            } else
               return CreateObserver<A, N, (I + 1 < 4)>::template doF<Ptr,
                                                                      I + 1>(
                   ptr, index);
         }
      };

      template <class List, int N>
      struct AddArgObserverAndName;

      template <class Head, class Tail, int N>
      struct AddArgObserverAndName<Loki::Typelist<Head, Tail>, N> {
         template <class A, class TupleArgs>
         static void doF(NodeSetAttributes<A>* node_ptr,
                         Loki::Tuple<TupleArgs>& tuple_of_args,
                         unsigned index
#ifdef DEBUG
                         ,
                         std::vector<std::string>& subject_index,
                         std::vector<BaseNodeFeatures*>& depends_on
#endif
                         ) {
            typedef typename Loki::TL::TypeAt<TupleArgs, N>::Result ArgTypeRaw;
            typedef typename Loki::Select<
                Loki::TypeTraits<ArgTypeRaw>::isPointer,
                typename Loki::TypeTraits<ArgTypeRaw>::PointeeType,
                ArgTypeRaw>::Result ArgType;
            ArgTypeRaw& arg = Loki::Field<N>(tuple_of_args);
            Observer* observer =
                CreateObserver<A, N, true>::template doF<NodeSetAttributes<A>,
                                                         0>(node_ptr, index);
            tmb::AddObserver<Loki::Conversion<ArgType, Subject>::exists ||
                             has_is_a_node<ArgType>::Result>::
                template doF<ArgTypeRaw>(arg, observer);
#ifdef DEBUG
            tmb::AddNodeName<Loki::Conversion<ArgType, Subject>::exists ||
                             has_is_a_node<ArgType>::Result>::
                template doF<ArgTypeRaw>(arg, subject_index, depends_on);
#endif
            AddArgObserverAndName<Tail, N + 1>::doF(node_ptr,
                                                    tuple_of_args,
                                                    index
#ifdef DEBUG
                                                    ,
                                                    subject_index,
                                                    depends_on
#endif
                                                    );
         }
      };

      template <class Tail, int N>
      struct AddArgObserverAndName<Loki::Typelist<Tail, Loki::NullType>, N> {
         template <class A, class TupleArgs>
         static void doF(NodeSetAttributes<A>* node_ptr,
                         Loki::Tuple<TupleArgs>& tuple_of_args,
                         unsigned index
#ifdef DEBUG
                         ,
                         std::vector<std::string>& subject_index,
                         std::vector<BaseNodeFeatures*>& depends_on
#endif
                         ) {
            typedef typename Loki::TL::TypeAt<TupleArgs, N>::Result ArgTypeRaw;
            typedef typename Loki::Select<
                Loki::TypeTraits<ArgTypeRaw>::isPointer,
                typename Loki::TypeTraits<ArgTypeRaw>::PointeeType,
                ArgTypeRaw>::Result ArgType;
            ArgTypeRaw& arg = Loki::Field<N>(tuple_of_args);
            Observer* observer =
                CreateObserver<A, N, true>::template doF<NodeSetAttributes<A>,
                                                         0>(node_ptr, index);
            tmb::AddObserver<Loki::Conversion<ArgType, Subject>::exists ||
                             has_is_a_node<ArgType>::Result>::
                template doF<ArgTypeRaw>(arg, observer);
#ifdef DEBUG
            tmb::AddNodeName<Loki::Conversion<ArgType, Subject>::exists ||
                             has_is_a_node<ArgType>::Result>::
                template doF<ArgTypeRaw>(arg, subject_index, depends_on);
#endif
         }
      };
   }
}

template <class A>
template <class ArgList, class TupleArgs>
void tmb::NodeSetAttributes<A>::addStrategyMultiple(
    Loki::Functor<A, ArgList>* functor,
    TupleArgs& tuple_of_args,
    std::string& dependency_name) {
   Loki::Tuple<typename tmb::Private::CreateFunctorPointerListFromArgs<
       ArgList>::Result> tuple_args;
   tmb::Private::SetTupleArgs<ArgList, 0>::doF(tuple_of_args, tuple_args);

   tmb::FunctorWrapper<A, ArgList>* wrap =
       new tmb::FunctorWrapper<A, ArgList>(*functor, tuple_args, false);
   _vec_functors[_number_of_strategies] =
       new Loki::Functor<A>(wrap, &tmb::FunctorWrapper<A, ArgList>::return_val);
   _vec_functor_wrappers->push_back(wrap);
   ++_number_of_strategies;
#ifdef DEBUG
   _vec_functors_stream.push_back(new Loki::Functor<std::vector<std::string> >(
       wrap, &tmb::FunctorWrapper<A, ArgList>::args_as_stream));
   static std::vector<std::string> empty_vec_string;
   static std::vector<tmb::BaseNodeFeatures*> empty_base_features;
   _vec_subjects.push_back(empty_vec_string);
   _depends_on.push_back(empty_base_features);

   _vec_subjects.back().reserve(Loki::TL::Length<ArgList>::value);
   _depends_on.back().reserve(Loki::TL::Length<ArgList>::value);
   _vec_functor_wrappers->push_back(wrap);
#endif

   //_vec_dependencies.push_back(
   //    std::vector<unsigned>(Loki::TL::Length<ArgList>::value, 0));
   char res = 1;
   for (unsigned i = 1; i < Loki::TL::Length<ArgList>::value; ++i) {
      res <<= 1;
      res += 1;
   }

   _vec_dependencies[_number_of_strategies - 1] = res;
   _vec_dependencies_max[_number_of_strategies - 1] = res;
   tmb::Private::AddArgObserverAndName<ArgList, 0>::doF(
       this,
       tuple_of_args,
       _number_of_strategies - 1
#ifdef DEBUG
       ,
       _vec_subjects.back(),
       _depends_on.back()
#endif
           );
   *_val = (*(_vec_functors[_number_of_strategies - 1]))();
#ifdef DEBUG
   _vec_dependency_name.push_back(dependency_name);
#endif
}

template <class A>
template <class Arg1, class Arg2>
void tmb::Node<A>::addStrategy(Loki::Functor<A, TYPELIST(Arg1)>* functor,
                               Arg2 arg1,
                               std::string dependency_name) {
   _set_ptr->addStrategy(functor, arg1, dependency_name);
}

template <class A>
template <class Arg1, class Arg2, class Arg1_param, class Arg2_param>
void tmb::Node<A>::addStrategy(Loki::Functor<A, TYPELIST(Arg1, Arg2)>* functor,
                               Arg1_param arg1,
                               Arg2_param arg2,
                               std::string dependency_name) {
   _set_ptr->addStrategy(functor, arg1, arg2, dependency_name);
}

template <class A>
tmb::NodeSetAttributes<A>* tmb::Node<A>::get_observable() {
   return _set_ptr;
}

template <class A>
template <class Arg1, class Arg2>
void tmb::NodeSetAttributes<A>::addStrategy(
    Loki::Functor<A, TYPELIST(Arg1)>* functor,
    Arg2 arg1,
    std::string dependency_name) {
   Loki::Tuple<TYPELIST(Arg2)> tuple_of_args;
   Loki::Field<0>(tuple_of_args) = arg1;
   addStrategyMultiple(functor, tuple_of_args, dependency_name);
}

template <class A>
template <class Arg1, class Arg2, class Arg1_param, class Arg2_param>
void tmb::NodeSetAttributes<A>::addStrategy(
    Loki::Functor<A, TYPELIST(Arg1, Arg2)>* functor,
    Arg1_param arg1,
    Arg2_param arg2,
    std::string dependency_name) {
   Loki::Tuple<TYPELIST(Arg1_param, Arg2_param)> tuple_of_args;
   Loki::Field<0>(tuple_of_args) = arg1;
   Loki::Field<1>(tuple_of_args) = arg2;
   addStrategyMultiple(functor, tuple_of_args, dependency_name);
}

template <class A, char Index, char Key>
tmb::NodeObserver<A, Index, Key>::NodeObserver(NodeSetAttributes<A>* ptr)
    : _ptr(ptr) {}

template <class A, char Index, char Key>
tmb::NodeObserver<A, Index, Key>::NodeObserver(
    const tmb::NodeObserver<A, Index, Key>& copy_from)
    : _ptr(copy_from._ptr) {}

template <class A, char Index, char Key>
void tmb::NodeObserver<A, Index, Key>::copy(
    const tmb::NodeObserver<A, Index, Key>& copy_from) {
   _ptr = copy_from._ptr;
}

template <class A, char Index, char Key>
void tmb::NodeObserver<A, Index, Key>::update() {
   _ptr->template set_active_strategy<Index, Key>();
}

template <class A>
const Loki::Functor<A>** tmb::NodeSetAttributes<A>::get_vec_functors() const {
   return _vec_functors;
}

template <class A>
template <unsigned char Index, unsigned char Key>
void tmb::NodeSetAttributes<A>::set_active_strategy() {
   unsigned char old_res = _vec_dependencies[Index];
   unsigned char res = _vec_dependencies[Index] &= (old_res ^ 1 << Key);
   if (res == 0) {
#ifdef DEBUG
      _value_set_using = _vec_dependency_name[Index];
#endif
      *_val = (*(_vec_functors[Index]))();
      notify();
   }
}

template <class A>
tmb::Node<A>::operator A() {
   return get();
}

template <class A>
Loki::Functor<A&>& tmb::Node<A>::get_get_func() {
   return *(new Loki::Functor<A&>(this, &tmb::Node<A>::get));
}

template <class A>
Loki::Functor<A>& tmb::Node<A>::get_copy_func() {
   return *(new Loki::Functor<A>(this, &tmb::Node<A>::get_copy));
}

template <class A>
Loki::Functor<const A&>& tmb::Node<A>::get_const_ref_func() {
   return *(new Loki::Functor<A>(this, &tmb::Node<A>::get_copy));
}

template <class A>
Loki::Functor<A*>& tmb::Node<A>::get_pointer_func() {
   return *(new Loki::Functor<A*>(this, &tmb::Node<A>::get_pointer));
}

template <class A>
Loki::Functor<const A*>& tmb::Node<A>::get_const_pointer_func() {
   return *(
       new Loki::Functor<const A*>(this, &tmb::Node<A>::get_const_pointer));
}

template <class A>
void tmb::draw_dot_graph(tmb::Node<A>* node, unsigned levels) {
   std::ofstream fs;
   std::vector<std::string> dictionary_of_nodes_added;
   fs.open("dependency_graph.dot");
   fs << "digraph G {" << std::endl;
   draw_nodes(fs, node, levels, 0, dictionary_of_nodes_added);
   fs << "}" << std::endl;
   fs.close();
}

template <class A>
std::string tmb::NodeSetAttributes<A>::get_val_as_string() {
   std::stringstream str;
   str << *_val;
   return str.str();
}

template <class A>
void tmb::NodeSetAttributes<A>::set_pointer(A* new_ptr) {
   _val = new_ptr;
}

template <class A>
void tmb::Node<A>::set_pointer(A* new_ptr) {
   delete _val_ptr->_val;
   _val_ptr->_val = new_ptr;
   _set_ptr->set_pointer(new_ptr);
}

template <class A>
void tmb::Node<A>::attach(Observer* obs) {
   _set_ptr->attach(obs);
}

#ifdef DEBUG
template <class A>
std::string& tmb::Node<A>::get_name() {
   return _set_ptr->get_name();
}

template <class A>
void tmb::draw_nodes(std::ofstream& fs,
                     tmb::Node<A>* node,
                     unsigned levels,
                     unsigned count,
                     std::vector<std::string>& dictionary_of_nodes_added) {
   tmb::draw_nodes(
       fs, node->get_observable(), levels, count, dictionary_of_nodes_added);
}
#endif

#endif
