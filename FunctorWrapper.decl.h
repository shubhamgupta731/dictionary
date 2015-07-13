#ifndef FUNCTOR_WRAPPER_DECL_H_INCLUDED
#define FUNCTOR_WRAPPER_DECL_H_INCLUDED

#include "loki/Functor.h"
#include "loki/HierarchyGenerators.h"

namespace tmb {
   template <class List>
   struct RemoveReferenceAndConst;
   ;
   template <class Head, class Tail>
   struct RemoveReferenceAndConst<Loki::Typelist<Head, Tail> > {
      typedef Head Type;
      typedef Loki::Typelist<
          typename Loki::TypeTraits<
              typename Loki::TypeTraits<Type>::ReferredType>::NonConstType,
          typename RemoveReferenceAndConst<Tail>::List> List;
   };
   template <>
   struct RemoveReferenceAndConst<Loki::NullType> {
      typedef Loki::NullType List;
   };
   /**
    * @class      : CreateFunctorPointerList
    * @brief This Class takes a typelist as a template parameter
    *        and creates a typelist of Functor pointers with each
    *        type present in the typelist using recursive template
    *        meta-programming
    */
   template <class List>
   struct CreateFunctorPointerList;

   /**
    * @class      : CreateFunctorPointerList
    * @brief Create a typelist with the Functor pointer of the head as then
    *        head of the new typelist where as the tail is the used to create
    *        another typelist with the similar pattern.
    */
   template <class Head, class Tail>
   struct CreateFunctorPointerList<Loki::Typelist<Head, Tail> > {
      typedef Head Type;
      typedef Loki::Typelist<Loki::Functor<Type> *,
                             typename CreateFunctorPointerList<Tail>::List>
          List;
   };

   /**
    * @class      : CreateFunctorPointerList
    * @brief This class is used to end the recursion metaprogramming
    *        algorithm being used to create a functor pointer list
    *        using each elment of a given typelist.
    */
   template <>
   struct CreateFunctorPointerList<Loki::NullType> {
      typedef Loki::NullType List;
   };

   /**
    * @class      : CopyFunctorPointersFromTuple
    * @brief This class is used in the copy constructor of a tuple which
    *        stores functor pointers as its element. Please note the tuple_from
    *        and tuple_to should have the same template class list
    */
   template <class Tlist, int N>
   struct CopyFunctorPointersFromTuple {
      template <class List>
      static void doF(
          Loki::Tuple<typename CreateFunctorPointerList<List>::List> &tuple_to,
          const Loki::Tuple<typename CreateFunctorPointerList<Tlist>::List> &
              tuple_from);
   };

   template <class Head, class Tail, int N>
   struct CopyFunctorPointersFromTuple<Loki::Typelist<Head, Tail>, N> {
      template <class List>
      static void doF(
          Loki::Tuple<typename CreateFunctorPointerList<List>::List> &tuple_to,
          const Loki::Tuple<typename CreateFunctorPointerList<List>::List> &
              tuple_from) {
         Loki::Field<N>(tuple_to) =
             new Loki::Functor<Head>(*(Loki::Field<N>(tuple_from)));
         CopyFunctorPointersFromTuple<Tail, N + 1>::template doF<List>(
             tuple_to, tuple_from);
      }
   };

   template <class Tail, int N>
   struct CopyFunctorPointersFromTuple<Loki::Typelist<Tail, Loki::NullType>,
                                       N> {
      template <class List>
      static void doF(
          Loki::Tuple<typename CreateFunctorPointerList<List>::List> &tuple_to,
          const Loki::Tuple<typename CreateFunctorPointerList<List>::List> &
              tuple_from) {
         Loki::Field<N>(tuple_to) =
             new Loki::Functor<Tail>(*(Loki::Field<N>(tuple_from)));
      }
   };

   /**
    * @class      : DeleteFunctorPointersFromTuple
    * @brief This class is used to delete the tuple arguments from the stack
    */
   template <class Tlist, int N>
   struct DeleteFunctorPointersFromTuple {
      template <class List>
      static void doF(
          Loki::Tuple<typename CreateFunctorPointerList<List>::List> &tuple);
   };

   template <class Head, class Tail, int N>
   struct DeleteFunctorPointersFromTuple<Loki::Typelist<Head, Tail>, N> {
      template <class List>
      static void doF(
          Loki::Tuple<typename CreateFunctorPointerList<List>::List> &tuple) {
         delete Loki::Field<N>(tuple);
         DeleteFunctorPointersFromTuple<Tail, N + 1>::template doF<List>(tuple);
      }
   };

