#ifndef _CELL_DECL_H_INCLUDED
#define _CELL_DECL_H_INCLUDED

#include <vector>
#include <map>
#include "loki/HierarchyGenerators.h"
#include "NodeSetGet.decl.h"

namespace tmb {

   template <class List>
   struct CreateMapOfNodeElements;

   template <class Type>
   struct Node;

   template <class Head, class Tail>
   struct CreateMapOfNodeElements<Loki::Typelist<Head, Tail> > {
      typedef Loki::Typelist<std::map<std::string, Node<Head> >,
                             typename CreateMapOfNodeElements<Tail>::List> List;
   };

   template <class Tail>
   struct CreateMapOfNodeElements<Loki::Typelist<Tail, Loki::NullType> > {
      typedef Loki::Typelist<std::map<std::string, Node<Tail> >, Loki::NullType>
          List;
   };

   template <class List>
   struct CreateNodeElements;

   template <class Head, class Tail>
   struct CreateNodeElements<Loki::Typelist<Head, Tail> > {
      typedef Loki::Typelist<Node<Head>,
                             typename CreateNodeElements<Tail>::List> List;
   };

   template <class Tail>
   struct CreateNodeElements<Loki::Typelist<Tail, Loki::NullType> > {
      typedef Loki::Typelist<Node<Tail>, Loki::NullType> List;
   };

   template <class A>
   struct Node;
   template <class PropertiesTypeList>
   struct Cell {
      typedef
          typename tmb::CreateNodeElements<PropertiesTypeList>::List NodeList;
#ifdef DEBUG
      /**
       * @brief Node holder
       *
       * @details We will create a tuple of maps which holds nodes of
       *          each element of propertiestypelist. Please note we
       *          will only use this in debug mode and this is going
       *          be used when we are getting values from the cell.
       *          We will check the return type requested with the
       *          original type of the variable
       */
      Loki::Tuple<typename CreateNodeElements<PropertiesTypeList>::List>
          _node_holder;
#endif

      /**
       * @brief Vector of functors which will be used to create a copy of
       *        the elements in _vec_pointer.
       *
       * Each functor is a pointer to the create_copy_from_void_ptr. This
       * is used in the copy constructor to make a deep copy of the Cell.
       */
      std::vector<Loki::Tuple<LOKI_TYPELIST_3(
          void *(*)(void *, NodeSetGet<PropertiesTypeList> *, void *),
          NodeSetGet<PropertiesTypeList> *,
          void (*)(void *))> > *_vec_copy_functor_node_set_get;

      /**
       * @brief Vector of pointers to different nodes
       *
       * We store this as a void pointer because a node can be a node of
       * double or a node of vectors or matrices.
       */
      std::vector<void *> _vec_pointer;

      /**
       * @brief Map of index in _vec_pointers to different nodes
       *
       * We use strings as keys to store the index of the pointers
       * in _vec_pointer.
       */
      std::map<std::string, size_t> _vec_pointer_map;

      template <class A>
      void add_a_new_node(const char *name, A &default_val);
      /**
       * @brief  Register a vairable with name
       * @param  name - Name of the variable
       * @details  This function will create a new node and add it _vec_pointer
       *           and to _vec_pointer_map with name as the key
       */
      template <class A>
      void register_variable(const char *name);

      /**
       * @brief  Add multiple variables at the same time with a default val
       * @param  vec_vars - vector of variable names
       * @param  default_val - Default value of the variable
       * @details This will add a new variable for each element of vec_vars
       *          by looping over them
       */
      template <class A>
      void register_variable(const std::vector<const char *> &vec_vars,
                             A default_val);

      /**
       * @brief   Add new nodes using variadic arguments
       * @param   default_val Default value of the arguments
       * @param   count Number of variables to be added
       * @details This function uses the register_variable which takes a vector
       *          of strings as arguments to add variables
       */
      template <class A>
      void register_variable(A default_val, int count, ...);

      /**
       * @brief   This function returns the key of variable at given index
       * @param   index Index of the element in Cell
       * @return  Name of the variable
       * @details This function gets the pointer from _vec_pointer[index] and
       *          then searches for this pointer in _vec_pointer_map. It
       *          then returns the key at this iterator position in the map
       */
      std::string key_of_index(size_t index) const;

      /**
       * @brief   Copy constructor
       * @param   copy_from Cell to be copied
       * @details This will make a deep copy from copy_from and use the
       *          vector of copy functors extensively to do so.
       */
      Cell(const Cell &copy_from);

      /**
       * @brief   Default constructor
       * @details  This does nothing
       */
      Cell();

      /**
       * @brief   This function returns the value of the variable
       * @param   name Name of the variable
       * @return  reference to the value
       */
      template <class A>
      A &get_val(std::string name);

      /**
       * @brief   This function returns the value of the variable
       * @param   index Index of the variable
       * @return  reference to the value
       */
      template <class A>
      A &get_val(size_t index);

      /**
       * @brief   This function is used to the value of a node
       * @param   name Name of the variable
       * @param   val Value of the variable
       */
      template <class A, class Val>
      void set(std::string name, Val val);

      /**
       * @brief   This function sets the value of a node
       * @param   index Index of the variable
       * @param   val Value of the variable
       */
      template <class A, class Val>
      void set(size_t index, Val val);

      NodeSetGet<PropertiesTypeList> &operator[](std::string name);

      void delete_node(unsigned index);
      void replace_node(unsigned index, void *ptr);

      ~Cell();
   };
}
#endif
