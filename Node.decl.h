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
   class NodeSetAttributes;
   class BaseNodeFeatures;

   template <class A, char Index, char Key>
   class NodeObserver : public Observer {
     protected:
      NodeSetAttributes<A>* _ptr;

     public:
      NodeObserver(NodeSetAttributes<A>* ptr = NULL);
      NodeObserver(const NodeObserver<A, Index, Key>& copy);
      void update();
      void copy(const NodeObserver<A, Index, Key>& copy_from);
   };

   template <class A>
   void draw_nodes(std::ofstream& fs,
                   tmb::Node<A>* node,
                   unsigned levels,
                   unsigned count,
                   std::vector<std::string>& dictionary_of_nodes_added);

   void draw_nodes(std::ofstream& fs,
                   tmb::BaseNodeFeatures* node,
                   unsigned levels,
                   unsigned count,
                   std::vector<std::string>& dictionary_of_nodes_added);

   template <class A>
   void draw_dot_graph(tmb::Node<A>* node);

   template <class A>
   void draw_dot_graph(tmb::Node<A>* node, unsigned levels);

   class BaseFunctorWrapper;
   class BaseNodeFeatures {
     protected:
      /**
       * @brief name - Name of the variable
       */
      std::vector<tmb::BaseFunctorWrapper*>* _vec_functor_wrappers;
#ifdef DEBUG
      std::string _name;
      std::vector<Loki::Functor<std::vector<std::string> >*>
          _vec_functors_stream;
      std::vector<std::string> _vec_dependency_name;
      std::vector<std::vector<BaseNodeFeatures*> > _depends_on;
      std::vector<std::vector<std::string> > _vec_subjects;
      std::string _value_set_using;
#endif
      char _vec_dependencies_max[4], _vec_dependencies[4];
      char _number_of_strategies;

     public:
      void reset_dependencies();
      virtual std::string get_val_as_string() = 0;
#ifdef DEBUG
      virtual std::string& get_name();
      const std::vector<std::vector<std::string> >& vector_of_strings() const;
      const std::vector<std::string>& get_vector_dependencies_name() const;
      const std::vector<std::vector<BaseNodeFeatures*> >& get_dependent_nodes()
          const;
      virtual const std::string& get_value_set_using() const;
#endif
      std::vector<std::vector<std::string> > val_of_functor_wrappers();
      BaseNodeFeatures(const BaseNodeFeatures& copy_from);
      BaseNodeFeatures();
      virtual ~BaseNodeFeatures();
   };

   template <class A>
   class NodeVal {
     public:
      A* _val;
      NodeVal(const NodeVal<A>& copy_from);
      NodeVal();
   };

   template <class A>
   class NodeSetAttributes : public BaseNodeFeatures, public Subject {
     protected:
      /**
       * @brief Vector of functors which to be used to compute the variable
       *
       * Please note that we store a functor which returns a ref to a value
       * using a function which is used to compute it. Since the function will
       * have arguments we will use a functorWrapper which will bind the
       * different arguments to certain variables or to other Nodes
       */
      Loki::Functor<A>* _vec_functors[4];
      A* _val;

     public:
      template <class B>
      A& set(B val);
      NodeSetAttributes(std::string name);
      NodeSetAttributes(const NodeSetAttributes<A>& copy_from);
      ~NodeSetAttributes();

      template <class ArgList, class TupleArgs>
      void addStrategyMultiple(Loki::Functor<A, ArgList>* functor,
                               TupleArgs& tuple_args,
                               std::string& dependency_name);
      /**
       * @brief   Add a strategy which takes one argument
       */
      template <class Arg1, class Arg1_param>
      void addStrategy(Loki::Functor<A, TYPELIST(Arg1)>* functor,
                       Arg1_param arg1,
                       std::string dependency_name = "unknown");

      /**
       * @brief   Add a strategy which takes two argument
       */
      template <class Arg1, class Arg2, class Arg1_param, class Arg2_param>
      void addStrategy(Loki::Functor<A, TYPELIST(Arg1, Arg2)>* functor,
                       Arg1_param arg1,
                       Arg2_param arg2,
                       std::string dependency_name = "unknown");
      const Loki::Functor<A>** get_vec_functors() const;

      template <unsigned char Index, unsigned char Key>
      void set_active_strategy();
      void set_pointer(A* new_ptr);
      virtual std::string get_val_as_string();
   };

   template <class A>
   class Node {
     protected:
      NodeSetAttributes<A>* _set_ptr;
      NodeVal<A>* _val_ptr;

     public:
      typedef Loki::Int2Type<0> isANode;
      typedef A Type;
      /**
       * @brief   Calls the functor pointed by _active_get
       * @return  Value of the variable
       */
      A& get();
      /**
       * @brief   Calls the functor pointed by _active_get
       * @return  Value of the variable
       */
      const A& get() const;
      /**
       * @brief   Calls the functor pointed by _active_get
       * @return  Value of the variable
       */
      const A& get_const_ref();
      /**
       * @brief   Calls the functor pointed by _active_get
       * @return  Value of the variable
       */
      A get_copy();
      /**
       * @brief   Calls the functor pointed by _active_get
       * @return  Value of the variable
       */
      A* get_pointer();
      /**
       * @brief   Calls the functor pointed by _active_get
       * @return  Value of the variable
       */
      const A* get_const_pointer();
      /**
       * @brief   Sets the value of the node
       * @return  Returns a reference to the value of the node
       */
      template <class B>
      A& set(B bal);
      /**
       * @brief   Add a strategy which takes one argument
       */
      template <class Arg1, class Arg1_param>
      void addStrategy(Loki::Functor<A, TYPELIST(Arg1)>* functor,
                       Arg1_param arg1,
                       std::string dependency_name = "unknown");

#ifdef DEBUG
      virtual std::string& get_name();
#endif

      /**
       * @brief   Add a strategy which takes two argument
       */
      template <class Arg1, class Arg2, class Arg1_param, class Arg2_param>
      void addStrategy(Loki::Functor<A, TYPELIST(Arg1, Arg2)>* functor,
                       Arg1_param arg1,
                       Arg2_param arg2,
                       std::string dependency_name = "unknown");
      Node(std::string name = "undefined");
      Node(const Node<A>& copy_from);
      Loki::Functor<A&>& get_get_func();
      Loki::Functor<const A&>& get_const_ref_func();
      Loki::Functor<A*>& get_pointer_func();
      Loki::Functor<const A*>& get_const_pointer_func();
      Loki::Functor<A>& get_copy_func();
      NodeSetAttributes<A>* get_observable();
      void set_pointer(A* new_ptr);
      void attach(Observer* obs);
      ~Node();

      template <class B>
      Node& operator=(B val);
   };
}
#endif