   template <class Tail, int N>
   struct DeleteFunctorPointersFromTuple<Loki::Typelist<Tail, Loki::NullType>,
                                         N> {
      template <class List>
      static void doF(
          Loki::Tuple<typename CreateFunctorPointerList<List>::List> &tuple) {
         delete Loki::Field<N>(tuple);
      }
   };

   /**
    * @class      : FunctorWrapper
    * @brief      : FunctorWrappers are used to wrap functors which take
    *               multiple arguments.
    * @details    : We store the functor pointer in _func. Then we have a
    *               functor with the same return type as the functor
    *               pointer but it doesn't accept any arguments. In the
    *               constructor we specify the functor pointer, the tuple
    *               of functors which will be used to compute the arguments
    *               to the functors. So when we cast the object to a
    *               variable of type Return we first update the _tuple_args
    *               which stores the vaulues of the functor arguments by
    *               callind each element of the tuple of functors for the
    *               arguments and then after using the values from _tuple_args
    *               we call the functor specified in _func. This is especially
    *               useful in dictionaries
    */
   template <class Return,
             class Tlist = Loki::NullType,
             class PropertiesType = Tlist>
   struct FunctorWrapper {
     public:
      /**
       * @brief Pointer to the functor which is being wrapped
       *
       * @details Suppose we want to call a function with automatic
       *          computation of the arguments of the funcion we wrap
       *          that functor with a FunctorWrapper object
       */
      Loki::Functor<Return, Tlist> *_func;
      /**
       * @brief This is the functor which doesn't take any arguments
       *
       * @details This works because we use the BindFirstRef function to
       *          bind the arguments of the function
       */
      Loki::Functor<Return> _func_no_args;
      /**
       * @brief Tuple which holds the values of the arguments of the function
       *
       * @details This is always updated in the static_cast operator by
       *          calling each element of _arg_functors and storing the result
       *          in this tuple.
       */
      Loki::Tuple<typename RemoveReferenceAndConst<Tlist>::List> _tuple_args;
      /**
       * @brief Tuple of functors which are going to be used to compute the
       *        arguments.
       *
       * @details We call each element of this tuple and store the returned
       *          value in _tuple_args as the _func_no_args has arguments
       *          bound to the elements inside _tuple_args.
       */
      Loki::Tuple<typename CreateFunctorPointerList<Tlist>::List> _arg_functors;

      typedef Return ResultType;
      typedef Tlist ListArgs;
      FunctorWrapper(
          Loki::Functor<ResultType, Tlist> &func,
          const Loki::Tuple<typename CreateFunctorPointerList<Tlist>::List> &
              tuple_functors);
      FunctorWrapper(const FunctorWrapper &obj);

      FunctorWrapper();

      template <class Tlist_args, int N>
      struct IterateOverArgs {
         static Loki::Functor<Return> doF(
             FunctorWrapper &obj,
             Loki::Functor<ResultType, Tlist_args> &func_original,
             Loki::Tuple<typename CreateFunctorPointerList<Tlist>::List> &
                 tuple_functors);
      };

      template <class Head, class Tail, int N>
      struct IterateOverArgs<Loki::Typelist<Head, Tail>, N> {
         static Loki::Functor<Return> doF(
             FunctorWrapper &obj,
             Loki::Functor<ResultType, Loki::Typelist<Head, Tail> > &
                 func_original,
             Loki::Tuple<typename CreateFunctorPointerList<Tlist>::List> &
                 tuple_functors);
      };

      template <class Tail, int N>
      struct IterateOverArgs<Loki::Typelist<Tail, Loki::NullType>, N> {
         static Loki::Functor<Return> doF(
             FunctorWrapper &obj,
             Loki::Functor<ResultType, Loki::Typelist<Tail, Loki::NullType> > &
                 func_original,
             Loki::Tuple<typename CreateFunctorPointerList<Tlist>::List> &
                 tuple_functors);
      };

      template <int N>
      struct IterateOverArgs<Loki::NullType, N> {
         static Loki::Functor<Return> doF(
             FunctorWrapper &obj,
             Loki::Functor<ResultType> &func_original,
             Loki::Tuple<typename CreateFunctorPointerList<Tlist>::List> &
                 tuple_functors);
      };

      template <int N, class flag = Loki::Int2Type<1> >
      struct LoopOverTuple {
         static void doF(FunctorWrapper &obj);
      };

      template <int N>
      struct LoopOverTuple<N, Loki::Int2Type<0> > {
         static void doF(FunctorWrapper &obj);
      };
      Return return_val();

      operator ResultType();

      virtual ~FunctorWrapper();
   };

   template <class A>
   A return_val(A val);

   template <class A>
   Loki::Functor<A> *wrap_var_with_functor(A val);
}
#endif
