#include <iostream>
#include "Node.h"
#include "FunctorWrapper.h"

int return_1() { return 1; }

int return_2(int& check) { return check + 5; }
double return_3(double& check) { return check + 10; }
double return_4(double& check, double& check1) { return check * check1 + 10; }

int main() {
   int check = 5;
   // Loki::Tuple<LOKI_TYPELIST_1(Loki::Functor<int&>*)> tuple_args;
   // Loki::Field<0>(tuple_args) = tmb::wrap_var_with_functor<int&>(check);
   Loki::Functor<int, LOKI_TYPELIST_1(int&)> return_2_func(&return_2);
   Loki::Functor<double, LOKI_TYPELIST_1(double&)> return_3_func(&return_3);
   Loki::Functor<double, LOKI_TYPELIST_2(double&, double&)> return_4_func(
       &return_4);
   // tmb::FunctorWrapper<int, LOKI_TYPELIST_1(int&)> wrap(return_2_func,
   // tuple_args);
   // int test = wrap;
   // std::cout << "test: " << test << std::endl;
   check = 7;
   // test = wrap;
   // std::cout << "test: " << test << std::endl;

   Loki::SmartPtr<tmb::Node<double> > test_node = new tmb::Node<double>("test");
   std::vector<tmb::Node<double> > int_nodes, test_nodes, int_nodes_1,
       int_nodes_2, int_nodes_3;
   int_nodes_1.resize(100000, tmb::Node<double>("int_node_1"));
   int_nodes.resize(100000, tmb::Node<double>("int_node"));
   int_nodes_2.resize(100000, tmb::Node<double>("int_node_2"));
   int_nodes_3.resize(100000, tmb::Node<double>("int_node_3"));
   test_nodes.resize(100000, tmb::Node<double>("test_node"));
   for (size_t i = 0; i < 100000; ++i)
      int_nodes[i].addStrategy<double&>(&return_3_func, &int_nodes_1[i], "return_3");
   for (size_t i = 0; i < 100000; ++i)
      test_nodes[i].addStrategy<double&, double&>(
          &return_4_func, &int_nodes[i], &int_nodes_1[i], "return_4");
   for (size_t i = 0; i < 100000; ++i)
      test_nodes[i].addStrategy<double&, double&>(&return_4_func,
                                                  &int_nodes_2[i],
                                                  &int_nodes_3[i],
                                                  "return_4_with_2_3");
   double sum = 0;
   for (unsigned count = 0; count < 1; ++count)
      for (size_t i = 0; i < 100000; ++i) {
         int_nodes_1[i].set(5.0);
         sum += test_nodes[i].get();
      }
   std::cout << "sum: " << sum << std::endl;
   tmb::draw_dot_graph(&(test_nodes[0]), 5);
   return 0;
}
