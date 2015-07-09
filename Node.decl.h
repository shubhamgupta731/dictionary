#ifndef _NODE_DECL_H_INCLUDED
#define _NODE_DECL_H_INCLUDED
#include <string>
#include <vector>
#include "loki/Functor.h"
#include "loki/SmartPtr.h"
#include "Subject.decl.h"
#include "Observer.decl.h"
#include "Typelist.decl.h"

/**
 * @class      : HAS_TYPEDEF
 * @brief      : This macro will be used to check if a class has a particular
 * typedef or not. A tutorial can be found in
 * $TRANSATMB_DIR/examples/HAS_TYPEDEF_HAS_MEM_FUNC_tutorial.cxx to see an
 * example of how it works.
 * @param typedef_name : Name of Typedef
 * @param name  : Name of struct which is going to be run the test for
 * the given particular typedef specified in typedef_name
 */
#define HAS_TYPEDEF(typedef_name, name)                               \
   template <typename T>                                              \
   struct name {                                                      \
      typedef char yes[1];                                            \
      typedef char no[2];                                             \
      template <typename U>                                           \
      struct type_check;                                              \
      template <typename _1>                                          \
      static yes& chk(type_check<typename _1::typedef_name>*);        \
      template <typename>                                             \
      static no& chk(...);                                            \
      template <bool has_typedef, class _1>                           \
      struct holder {                                                 \
         typedef Loki::Typelist<Loki::NullType, Loki::NullType> type; \
      };                                                              \
      template <class _1>                                             \
      struct holder<true, _1> {                                       \
         typedef typename _1::typedef_name type;                      \
      };                                                              \
      static bool const Result = sizeof(chk<T>(0)) == sizeof(yes);    \
   }
namespace tmb {
   HAS_TYPEDEF(isANode, has_is_a_node);
   template <class A>
   class Node;

   template <class A>
   class NodeObserver : public Observer {
     protected:
      size_t _index;
      Node<A>* _ptr;

     public:
      NodeObserver(size_t index, Node<A>* ptr);
      void update();
   };

   template <class A>
   class Node : public Subject {
     protected:
      typedef Loki::Int2Type<0> isANode;
      /**
       * @brief name - Name of the variable
       */
      std::string _name;
      /**
       * @brief Value of the node
       */
      A _val;

      /**
       * @brief Vector of functors which to be used to compute the variable
       *
       * Please note that we store a functor which returns a ref to a value
       * using a function which is used to compute it. Since the function will
       * have arguments we will use a functorWrapper which will bind the
       * different arguments to certain variables or to other Nodes
       */
      std::vector<Loki::SmartPtr<Loki::Functor<A> > > _vec_functors;

      /**
       * @brief vector of observers
       *
       * The size of this vector should be equal to _vec_functors because
       * each observer belongs to a strategy.
       */
      std::vector<Loki::SmartPtr<NodeObserver<A> > > _vec_observers;

      /**
       * @brief This functor points to the get_solved function which just
       * returns
       *        the value
       */
      Loki::SmartPtr<Loki::Functor<A> > _get_solved;
      /**
       * @brief Reference to the functor that will be called when get function
       * is
       *        called.
       */
      Loki::SmartPtr<Loki::Functor<A> >* _active_get;

      /**
       * @brief   This function just returns the value of the variable
       * @return  Value of the variable
       */
      A& get_solved();

     public:
      /**
       * @brief   Calls the functor pointed by _active_get
       * @return  Value of the variable
       */
      A& get();
      /**
       * @brief   Sets the value of the node
       * @return  Returns a reference to the value of the node
       */
      template <class B>
      A& set(B bal);

      /**
       * @brief   Add a strategy which takes one argument
       */
      template <class Arg1>
      void addStrategy(Loki::Functor<A, TYPELIST(Arg1)>* functor, Arg1 arg1);

      /**
       * @brief   Add a strategy which takes two argument
       */
      template <class Arg1, class Arg2>
      void addStrategy(Loki::Functor<A, TYPELIST(Arg1, Arg2)>* functor,
                       Arg1 arg1,
                       Arg2 arg2);

      Node(std::string name);
      Node(const Node<A>& copy_from);
   };
}
#endif
