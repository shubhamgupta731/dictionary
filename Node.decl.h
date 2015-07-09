#ifndef _NODE_DECL_H_INCLUDED
#define _NODE_DECL_H_INCLUDED
#include <string>
#include <vector>
#include "loki/Functor.h"
#include "loki/SmartPtr.h"
#include "Subject.decl.h"
#include "Observer.decl.h"

namespace tmb {
   template <class A>
   class Node : public Subject, public Observer {
     protected:
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
       * @brief   Add a strategy to the vector of functors for this node
       */
      void addStrategy(Loki::SmartPtr<Loki::Functor<A> > strategy);

      Node(std::string name);
      Node(const Node<A>& copy_from);
   };
}
#endif
