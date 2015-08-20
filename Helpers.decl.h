#ifndef _HELPERS_DECL_H_INCLUDED
#define _HELPERS_DECL_H_INCLUDED
#include <iostream>

// Ref:
// http://stackoverflow.com/questions/5768511/using-sfinae-to-check-for-global-operator
namespace tmb {
   namespace Private {
      namespace has_insertion_operator_impl {
         typedef char no;
         typedef char yes[2];

         struct any_t {
            template <typename T>
            any_t(T const&);
         };

         no operator<<(std::ostream const&, any_t const&);
         no operator<<(std::ostream const&, any_t&);

         yes& test(std::ostream&);
         no test(no);

         template <typename T>
         struct has_insertion_operator {
            static std::ostream& s;
            static T const& t;
            static bool const Result = sizeof(test(s << t)) == sizeof(yes);
         };

         template <typename T>
         struct has_non_const_insertion_operator {
            static std::ostream& s;
            static T& t;
            static bool const Result = sizeof(test(s << t)) == sizeof(yes);
         };
      }

      template <typename T>
      struct has_insertion_operator
          : has_insertion_operator_impl::has_insertion_operator<T> {};

      template <typename T>
      struct has_non_const_insertion_operator
          : has_insertion_operator_impl::has_non_const_insertion_operator<T> {};
   }
}

#endif
