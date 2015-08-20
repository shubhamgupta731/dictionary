#ifndef _NODE_SET_GET_DECL_H_INCLUDED
#define _NODE_SET_GET_DECL_H_INCLUDED

#include "loki/HierarchyGenerators.h"
#include <iostream>
#include <cassert>
#include "Node.decl.h"

namespace tmb {

   template <class List>
   struct CreateNodePointerList;

   template <class Head, class Tail>
   struct CreateNodePointerList<Loki::Typelist<Head, Tail> > {
      typedef Loki::Typelist<Node<Head>*,
                             typename CreateNodePointerList<Tail>::List> List;
   };

   template <class Tail>
   struct CreateNodePointerList<Loki::Typelist<Tail, Loki::NullType> > {
      typedef Loki::Typelist<Node<Tail>*, Loki::NullType> List;
   };

   template <class A>
   class Cell;

   template <class List>
   class NodeSetGet {
      Loki::Tuple<typename CreateNodePointerList<List>::List> _tuple;
      unsigned _index_in_tuple;
      unsigned _index_in_cell;
      Cell<List>* _cell;

     public:
      NodeSetGet(Cell<List>* cell = NULL);
      NodeSetGet(const NodeSetGet<List>& copy_from);
      bool has_a_node() const;
      Loki::Tuple<typename CreateNodePointerList<List>::List>& get_tuple();
      NodeSetGet(Cell<List>* cell, unsigned index_in_cell);
      template <class A>
      void set_node(tmb::Node<A>* node);
      template <class A>
      const NodeSetGet<List>& operator=(A val);

      template <class ResultType>
      ResultType get() const;

      const NodeSetGet<List>& operator=(const NodeSetGet<List>& copy_from);

      template <class A>
      const NodeSetGet& operator+=(const A& rhs);

      template <class A>
      const NodeSetGet& operator-=(const A& rhs);

      template <class A>
      const NodeSetGet& operator*=(const A& rhs);

      template <class A>
      const NodeSetGet& operator/=(const A& rhs);

      template <class A>
      const NodeSetGet& operator%=(const A& rhs);

      template <class A>
      const NodeSetGet& operator^=(const A& rhs);

      template <class A>
      const NodeSetGet& operator&=(const A& rhs);

      template <class A>
      const NodeSetGet& operator|=(const A& rhs);
   };

   template <class A, class List>
   A& operator<<(A& lhs, NodeSetGet<List>& rhs);
}
#endif
