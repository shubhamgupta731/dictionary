#ifndef _FUNCTOR_WRAPPER_H_INCLUDED
#define _FUNCTOR_WRAPPER_H_INCLUDED
#include "FunctorWrapper.decl.h"

#include "Typelist.decl.h"
#include "loki/Functor.h"
#include "loki/HierarchyGenerators.h"
#include "CustomLokiFunctions.decl.h"

namespace tmb {
   template <class Return, class Tlist, class PropertiesType>
   FunctorWrapper<Return, Tlist, PropertiesType>::FunctorWrapper(
       Loki::Functor<Return, Tlist> &func,
       const Loki::Tuple<typename CreateFunctorPointerList<Tlist>::List> &
           tuple_functors)
       : _func(&func) {
      // TO ensure there are no memory issues we will make a copy of all
      // the pointers in tuple_functors
      CopyFunctorPointersFromTuple<Tlist, 0>::doF(_arg_functors,
                                                  tuple_functors);
      _func_no_args =
          IterateOverArgs<ListArgs, 0>::doF(*this, *_func, _arg_functors);
   }

   template <class Return, class Tlist, class PropertiesType>
   FunctorWrapper<Return, Tlist, PropertiesType>::FunctorWrapper(
       const FunctorWrapper &obj)
       : _func(obj._func), _func_no_args(obj._func_no_args) {
      CopyFunctorPointersFromTuple<Tlist, 0>::doF(_arg_functors,
                                                  obj._arg_functors);
   }

   template <class Return, class Tlist, class PropertiesType>
   FunctorWrapper<Return, Tlist, PropertiesType>::FunctorWrapper()
       : _func(NULL) {}

   template <class Return, class Tlist, class PropertiesType>
   template <class Head, class Tail, int N>
   Loki::Functor<Return> &FunctorWrapper<Return, Tlist, PropertiesType>::
       IterateOverArgs<Loki::Typelist<Head, Tail>, N>::doF(
           FunctorWrapper<ResultType, Tlist, PropertiesType> &obj,
           Loki::Functor<ResultType, Loki::Typelist<Head, Tail> > &
               func_original,
           Loki::Tuple<typename CreateFunctorPointerList<Tlist>::List> &
               tuple_functors) {
      typedef
          typename Loki::Select<Loki::TypeTraits<Head>::isConst,
                                typename Loki::TypeTraits<Head>::NonConstType,
                                Head>::Result VarTypeRefOrValue;
      typedef typename Loki::Select<
          Loki::TypeTraits<VarTypeRefOrValue>::isReference,
          typename Loki::TypeTraits<VarTypeRefOrValue>::ReferredType,
          VarTypeRefOrValue>::Result VarType;
      VarType *val = &(Loki::Field<N>(obj._tuple_args));
      Loki::Functor<ResultType, Tail> newfunc(
          tmb::BindFirstRef(func_original, *val));
      return FunctorWrapper<ResultType, Tlist, PropertiesType>::
          template IterateOverArgs<Tail, N + 1>::doF(obj, newfunc);
   }

   template <class Return, class Tlist, class PropertiesType>
   template <class Tail, int N>
   Loki::Functor<Return> &FunctorWrapper<Return, Tlist, PropertiesType>::
       IterateOverArgs<Loki::Typelist<Tail, Loki::NullType>, N>::doF(
           FunctorWrapper<ResultType, Tlist, PropertiesType> &obj,
           Loki::Functor<ResultType, Loki::Typelist<Tail, Loki::NullType> > &
               func_original,
           Loki::Tuple<typename CreateFunctorPointerList<Tlist>::List> &
               tuple_functors) {
      typedef
          typename Loki::Select<Loki::TypeTraits<Tail>::isConst,
                                typename Loki::TypeTraits<Tail>::NonConstType,
                                Tail>::Result VarTypeRefOrValue;
      typedef typename Loki::Select<
          Loki::TypeTraits<VarTypeRefOrValue>::isReference,
          typename Loki::TypeTraits<VarTypeRefOrValue>::ReferredType,
          VarTypeRefOrValue>::Result VarType;
      VarType *val = &(Loki::Field<N>(obj._tuple_args));
      Loki::Functor<ResultType> newfunc(tmb::BindFirstRef(func_original, *val));
      return newfunc;
   }

   template <class Return, class Tlist, class PropertiesType>
   template <int N>
   Loki::Functor<Return> &FunctorWrapper<Return, Tlist, PropertiesType>::
       IterateOverArgs<Loki::NullType, N>::doF(
           FunctorWrapper<Return, Tlist, PropertiesType> &obj,
           Loki::Functor<Return> &func_original,
           Loki::Tuple<typename CreateFunctorPointerList<Tlist>::List> &
               tuple_functors) {
      return func_original;
   }
}

#endif
