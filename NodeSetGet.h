#ifndef _NODE_SET_GET_H_INCLUDED
#define _NODE_SET_GET_H_INCLUDED

#include "NodeSetGet.decl.h"
#include "Node.h"

namespace tmb {
   template <class List, class OriginalList, int N, class Operation>
   struct IterateOverTupleToFindElement;

   template <class List, class OriginalList, int N>
   struct IterateOverTupleToFindElementAndGet;

   struct ReturnRHS;
   struct ReturnSum;
   struct ReturnDiff;
   struct ReturnProduct;
   struct ReturnDiv;

   template <class List, int N>
   struct CheckIfNodeHasANode;
}

template <class List>
tmb::NodeSetGet<List>::NodeSetGet(const tmb::NodeSetGet<List>& copy_from) {
   (*this) = copy_from;
}

template <class List>
tmb::NodeSetGet<List>::NodeSetGet(tmb::Cell<List>* cell)
    : _cell(cell) {}

template <class List>
Loki::Tuple<typename tmb::CreateNodePointerList<List>::List>&
    tmb::NodeSetGet<List>::get_tuple() {
   return _tuple;
}
template <class List>
tmb::NodeSetGet<List>::NodeSetGet(tmb::Cell<List>* cell, unsigned index_in_cell)
    : _cell(cell), _index_in_cell(index_in_cell) {}

template <class List>
template <class A>
void tmb::NodeSetGet<List>::set_node(tmb::Node<A>* node) {
   Loki::Field<Loki::TL::IndexOf<List, A>::value>(_tuple) = node;
   _index_in_tuple = Loki::TL::IndexOf<List, A>::value;
}

template <class A, class List>
A operator+(A lhs, tmb::NodeSetGet<List>& rhs) {
   return lhs + rhs.template get<A>();
}

template <class List1, class List2>
tmb::NodeSetGet<List1> operator+(tmb::NodeSetGet<List1>& lhs,
                                 const tmb::NodeSetGet<List2>& rhs) {
   tmb::NodeSetGet<List1> temp(lhs);
   return temp += rhs;
}

template <class A, class List>
A& operator<<(A& lhs, tmb::NodeSetGet<List>& rhs) {
   return lhs << rhs.template get<A>();
}

template <class A, class List>
A& operator<<(A& lhs, tmb::NodeSetGet<List>& rhs);

template <class List>
template <class A>
const tmb::NodeSetGet<List>& tmb::NodeSetGet<List>::operator=(A val) {
   if (!tmb::CheckIfNodeHasANode<List, 0>::doF(this)) {
      tmb::Node<A>* ptr = new tmb::Node<A>(_cell->key_of_index(_index_in_cell));
      ptr->set(val);
      set_node(ptr);
      _cell->replace_node(_index_in_cell, ptr);
      return *this;
   } else {
      return tmb::IterateOverTupleToFindElement<List, List, 0, ReturnRHS>::doF(
          this, val);
   }
}

template <class List>
const tmb::NodeSetGet<List>& tmb::NodeSetGet<List>::operator=(
    const tmb::NodeSetGet<List>& copy_from) {
   return tmb::IterateOverTupleToFindElement<List, List, 0, ReturnRHS>::doF(
       this, copy_from);
}

template <class List>
template <class A>
const tmb::NodeSetGet<List>& tmb::NodeSetGet<List>::operator+=(const A& rhs) {
   return tmb::IterateOverTupleToFindElement<List, List, 0, ReturnSum>::doF(
       this, rhs);
}

template <class List>
template <class A>
const tmb::NodeSetGet<List>& tmb::NodeSetGet<List>::operator-=(const A& rhs) {
   return tmb::IterateOverTupleToFindElement<List, List, 0, ReturnDiff>::doF(
       this, rhs);
}

template <class List>
template <class A>
const tmb::NodeSetGet<List>& tmb::NodeSetGet<List>::operator*=(const A& rhs) {
   return tmb::IterateOverTupleToFindElement<List, List, 0, ReturnProduct>::doF(
       this, rhs);
}

template <class List>
template <class A>
const tmb::NodeSetGet<List>& tmb::NodeSetGet<List>::operator/=(const A& rhs) {
   return tmb::IterateOverTupleToFindElement<List, List, 0, ReturnDiv>::doF(
       this, rhs);
}

template <class List>
template <class ResultType>
ResultType tmb::NodeSetGet<List>::get() const {
   return tmb::IterateOverTupleToFindElementAndGet<List, List, 0>::template doF<
       ResultType>(_tuple);
}

namespace tmb {
   template <class Var>
   struct CheckAndGetValues {
      template <class A>
      static const A doF(const Var& val) {
         return static_cast<const A>(val);
      }
   };

