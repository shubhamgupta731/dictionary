#include <iostream>
#include "Node.h"
#include "FunctorWrapper.h"
#include <time.h> /* clock_t, clock, CLOCKS_PER_SEC */
#include <cmath>
#include "Cell.h"

int return_1() { return 1; }

int return_2(int& check) { return check + 5; }
double return_3(double& check) { return check + 10; }
double return_4(double& check, double& check1) { return check * check1 + 10; }

int main() {
   tmb::Cell<Loki::Typelist<double, Loki::NullType> > test;
   test.register_variable<double>(5.0, 2, "pressure", "pressure1");
   std::cout << test.get_val<double>("pressure") << std::endl;
   std::cout << test.get_val<double>("pressure1") << std::endl;
   test.delete_node(0);
   tmb::Cell<Loki::Typelist<double, Loki::NullType> > copy_test(test);
   copy_test.set<double>("pressure1", 10.0);
   copy_test.register_variable<double>(5.0, 1, "temperature");
   copy_test["pressure1"] = copy_test["temperature"];
   copy_test["temperature"] += copy_test["pressure1"];
   copy_test["temperature"] += 1.0;
   copy_test["test123"] = 1.1;
   std::cout << copy_test.get_val<double>("pressure1") << std::endl;
   std::cout << copy_test.get_val<double>("temperature") << std::endl;
   std::cout << copy_test.get_val<double>("test123") << std::endl;
   std::cout << copy_test.get_val<double>("pressure") << std::endl;
   double bla = 1.0 + copy_test["test123"];
   int test123 = 0 + copy_test["test123"] + copy_test["pressure1"];
   std::cout << bla << std::endl;
   std::cout << "test123: " << test123 << std::endl;

   int check = 5;

   Loki::Functor<int, LOKI_TYPELIST_1(int&)> return_2_func(&return_2);
   Loki::Functor<double, LOKI_TYPELIST_1(double&)> return_3_func(&return_3);
   Loki::Functor<double, LOKI_TYPELIST_2(double&, double&)> return_4_func(
       &return_4);
   std::cout << "sizeof Node: " << sizeof(tmb::Node<Loki::Functor<double> >)
             << std::endl;
   std::cout << "sizeof vector: " << sizeof(std::vector<double>) << std::endl;
   Loki::SmartPtr<tmb::Node<double> > test_node = new tmb::Node<double>("test");
   std::vector<tmb::Node<double> > int_nodes, test_nodes, int_nodes_1,
       int_nodes_2, int_nodes_3;
   {
      int_nodes_1.resize(1000000, tmb::Node<double>("int_node_1"));
      int_nodes.resize(1000000, tmb::Node<double>("int_node"));
      int_nodes_2.resize(1000000, tmb::Node<double>("int_node_2"));
      int_nodes_3.resize(1000000, tmb::Node<double>("int_node_3"));
      test_nodes.resize(1000000, tmb::Node<double>("test_node"));
   }
   for (size_t i = 0; i < 1000000; ++i)
      int_nodes[i].addStrategy<double&>(
          &return_3_func, &int_nodes_1[i], "return_3");
   for (size_t i = 0; i < 1000000; ++i)
      test_nodes[i].addStrategy<double&, double&>(&return_4_func,
                                                  &int_nodes_2[i],
                                                  &int_nodes_3[i],
                                                  "return_4_with_2_3");
   for (size_t i = 0; i < 1000000; ++i)
      test_nodes[i].addStrategy<double&, double&>(
          &return_4_func, &int_nodes[i], &int_nodes_1[i], "return_4");
   clock_t t;
   double sum = 0;
   std::vector<double*> test_node_ptr(1000000, NULL);
   for (size_t i = 0; i < 1000000; ++i) {
      test_node_ptr[i] = test_nodes[i].get_pointer();
   }
   for (size_t i = 1; i < 1000000; ++i)
      int_nodes_1[i].set(5.0);
   for (size_t i = 0; i < 1000000; ++i)
      int_nodes[i].set(5.0);
   t = clock();
   for (unsigned count = 0; count < 1000; ++count) {
      for (size_t i = 0; i < 1000000; ++i) {
         sum += *(test_node_ptr[i]);
      }
   }
   t = clock() - t;
   std::cout << "It took " << ((float)t) / CLOCKS_PER_SEC << std::endl;
   std::cout << "sum: " << sum << std::endl;
#ifdef DEBUG
   tmb::draw_dot_graph(&(test_nodes[0]), 5);
#endif
   int_nodes_1.clear();
   int_nodes.clear();
   int_nodes_2.clear();
   int_nodes_3.clear();
   test_nodes.clear();
   return 0;
}
