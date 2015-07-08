#include <iostream>

template <class A>
A cast_value(void* void_ptr) {
   return *static_cast<A*>(void_ptr);
}
