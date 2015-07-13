#include <iostream>
#include "Node.h"
#include "FunctorWrapper.h"

template <class A>
A cast_value(void* void_ptr) {
   return *static_cast<A*>(void_ptr);
}

int return_1() { return 1; }

int return_2(int& check) { return check + 5; }
int return_3(int& check) { return check + 10; }
int return_4(int& check, int& check1) { return check*check1 + 10; }

int main() {
   int check = 5;
   // Loki::Tuple<LOKI_TYPELIST_1(Loki::Functor<int&>*)> tuple_args;
   // Loki::Field<0>(tuple_args) = tmb::wrap_var_with_functor<int&>(check);
   Loki::Functor<int, LOKI_TYPELIST_1(int&)> return_2_func(&return_2);
   Loki::Functor<int, LOKI_TYPELIST_1(int&)> return_3_func(&return_3);
   Loki::Functor<int, LOKI_TYPELIST_2(int&, int&)> return_4_func(&return_4);
   // tmb::FunctorWrapper<int, LOKI_TYPELIST_1(int&)> wrap(return_2_func,
   // tuple_args);
   // int test = wrap;
   // std::cout << "test: " << test << std::endl;
   check = 7;
   // test = wrap;
   // std::cout << "test: " << test << std::endl;

   Loki::SmartPtr<tmb::Node<int> > test_node = new tmb::Node<int>("test");
   tmb::Node<int> int_node("int_node");
   int_node.set(1);
   tmb::Node<int> int_node_2("int_node_2");
   int_node_2.set(1);
   test_node->addStrategy<int&>(&return_2_func, &int_node);
   test_node->addStrategy<int&>(&return_3_func, &int_node_2);
   test_node->addStrategy<int&, int&>(&return_4_func, &int_node_2, &int_node);
   int_node.set(2);
   std::cout << "Node: " << test_node->get() << std::endl;
   int_node_2.set(2);
   std::cout << "Node: " << test_node->get() << std::endl;
   int_node.set(5);
   std::cout << "Node: " << test_node->get() << std::endl;
   int_node_2.set(5);
   std::cout << "Node: " << test_node->get() << std::endl;
}