   template <class Var>
   struct CheckAndGetValues<NodeSetGet<Var> > {
      template <class A>
      static const A doF(const NodeSetGet<Var>& val) {
         return val.template get<const A>();
      }
   };

   struct ReturnRHS {
      template <class Return, class Var1, class Var2>
      static Return doF(const Var1&, const Var2& var2) {
         return var2;
      }
   };

   struct ReturnSum {
      template <class Return, class Var1, class Var2>
      static Return doF(const Var1& var1, const Var2& var2) {
         return var1 + var2;
      }
   };

   struct ReturnDiff {
      template <class Return, class Var1, class Var2>
      static Return doF(const Var1& var1, const Var2& var2) {
         return var1 - var2;
      }
   };

   struct ReturnProduct {
      template <class Return, class Var1, class Var2>
      static Return doF(const Var1& var1, const Var2& var2) {
         return var1 * var2;
      }
   };

   struct ReturnDiv {
      template <class Return, class Var1, class Var2>
      static Return doF(const Var1& var1, const Var2& var2) {
         return var1 / var2;
      }
   };

   template <class Head, class Tail, int N>
   struct CheckIfNodeHasANode<Loki::Typelist<Head, Tail>, N> {
      static bool doF(NodeSetGet<Loki::Typelist<Head, Tail> >* node) {
         if (Loki::Field<N>(node->get_tuple())) {
            return true;
         } else
            return CheckIfNodeHasANode<Tail, N + 1>::doF(node);
      }
   };

   template <class Tail, int N>
   struct CheckIfNodeHasANode<Loki::Typelist<Tail, Loki::NullType>, N> {
      static bool doF(NodeSetGet<Loki::Typelist<Tail, Loki::NullType> >* node) {
         if (Loki::Field<N>(node->get_tuple())) {
            return true;
         } else
            return false;
      }
   };
   template <class Head, class Tail, class OriginalList, int N, class Operation>
   struct IterateOverTupleToFindElement<Loki::Typelist<Head, Tail>,
                                        OriginalList,
                                        N,
                                        Operation> {
      template <class Var>
      static const NodeSetGet<OriginalList>& doF(NodeSetGet<OriginalList>* node,
                                                 const Var& var) {
         if (Loki::Field<N>(node->get_tuple())) {
            typedef typename Loki::TL::TypeAt<OriginalList, N>::Result Type;
            Loki::Field<N>(node->get_tuple())
                ->set(Operation::template doF<Type>(
                    node->template get<Type>(),
                    CheckAndGetValues<Var>::template doF<Type>(var)));
            return *node;
         } else {
            return IterateOverTupleToFindElement<
                Loki::Typelist<Tail, Loki::NullType>,
                OriginalList,
                N + 1,
                Operation>::doF(node->get_tuple(), var);
         }
      }
   };

   template <class Tail, class OriginalList, int N, class Operation>
   struct IterateOverTupleToFindElement<Loki::Typelist<Tail, Loki::NullType>,
                                        OriginalList,
                                        N,
                                        Operation> {
      template <class Var>
      static const NodeSetGet<OriginalList>& doF(NodeSetGet<OriginalList>* node,
                                                 const Var& var) {
         if (Loki::Field<N>(node->get_tuple())) {
            typedef typename Loki::TL::TypeAt<OriginalList, N>::Result Type;
            Loki::Field<N>(node->get_tuple())
                ->set(Operation::template doF<Type>(
                    node->template get<Type>(),
                    CheckAndGetValues<Var>::template doF<Type>(var)));
            return *node;
         } else {
            std::cerr << "Operation on invalid node" << std::endl;
            assert(false);
            return *node;
         }
      }
   };

   template <class Head, class Tail, class OriginalList, int N>
   struct IterateOverTupleToFindElementAndGet<Loki::Typelist<Head, Tail>,
                                              OriginalList,
                                              N> {
      template <class Return>
      static Return doF(
          const Loki::Tuple<typename CreateNodePointerList<OriginalList>::List>&
              tuple) {
         if (Loki::Field<N>(tuple)) {
            return Loki::Field<N>(tuple)->get();
         } else {
            return IterateOverTupleToFindElementAndGet<
                Loki::Typelist<Tail, Loki::NullType>,
                OriginalList,
                N + 1>::template doF<Return>(tuple);
         }
      }
   };

   template <class Tail, class OriginalList, int N>
   struct IterateOverTupleToFindElementAndGet<
       Loki::Typelist<Tail, Loki::NullType>,
       OriginalList,
       N> {
      template <class Return>
      static Return doF(
          const Loki::Tuple<typename CreateNodePointerList<OriginalList>::List>&
              tuple) {
         if (Loki::Field<N>(tuple)) {
            return Loki::Field<N>(tuple)->get();
         } else {
            std::cerr << "No pointer is set inside the tuple" << std::endl;
            assert(false);
         }
      }
   };
}

#endif
