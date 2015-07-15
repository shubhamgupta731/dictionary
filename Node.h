#ifndef _NODE_H_INCLUDED
#define _NODE_H_INCLUDED
#include "Node.decl.h"
#include "FunctorWrapper.h"
#include <iostream>
#include <fstream>
#include <numeric>
#include <iterator>
#include "loki/Typelist.h"

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
tmb::Node<A>::Node(std::string name) {
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
   _active_get = &_get_solved;
}

template <class A>
tmb::Node<A>::Node(const tmb::Node<A>& copy_from)
    : BaseNodeFeatures(copy_from), _vec_functors(copy_from._vec_functors) {
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
   if (copy_from._active_get == &(copy_from._get_solved))
      _active_get = &_get_solved;
   else
      _active_get = copy_from._active_get;
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
      static void doF(A arg1,
                      std::vector<std::string>& vec_str,
                      std::vector<BaseNodeFeatures*>& depends_on) {
         vec_str.push_back(arg1->get_name());
         depends_on.push_back(arg1);
      }
   };

   template <>
   struct AddNodeName<false> {
      template <class A>
      static void doF(A,
                      std::vector<std::string>& vec_str,
                      std::vector<BaseNodeFeatures*>&) {
         vec_str.push_back("uknown");
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

      template <class List, int N>
      struct AddArgObserverAndName;

      template <class Head, class Tail, int N>
      struct AddArgObserverAndName<Loki::Typelist<Head, Tail>, N> {
         template <class A, class TupleArgs>
         static void doF(Node<A>* node_ptr,
                         Loki::Tuple<TupleArgs>& tuple_of_args,
                         unsigned index,
                         std::vector<std::string>& subject_index,
                         std::vector<BaseNodeFeatures*>& depends_on) {
            typedef typename Loki::TL::TypeAt<TupleArgs, N>::Result ArgTypeRaw;
            typedef typename Loki::Select<
                Loki::TypeTraits<ArgTypeRaw>::isPointer,
                typename Loki::TypeTraits<ArgTypeRaw>::PointeeType,
                ArgTypeRaw>::Result ArgType;
            ArgTypeRaw& arg = Loki::Field<N>(tuple_of_args);
            tmb::NodeObserver<A>* observer =
                new tmb::NodeObserver<A>(index, N, node_ptr);
            tmb::AddObserver<Loki::Conversion<ArgType, Subject>::exists>::
                template doF<ArgTypeRaw>(arg, observer);
            tmb::AddNodeName<Loki::Conversion<ArgType, Subject>::exists>::
                template doF<ArgTypeRaw>(arg, subject_index, depends_on);
            AddArgObserverAndName<Tail, N + 1>::doF(
                node_ptr, tuple_of_args, index, subject_index, depends_on);
         }
      };

      template <class Tail, int N>
      struct AddArgObserverAndName<Loki::Typelist<Tail, Loki::NullType>, N> {
         template <class A, class TupleArgs>
         static void doF(Node<A>* node_ptr,
                         Loki::Tuple<TupleArgs>& tuple_of_args,
                         unsigned index,
                         std::vector<std::string>& subject_index,
                         std::vector<BaseNodeFeatures*>& depends_on) {
            typedef typename Loki::TL::TypeAt<TupleArgs, N>::Result ArgTypeRaw;
            typedef typename Loki::Select<
                Loki::TypeTraits<ArgTypeRaw>::isPointer,
                typename Loki::TypeTraits<ArgTypeRaw>::PointeeType,
                ArgTypeRaw>::Result ArgType;
            ArgTypeRaw& arg = Loki::Field<N>(tuple_of_args);
            tmb::NodeObserver<A>* observer =
                new tmb::NodeObserver<A>(index, N, node_ptr);
            tmb::AddObserver<Loki::Conversion<ArgType, Subject>::exists>::
                template doF<ArgTypeRaw>(arg, observer);
            tmb::AddNodeName<Loki::Conversion<ArgType, Subject>::exists>::
                template doF<ArgTypeRaw>(arg, subject_index, depends_on);
         }
      };
   }
}

template <class A>
template <class ArgList, class TupleArgs>
void tmb::Node<A>::addStrategyMultiple(Loki::Functor<A, ArgList>* functor,
                                       TupleArgs& tuple_of_args,
                                       std::string& dependency_name) {
   Loki::Tuple<typename tmb::Private::CreateFunctorPointerListFromArgs<
       ArgList>::Result> tuple_args;
   tmb::Private::SetTupleArgs<ArgList, 0>::doF(tuple_of_args, tuple_args);

   tmb::FunctorWrapper<A, ArgList>* wrap =
       new tmb::FunctorWrapper<A, ArgList>(*functor, tuple_args);
   _vec_functors.push_back(new Loki::Functor<A>(
       wrap, &tmb::FunctorWrapper<A, ArgList>::return_val));
   _vec_functors_stream.push_back(new Loki::Functor<std::vector<std::string> >(
       wrap, &tmb::FunctorWrapper<A, ArgList>::args_as_stream));
   _vec_subjects.resize(_vec_subjects.size() + 1);

   _vec_dependencies.push_back(
       std::vector<unsigned>(Loki::TL::Length<ArgList>::value, 0));

   _vec_functor_wrappers.push_back(wrap);
   tmb::Private::AddArgObserverAndName<ArgList, 0>::doF(this,
                                                        tuple_of_args,
                                                        _vec_functors.size() -
                                                            1,
                                                        _vec_subjects.back(),
                                                        _depends_on);
   _active_get = &(_vec_functors.back());
   _vec_dependency_name.push_back(dependency_name);
}

template <class A>
template <class Arg1, class Arg2>
void tmb::Node<A>::addStrategy(Loki::Functor<A, TYPELIST(Arg1)>* functor,
                               Arg2 arg1,
                               std::string dependency_name) {
   Loki::Tuple<TYPELIST(Arg2)> tuple_of_args;
   Loki::Field<0>(tuple_of_args) = arg1;
   addStrategyMultiple(functor, tuple_of_args, dependency_name);
}

template <class A>
template <class Arg1, class Arg2, class Arg1_param, class Arg2_param>
void tmb::Node<A>::addStrategy(Loki::Functor<A, TYPELIST(Arg1, Arg2)>* functor,
                               Arg1_param arg1,
                               Arg2_param arg2,
                               std::string dependency_name) {
   Loki::Tuple<TYPELIST(Arg1_param, Arg2_param)> tuple_of_args;
   Loki::Field<0>(tuple_of_args) = arg1;
   Loki::Field<1>(tuple_of_args) = arg2;
   addStrategyMultiple(functor, tuple_of_args, dependency_name);
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
std::string tmb::Node<A>::get_val_as_string() {
   std::stringstream str;
   str << get();
   return str.str();
}
#endif
