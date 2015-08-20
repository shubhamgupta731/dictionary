#ifndef _CELL_H_INCLUDED
#define _CELL_H_INCLUDED
#include "Cell.decl.h"
#include "NodeSetGet.h"
#include <algorithm>
#include <iostream>
#include <assert.h>

namespace tmb {
   template <class A>
   void cast_and_delete(void* ptr) {
      delete *static_cast<A**>(ptr);
      ptr = NULL;
   }

   template <class A, class List>
   void* create_copy_from_void_ptr(void* ptr,
                                   NodeSetGet<List>* node_set_get,
                                   void* void_pointer_copy_functor_set_get) {
      std::vector<Loki::Tuple<LOKI_TYPELIST_3(
          void* (*)(void*, NodeSetGet<List>*, void*),
          NodeSetGet<List>*,
          void (*)(void*))> >* vec =
          static_cast<std::vector<Loki::Tuple<LOKI_TYPELIST_3(
              void* (*)(void*, NodeSetGet<List>*, void*),
              NodeSetGet<List>*,
              void (*)(void*))> >*>(void_pointer_copy_functor_set_get);
      A* node_ptr;
      if (ptr)
         node_ptr = new A(*static_cast<A*>(ptr));
      else
         node_ptr = NULL;
      node_set_get->template set_node<typename A::Type>(node_ptr);
      Loki::Tuple<LOKI_TYPELIST_3(void* (*)(void*, NodeSetGet<List>*, void*),
                                  NodeSetGet<List>*,
                                  void (*)(void*))> tuple;
      Loki::Field<0>(tuple) = &tmb::create_copy_from_void_ptr<A, List>;
      Loki::Field<1>(tuple) = node_set_get;
      Loki::Field<2>(tuple) = &cast_and_delete<A*>;
      vec->push_back(tuple);
      return node_ptr;
   }
}

template <class List>
template <class A>
void tmb::Cell<List>::add_a_new_node(const char* name, A& default_val) {
   typedef typename tmb::CreateNodeElements<List>::List NodeList;
   if (_vec_pointer_map.find(name) == _vec_pointer_map.end()) {
      std::string name_str(name);
      tmb::Node<A>* node = new tmb::Node<A>(name);
      node->set(default_val);
      _vec_pointer.push_back(node);
      tmb::NodeSetGet<List>* node_set_get =
          new tmb::NodeSetGet<List>(this, _vec_pointer.size() - 1);
      node_set_get->set_node(node);
      _vec_pointer_map[name] = _vec_pointer.size() - 1;
      Loki::Tuple<LOKI_TYPELIST_3(void* (*)(void*, NodeSetGet<List>*, void*),
                                  NodeSetGet<List>*,
                                  void (*)(void*))> tuple;
      Loki::Field<0>(tuple) =
          &tmb::create_copy_from_void_ptr<tmb::Node<A>, List>;
      Loki::Field<1>(tuple) = node_set_get;
      Loki::Field<2>(tuple) = &cast_and_delete<tmb::Node<A>*>;
      _vec_copy_functor_node_set_get->push_back(tuple);
   } else {
      std::cerr << "Variable with name: " << name << "already exists."
                << std::endl;
      assert(false);
   }
}

template <class List>
template <class A>
void tmb::Cell<List>::register_variable(const char* name) {
   // TODO: need to add a function which returns the default value for
   //       different template parameters
   A default_val;
   add_a_new_node(name, default_val);
}

template <class List>
template <class A>
void tmb::Cell<List>::register_variable(
    const std::vector<const char*>& vec_vars, A default_val) {
   for (std::vector<const char*>::const_iterator it = vec_vars.begin();
        it != vec_vars.end();
        ++it) {
      add_a_new_node(*it, default_val);
   }
}

template <class List>
template <class A>
void tmb::Cell<List>::register_variable(A default_val, int count, ...) {
   std::vector<const char*> vec_args;
   va_list args;
   va_start(args, count);
   for (int i = 0; i < count; ++i)
      vec_args.push_back(va_arg(args, const char*));
   va_end(args);
   register_variable<A>(vec_args, default_val);
}

