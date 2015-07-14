#ifndef _FUNCTOR_WRAPPER_H_INCLUDED
#define _FUNCTOR_WRAPPER_H_INCLUDED
#include "FunctorWrapper.decl.h"

#include "Typelist.decl.h"
#include "loki/Functor.h"
#include "loki/HierarchyGenerators.h"
#include "loki/Typelist.h"
#include "loki/TypeTraits.h"
#include <sstream>

namespace tmb {

   ////////////////////////////////////////////////////////////////////////////////
   ///  \class BinderFirstPtr
   ///
   ///  \ingroup FunctorGroup
   ///  Binds the first parameter of a Functor object to a specific value
   ////////////////////////////////////////////////////////////////////////////////

   template <class OriginalFunctor>
   class BinderFirstPtr
       : public Loki::Private::BinderFirstTraits<OriginalFunctor>::Impl {
      typedef typename Loki::Private::BinderFirstTraits<OriginalFunctor>::Impl
          Base;
      typedef typename OriginalFunctor::ResultType ResultType;

      typedef typename OriginalFunctor::Parm1 BoundType;

      typedef typename Loki::Private::BinderFirstBoundTypeStorage<
          typename Loki::Private::BinderFirstTraits<
              OriginalFunctor>::OriginalParm1>::RefOrValue BoundTypeStorage;

      typedef typename OriginalFunctor::Parm2 Parm1;
      typedef typename OriginalFunctor::Parm3 Parm2;
      typedef typename OriginalFunctor::Parm4 Parm3;
      typedef typename OriginalFunctor::Parm5 Parm4;
      typedef typename OriginalFunctor::Parm6 Parm5;
      typedef typename OriginalFunctor::Parm7 Parm6;
      typedef typename OriginalFunctor::Parm8 Parm7;
      typedef typename OriginalFunctor::Parm9 Parm8;
      typedef typename OriginalFunctor::Parm10 Parm9;
      typedef typename OriginalFunctor::Parm11 Parm10;
      typedef typename OriginalFunctor::Parm12 Parm11;
      typedef typename OriginalFunctor::Parm13 Parm12;
      typedef typename OriginalFunctor::Parm14 Parm13;
      typedef typename OriginalFunctor::Parm15 Parm14;
      typedef Loki::EmptyType Parm15;

     public:
      BinderFirstPtr(const OriginalFunctor& fun, const BoundType* bound)
          : f_(fun), b_(bound) {}

      LOKI_DEFINE_CLONE_FUNCTORIMPL(BinderFirstPtr)

#ifdef LOKI_FUNCTORS_ARE_COMPARABLE

      bool operator==(const typename Base::FunctorImplBaseType& rhs) const {
         if (typeid(*this) != typeid(rhs))
            return false;    // cannot be equal
         // if this line gives a compiler error, you are using a function
         // object.
         // you need to implement bool MyFnObj::operator == (const MyFnObj&)
         // const;
         return f_ == ((static_cast<const BinderFirstPtr&>(rhs)).f_) &&
                b_ == ((static_cast<const BinderFirstPtr&>(rhs)).b_);
      }
#endif

      // operator() implementations for up to 15 arguments

      ResultType operator()() { return f_(*b_); }

      ResultType operator()(Parm1 p1) { return f_(*b_, p1); }

      ResultType operator()(Parm1 p1, Parm2 p2) { return f_(*b_, p1, p2); }

      ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3) {
         return f_(*b_, p1, p2, p3);
      }

      ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4) {
         return f_(*b_, p1, p2, p3, p4);
      }

      ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5) {
         return f_(*b_, p1, p2, p3, p4, p5);
      }

      ResultType operator()(
          Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5, Parm6 p6) {
         return f_(*b_, p1, p2, p3, p4, p5, p6);
      }

      ResultType operator()(Parm1 p1,
                            Parm2 p2,
                            Parm3 p3,
                            Parm4 p4,
                            Parm5 p5,
                            Parm6 p6,
                            Parm7 p7) {
         return f_(*b_, p1, p2, p3, p4, p5, p6, p7);
      }

      ResultType operator()(Parm1 p1,
                            Parm2 p2,
                            Parm3 p3,
                            Parm4 p4,
                            Parm5 p5,
                            Parm6 p6,
                            Parm7 p7,
                            Parm8 p8) {
         return f_(*b_, p1, p2, p3, p4, p5, p6, p7, p8);
      }

      ResultType operator()(Parm1 p1,
                            Parm2 p2,
                            Parm3 p3,
                            Parm4 p4,
                            Parm5 p5,
                            Parm6 p6,
                            Parm7 p7,
                            Parm8 p8,
                            Parm9 p9) {
         return f_(*b_, p1, p2, p3, p4, p5, p6, p7, p8, p9);
      }

      ResultType operator()(Parm1 p1,
                            Parm2 p2,
                            Parm3 p3,
                            Parm4 p4,
                            Parm5 p5,
                            Parm6 p6,
                            Parm7 p7,
                            Parm8 p8,
                            Parm9 p9,
                            Parm10 p10) {
         return f_(*b_, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
      }

      ResultType operator()(Parm1 p1,
                            Parm2 p2,
                            Parm3 p3,
                            Parm4 p4,
                            Parm5 p5,
                            Parm6 p6,
                            Parm7 p7,
                            Parm8 p8,
                            Parm9 p9,
                            Parm10 p10,
                            Parm11 p11) {
         return f_(*b_, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
      }

      ResultType operator()(Parm1 p1,
                            Parm2 p2,
                            Parm3 p3,
                            Parm4 p4,
                            Parm5 p5,
                            Parm6 p6,
                            Parm7 p7,
                            Parm8 p8,
                            Parm9 p9,
                            Parm10 p10,
                            Parm11 p11,
                            Parm12 p12) {
         return f_(*b_, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
      }

      ResultType operator()(Parm1 p1,
                            Parm2 p2,
                            Parm3 p3,
                            Parm4 p4,
                            Parm5 p5,
                            Parm6 p6,
                            Parm7 p7,
                            Parm8 p8,
                            Parm9 p9,
                            Parm10 p10,
                            Parm11 p11,
                            Parm12 p12,
                            Parm13 p13) {
         return f_(*b_, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
      }

      ResultType operator()(Parm1 p1,
                            Parm2 p2,
                            Parm3 p3,
                            Parm4 p4,
                            Parm5 p5,
                            Parm6 p6,
                            Parm7 p7,
                            Parm8 p8,
                            Parm9 p9,
                            Parm10 p10,
                            Parm11 p11,
                            Parm12 p12,
                            Parm13 p13,
                            Parm14 p14) {
         return f_(
             *b_, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
      }

     private:
      OriginalFunctor f_;
      const BoundTypeStorage* b_;
   };

   ////////////////////////////////////////////////////////////////////////////////
   ///  Binds the first parameter of a Functor object to a specific value
   ///  \ingroup FunctorGroup
   ////////////////////////////////////////////////////////////////////////////////

   template <class Fctor>
   typename Loki::Private::BinderFirstTraits<Fctor>::BoundFunctorType
       BindFirstPtr(const Fctor& fun, typename Fctor::Parm1* bound) {
      typedef typename Loki::Private::BinderFirstTraits<Fctor>::BoundFunctorType
          Outgoing;

      return Outgoing(std::auto_ptr<typename Outgoing::Impl>(
          new BinderFirstPtr<Fctor>(fun, bound)));
   }

   template <class Fctor>
   typename Loki::Private::BinderFirstTraits<Fctor>::BoundFunctorType
       BindFirstPtr(const Fctor& fun, const typename Fctor::Parm1* bound) {
      typedef typename Loki::Private::BinderFirstTraits<Fctor>::BoundFunctorType
          Outgoing;

      return Outgoing(std::auto_ptr<typename Outgoing::Impl>(
          new BinderFirstPtr<Fctor>(fun, bound)));
   }

   template <class OriginalFunctor>
   class BinderFirstRef
       : public Loki::Private::BinderFirstTraits<OriginalFunctor>::Impl {
      typedef typename Loki::Private::BinderFirstTraits<OriginalFunctor>::Impl
          Base;
      typedef typename OriginalFunctor::ResultType ResultType;

      typedef typename Loki::TypeTraits<
          typename OriginalFunctor::Parm1>::ReferredType BoundType;

      typedef typename Loki::Private::BinderFirstBoundTypeStorage<
          typename Loki::Private::BinderFirstTraits<
              OriginalFunctor>::OriginalParm1>::RefOrValue BoundTypeStorage;

      typedef typename OriginalFunctor::Parm2 Parm1;
      typedef typename OriginalFunctor::Parm3 Parm2;
      typedef typename OriginalFunctor::Parm4 Parm3;
      typedef typename OriginalFunctor::Parm5 Parm4;
      typedef typename OriginalFunctor::Parm6 Parm5;
      typedef typename OriginalFunctor::Parm7 Parm6;
      typedef typename OriginalFunctor::Parm8 Parm7;
      typedef typename OriginalFunctor::Parm9 Parm8;
      typedef typename OriginalFunctor::Parm10 Parm9;
      typedef typename OriginalFunctor::Parm11 Parm10;
      typedef typename OriginalFunctor::Parm12 Parm11;
      typedef typename OriginalFunctor::Parm13 Parm12;
      typedef typename OriginalFunctor::Parm14 Parm13;
      typedef typename OriginalFunctor::Parm15 Parm14;
      typedef Loki::EmptyType Parm15;

     public:
      BinderFirstRef(const OriginalFunctor& fun, BoundType& bound)
          : f_(fun), b_(bound) {}

      LOKI_DEFINE_CLONE_FUNCTORIMPL(BinderFirstRef)

#ifdef LOKI_FUNCTORS_ARE_COMPARABLE

      bool operator==(const typename Base::FunctorImplBaseType& rhs) const {
         if (typeid(*this) != typeid(rhs))
            return false;    // cannot be equal
         // if this line gives a compiler error, you are using a function
         // object.
         // you need to implement bool MyFnObj::operator == (const MyFnObj&)
         // const;
         return f_ == ((static_cast<const BinderFirst&>(rhs)).f_) &&
                b_ == ((static_cast<const BinderFirst&>(rhs)).b_);
      }
#endif

      // operator() implementations for up to 15 arguments

      ResultType operator()() { return f_(b_); }

      ResultType operator()(Parm1 p1) { return f_(b_, p1); }

      ResultType operator()(Parm1 p1, Parm2 p2) { return f_(b_, p1, p2); }

      ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3) {
         return f_(b_, p1, p2, p3);
      }

      ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4) {
         return f_(b_, p1, p2, p3, p4);
      }

      ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5) {
         return f_(b_, p1, p2, p3, p4, p5);
      }

      ResultType operator()(
          Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5, Parm6 p6) {
         return f_(b_, p1, p2, p3, p4, p5, p6);
      }

      ResultType operator()(Parm1 p1,
                            Parm2 p2,
                            Parm3 p3,
                            Parm4 p4,
                            Parm5 p5,
                            Parm6 p6,
                            Parm7 p7) {
         return f_(b_, p1, p2, p3, p4, p5, p6, p7);
      }

      ResultType operator()(Parm1 p1,
                            Parm2 p2,
                            Parm3 p3,
                            Parm4 p4,
                            Parm5 p5,
                            Parm6 p6,
                            Parm7 p7,
                            Parm8 p8) {
         return f_(b_, p1, p2, p3, p4, p5, p6, p7, p8);
      }

      ResultType operator()(Parm1 p1,
                            Parm2 p2,
                            Parm3 p3,
                            Parm4 p4,
                            Parm5 p5,
                            Parm6 p6,
                            Parm7 p7,
                            Parm8 p8,
                            Parm9 p9) {
         return f_(b_, p1, p2, p3, p4, p5, p6, p7, p8, p9);
      }

      ResultType operator()(Parm1 p1,
                            Parm2 p2,
                            Parm3 p3,
                            Parm4 p4,
                            Parm5 p5,
                            Parm6 p6,
                            Parm7 p7,
                            Parm8 p8,
                            Parm9 p9,
                            Parm10 p10) {
         return f_(b_, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
      }

      ResultType operator()(Parm1 p1,
                            Parm2 p2,
                            Parm3 p3,
                            Parm4 p4,
                            Parm5 p5,
                            Parm6 p6,
                            Parm7 p7,
                            Parm8 p8,
                            Parm9 p9,
                            Parm10 p10,
                            Parm11 p11) {
         return f_(b_, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
      }

      ResultType operator()(Parm1 p1,
                            Parm2 p2,
                            Parm3 p3,
                            Parm4 p4,
                            Parm5 p5,
                            Parm6 p6,
                            Parm7 p7,
                            Parm8 p8,
                            Parm9 p9,
                            Parm10 p10,
                            Parm11 p11,
                            Parm12 p12) {
         return f_(b_, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
      }

      ResultType operator()(Parm1 p1,
                            Parm2 p2,
                            Parm3 p3,
                            Parm4 p4,
                            Parm5 p5,
                            Parm6 p6,
                            Parm7 p7,
                            Parm8 p8,
                            Parm9 p9,
                            Parm10 p10,
                            Parm11 p11,
                            Parm12 p12,
                            Parm13 p13) {
         return f_(b_, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
      }

      ResultType operator()(Parm1 p1,
                            Parm2 p2,
                            Parm3 p3,
                            Parm4 p4,
                            Parm5 p5,
                            Parm6 p6,
                            Parm7 p7,
                            Parm8 p8,
                            Parm9 p9,
                            Parm10 p10,
                            Parm11 p11,
                            Parm12 p12,
                            Parm13 p13,
                            Parm14 p14) {
         return f_(
             b_, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
      }

     private:
      OriginalFunctor f_;
      typename Loki::TypeTraits<BoundTypeStorage>::ReferredType& b_;
   };

   template <class Fctor>
   typename Loki::Private::BinderFirstTraits<Fctor>::BoundFunctorType
       BindFirstRef(const Fctor& fun,
                    typename Loki::TypeTraits<
                        typename Fctor::Parm1>::ReferredType& bound) {
      typedef typename Loki::Private::BinderFirstTraits<Fctor>::BoundFunctorType
          Outgoing;

      return Outgoing(std::auto_ptr<typename Outgoing::Impl>(
          new BinderFirstRef<Fctor>(fun, bound)));
   }
   template <class Return, class Tlist, class PropertiesType>
   FunctorWrapper<Return, Tlist, PropertiesType>::FunctorWrapper(
       Loki::Functor<Return, Tlist>& func,
       const Loki::Tuple<typename CreateFunctorPointerList<Tlist>::List>&
           tuple_functors)
       : _func(&func) {
      // TO ensure there are no memory issues we will make a copy of all
      // the pointers in tuple_functors
      CopyFunctorPointersFromTuple<Tlist, 0>::template doF<Tlist>(
          _arg_functors, tuple_functors);
      _func_no_args =
          IterateOverArgs<ListArgs, 0>::doF(*this, *_func, _arg_functors);
   }

   template <class Return, class Tlist, class PropertiesType>
   FunctorWrapper<Return, Tlist, PropertiesType>::FunctorWrapper(
       const FunctorWrapper& obj)
       : _func(obj._func), _func_no_args(obj._func_no_args) {
      CopyFunctorPointersFromTuple<Tlist, 0>::doF(_arg_functors,
                                                  obj._arg_functors);
   }

   template <class Return, class Tlist, class PropertiesType>
   FunctorWrapper<Return, Tlist, PropertiesType>::FunctorWrapper()
       : _func(NULL) {}

   template <class Return, class Tlist, class PropertiesType>
   template <class Head, class Tail, int N>
   Loki::Functor<Return> FunctorWrapper<Return, Tlist, PropertiesType>::
       IterateOverArgs<Loki::Typelist<Head, Tail>, N>::doF(
           FunctorWrapper<ResultType, Tlist, PropertiesType>& obj,
           Loki::Functor<ResultType, Loki::Typelist<Head, Tail> >&
               func_original,
           Loki::Tuple<typename CreateFunctorPointerList<Tlist>::List>&
               arg_functors) {
      typedef typename Loki::Select<
          Loki::TypeTraits<Head>::isConst,
          typename Loki::TypeTraits<Head>::NonConstType,
          Head>::Result VarTypeRefOrValue;
      typedef typename Loki::Select<
          Loki::TypeTraits<VarTypeRefOrValue>::isReference,
          typename Loki::TypeTraits<VarTypeRefOrValue>::ReferredType,
          VarTypeRefOrValue>::Result VarType;
      VarType* val = &(Loki::Field<N>(obj._tuple_args));
      Loki::Functor<ResultType, Tail> newfunc(
          tmb::BindFirstRef(func_original, *val));
      return FunctorWrapper<ResultType, Tlist, PropertiesType>::
          template IterateOverArgs<Tail, N + 1>::doF(
              obj, newfunc, arg_functors);
   }

   template <class Return, class Tlist, class PropertiesType>
   template <class Tail, int N>
   Loki::Functor<Return> FunctorWrapper<Return, Tlist, PropertiesType>::
       IterateOverArgs<Loki::Typelist<Tail, Loki::NullType>, N>::doF(
           FunctorWrapper<ResultType, Tlist, PropertiesType>& obj,
           Loki::Functor<ResultType, Loki::Typelist<Tail, Loki::NullType> >&
               func_original,
           Loki::Tuple<typename CreateFunctorPointerList<Tlist>::List>&) {
      typedef typename Loki::Select<
          Loki::TypeTraits<Tail>::isConst,
          typename Loki::TypeTraits<Tail>::NonConstType,
          Tail>::Result VarTypeRefOrValue;
      typedef typename Loki::Select<
          Loki::TypeTraits<VarTypeRefOrValue>::isReference,
          typename Loki::TypeTraits<VarTypeRefOrValue>::ReferredType,
          VarTypeRefOrValue>::Result VarType;
      VarType* val = &(Loki::Field<N>(obj._tuple_args));
      Loki::Functor<ResultType> newfunc(tmb::BindFirstRef(func_original, *val));
      return newfunc;
   }

   template <class Return, class Tlist, class PropertiesType>
   template <int N>
   Loki::Functor<Return> FunctorWrapper<Return, Tlist, PropertiesType>::
       IterateOverArgs<Loki::NullType, N>::doF(
           FunctorWrapper<Return, Tlist, PropertiesType>&,
           Loki::Functor<Return>& func_original,
           Loki::Tuple<typename CreateFunctorPointerList<Tlist>::List>&) {
      return func_original;
   }

   template <class Return, class Tlist, class PropertiesType>
   Return tmb::FunctorWrapper<Return, Tlist, PropertiesType>::return_val() {
      FunctorWrapper<Return, Tlist, PropertiesType>::LoopOverTuple<0>::doF(
          *this);
      return _func_no_args();
   }

   template <class Return, class Tlist, class PropertiesType>
   FunctorWrapper<Return, Tlist, PropertiesType>::operator ResultType() {
      return return_val();
   }

   template <class Return, class Tlist, class PropertiesType>
   template <int N, class flag>
   void FunctorWrapper<Return, Tlist, PropertiesType>::LoopOverTuple<
       N,
       flag>::doF(FunctorWrapper<Return, Tlist, PropertiesType>& obj) {
      Loki::Field<N>(obj._tuple_args) = (*Loki::Field<N>(obj._arg_functors))();
      LoopOverTuple<
          N + 1,
          typename Loki::Select<N + 1 < Loki::TL::Length<Tlist>::value,
                                Loki::Int2Type<1>,
                                Loki::Int2Type<0> >::Result>::doF(obj);
   }

   template <class Return, class Tlist, class PropertiesType>
   template <int N>
   void FunctorWrapper<Return, Tlist, PropertiesType>::
       LoopOverTuple<N, Loki::Int2Type<0> >::doF(
           FunctorWrapper<Return, Tlist, PropertiesType>&) {}

   template <class A>
   A return_val(A val) {
      return val;
   }

   template <class A>
   Loki::Functor<A>* wrap_var_with_functor(A val) {
      typedef typename Loki::TypeTraits<A>::ReferredType Type;
      Type* ref_val;
      // Check if A is a reference type or not. If not then create
      // a variable of type A on the stack and then copy the value
      // of val into it
      if (!Loki::TypeTraits<A>::isReference) {
         ref_val = new Type(val);
      } else {
         ref_val = &val;
      }
      Loki::Functor<A, LOKI_TYPELIST_1(A)> func(&return_val<A>);
      return new Loki::Functor<A>(tmb::BindFirstRef(func, *ref_val));
   }

   template <class Return, class Tlist, class PropertiesType>
   FunctorWrapper<Return, Tlist, PropertiesType>::~FunctorWrapper() {
      tmb::DeleteFunctorPointersFromTuple<Tlist, 0>::template doF<Tlist>(
          _arg_functors);
   }

   template <class List, int N>
   struct PutTupleFieldsInStream {
      template <class A>
      static void doF(Loki::Tuple<A>& tuple, std::vector<std::string>& stream);
   };

   template <class Head, class Tail, int N>
   struct PutTupleFieldsInStream<Loki::Typelist<Head, Tail>, N> {
      template <class A>
      static void doF(Loki::Tuple<A>& tuple, std::vector<std::string>& stream) {
         std::stringstream val_stream;
         val_stream << Loki::Field<N>(tuple);
         stream.push_back(val_stream.str());
         PutTupleFieldsInStream<Tail, N + 1>::doF(tuple, stream);
      }
   };

   template <class Tail, int N>
   struct PutTupleFieldsInStream<Loki::Typelist<Tail, Loki::NullType>, N> {
      template <class A>
      static void doF(Loki::Tuple<A>& tuple, std::vector<std::string>& stream) {
         std::stringstream val_stream;
         val_stream << Loki::Field<N>(tuple);
         stream.push_back(val_stream.str());
      }
   };

   template <class Return, class Tlist, class PropertiesType>
   std::vector<std::string>
       tmb::FunctorWrapper<Return, Tlist, PropertiesType>::args_as_stream() {
      std::vector<std::string> return_stream;
      PutTupleFieldsInStream<Tlist, 0>::doF(_tuple_args, return_stream);
      return return_stream;
   }
}

#endif