template <class List>
std::string tmb::Cell<List>::key_of_index(size_t index) const {
   for (std::map<std::string, size_t>::const_iterator it =
            _vec_pointer_map.begin();
        it != _vec_pointer_map.end();
        ++it)
      if (it->second == index)
         return it->first;
#ifdef DEBUG
   std::cerr << "Pointer given at index: " << index
             << " in _vec_pointer is not present in _vec_pointer_pointer."
             << std::endl;
   assert(false);
#endif
   return "Invalid";
}

template <class List>
tmb::Cell<List>::Cell(const tmb::Cell<List>& copy_from) {
   _vec_copy_functor_node_set_get = new std::vector<
       Loki::Tuple<LOKI_TYPELIST_3(void* (*)(void*, NodeSetGet<List>*, void*),
                                   NodeSetGet<List>*,
                                   void (*)(void*))> >();
   size_t end_size = copy_from._vec_copy_functor_node_set_get->size();
   for (size_t i = 0; i < end_size; ++i) {
      tmb::NodeSetGet<List>* node_set_get = new tmb::NodeSetGet<List>(this, i);
      void* ptr =
          Loki::Field<0>((*copy_from._vec_copy_functor_node_set_get)[i])(
              copy_from._vec_pointer[i],
              node_set_get,
              _vec_copy_functor_node_set_get);
      _vec_pointer.push_back(ptr);
      _vec_pointer_map[copy_from.key_of_index(i)] = _vec_pointer.size() - 1;
   }
}

template <class List>
tmb::Cell<List>::Cell() {
   _vec_copy_functor_node_set_get = new std::vector<
       Loki::Tuple<LOKI_TYPELIST_3(void* (*)(void*, NodeSetGet<List>*, void*),
                                   NodeSetGet<List>*,
                                   void (*)(void*))> >();
}

template <class List>
tmb::Cell<List>::~Cell() {
   delete _vec_copy_functor_node_set_get;
}

template <class List>
template <class A>
A& tmb::Cell<List>::get_val(std::string name) {
#ifdef DEBUG
   if (_vec_pointer_map.find(name) == _vec_pointer_map.end()) {
      std::cerr << "Could not find element with name: " << name << std::endl;
      assert(false);
   }
#endif
   return get_val<A>(_vec_pointer_map[name]);
}

template <class List>
template <class A>
A& tmb::Cell<List>::get_val(size_t index) {
#ifdef DEBUG
   if (index >= _vec_pointer.size()) {
      std::cerr << "Requested index: " << index
                << " is more than the number of elements." << std::endl;
      assert(false);
   }
#endif
   return static_cast<tmb::Node<A>*>(_vec_pointer[index])->get();
}

template <class List>
template <class A, class Val>
void tmb::Cell<List>::set(std::string name, Val val) {
#ifdef DEBUG
   if (_vec_pointer_map.find(name) == _vec_pointer_map.end()) {
      std::cerr << "Could not find element with name: " << name << std::endl;
      assert(false);
   }
#endif
   set<A>(_vec_pointer_map[name], val);
}

template <class List>
template <class A, class Val>
void tmb::Cell<List>::set(size_t index, Val val) {
#ifdef DEBUG
   if (index >= _vec_pointer.size()) {
      std::cerr << "Requested index: " << index
                << " is more than the number of elements." << std::endl;
      assert(false);
   }
#endif
   static_cast<tmb::Node<A>*>(_vec_pointer[index])->set(val);
}

template <class List>
tmb::NodeSetGet<List>& tmb::Cell<List>::operator[](std::string name) {
   return *Loki::Field<1>(
       (*_vec_copy_functor_node_set_get)[_vec_pointer_map[name]]);
}

template <class List>
void tmb::Cell<List>::delete_node(unsigned index) {
   Loki::Field<2>((*_vec_copy_functor_node_set_get)[index])(
       &(_vec_pointer[index]));
   _vec_pointer[index] = NULL;
}

template <class List>
void tmb::Cell<List>::replace_node(unsigned index, void* ptr) {
   delete_node(index);
   _vec_pointer[index] = ptr;
}
#endif
