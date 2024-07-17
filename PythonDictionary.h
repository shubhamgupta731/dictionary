#ifndef _PYTHON_DICTIONARY_H_INCLUDED
#define _PYTHON_DICTIONARY_H_INCLUDED
#include <cstdlib>
#include <cstdarg>
#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include "Eigen/Dense"
#include "ShortCutMacros.decl.h"
#include "Observer.h"
#include "Subject.h"
#include "CustomLokiFunctions.decl.h"
#include "loki/HierarchyGenerators.h"
#include "loki/TypeManip.h"
#include "loki/TypeTraits.h"
#include "boost/format.hpp"
#include "boost/lexical_cast.hpp"

template <typename A>
inline const char *typeName(void) {
   return "unknown";
}

HAS_TYPEDEF(ResultType, has_result_type);
HAS_TYPEDEF(ParmList, has_parm_list);

TYPE_NAME_IN_STRING(int, double, unsigned)

template <>
inline const char *typeName<std::string>(void) {
   return "string";
}

template <>
inline const char *typeName<Eigen::Vector3d>(void) {
   return "vector";
}

template <class A>
struct IsClassAFunctor {
   static bool const Result = (has_result_type<A>::Result);
};

template <class A>
std::ostream &operator<<(std::ostream &os, std::vector<A> &) {
   return os;
}

template <class PropertiesType>
struct PythonDictionary;

template <class Return = void,
          class Tlist = Loki::NullType,
          class PropertiesType = Tlist>
struct FunctorWrapper {
  public:
   Loki::Functor<Return, Tlist> *_func;
   Loki::Functor<Return> _func_no_args;
   std::vector<std::string> _list_of_strings;

   PythonDictionary<PropertiesType> *_dict;
   typedef Return ResultType;
   typedef Tlist ListArgs;
   FunctorWrapper(Loki::Functor<ResultType, Tlist> &func,
                  const std::vector<std::string> &list_of_strings,
                  PythonDictionary<PropertiesType> &dict)
       : _func(&func), _list_of_strings(list_of_strings), _dict(&dict) {
      _func_no_args = IterateOverArgs<ListArgs, 0>::doF(*this, *_func);
   }
   FunctorWrapper(const FunctorWrapper &obj)
       : _func(obj._func)
       , _list_of_strings(obj._list_of_strings)
       , _func_no_args(obj._func_no_args)
       , _dict(obj._dict) {}

   FunctorWrapper() : _func(NULL), _dict(NULL) {}

   template <class Tlist_args, int N>
   struct IterateOverArgs {
      static Loki::Functor<Return> &doF(
          FunctorWrapper obj,
          Loki::Functor<ResultType, Tlist_args> &func_original);
   };

   template <class Head, class Tail, int N>
   struct IterateOverArgs<Loki::Typelist<Head, Tail>, N> {
      static Loki::Functor<Return> doF(
          FunctorWrapper<ResultType, Tlist, PropertiesType> &obj,
          Loki::Functor<ResultType, Loki::Typelist<Head, Tail> > &
              func_original) {
         typedef typename Loki::Select<
             Loki::TypeTraits<Head>::isConst,
             typename Loki::TypeTraits<Head>::NonConstType,
             Head>::Result VarTypeRefOrValue;
         typedef typename Loki::Select<
             Loki::TypeTraits<VarTypeRefOrValue>::isReference,
             typename Loki::TypeTraits<VarTypeRefOrValue>::ReferredType,
             VarTypeRefOrValue>::Result VarType;
         VarType *val = (((*(obj._dict))[((obj._list_of_strings)[N])]));
         Loki::Functor<ResultType, Tail> newfunc(
             tmb::BindFirstRef(func_original, *val));
         return FunctorWrapper<ResultType, Tlist, PropertiesType>::
             template IterateOverArgs<Tail, N + 1>::doF(obj, newfunc);
      }
   };

   template <class Tail, int N>
   struct IterateOverArgs<Loki::Typelist<Tail, Loki::NullType>, N> {
      static Loki::Functor<Return> doF(
          FunctorWrapper<ResultType, Tlist, PropertiesType> &obj,
          Loki::Functor<ResultType, Loki::Typelist<Tail, Loki::NullType> > &
              func_original) {
         typedef typename Loki::Select<
             Loki::TypeTraits<Tail>::isConst,
             typename Loki::TypeTraits<Tail>::NonConstType,
             Tail>::Result VarTypeRefOrValue;
         typedef typename Loki::Select<
             Loki::TypeTraits<VarTypeRefOrValue>::isReference,
             typename Loki::TypeTraits<VarTypeRefOrValue>::ReferredType,
             VarTypeRefOrValue>::Result VarType;
         VarType *val = (((*(obj._dict))[((obj._list_of_strings)[N])]));
         Loki::Functor<ResultType> newfunc(
             tmb::BindFirstRef(func_original, *val));
         return newfunc;
      }
   };

   template <int N>
   struct IterateOverArgs<Loki::NullType, N> {
      static Loki::Functor<ResultType> doF(
          FunctorWrapper<ResultType, Tlist, PropertiesType> &obj,
          Loki::Functor<ResultType> &func_original) {
         return func_original;
      }
   };

   operator ResultType();
};
template <class Return, class Tlist, class PropertiesType>
std::ostream &operator<<(std::ostream &os,
                         FunctorWrapper<Return, Tlist, PropertiesType> &);
template <class PropertiesType>
std::ostream &operator<<(std::ostream &os, PythonDictionary<PropertiesType> &);
//
// template <class A, class PropertiesType>
// A &operator<<(A &obj, PythonDictionary<PropertiesType> &);

inline void hello_world() { std::cout << "Hello World!!!" << std::endl; }

template <class PropertiesType_List>
class PythonDictionaryObserver : public Observer {
   PythonDictionary<PropertiesType_List> *_dict;
   std::string _var_name;

  public:
   PythonDictionaryObserver(PythonDictionary<PropertiesType_List> &dict,
                            std::string var_name)
       : Observer(), _var_name(var_name) {
      _dict = &dict;
   }

   void update();
};

template <class PropertiesType_List, class ReturnType, class Args>
class CustomPythonDictionaryObserver : public Observer {
   std::vector<std::string> _subject;
   std::vector<std::string> _observer;
   PythonDictionary<PropertiesType_List> *_dict;
   FunctorWrapper<ReturnType, Args, PropertiesType_List> *_func;
   Loki::Functor<> *_func_no_args;

  public:
   void update_result();
   CustomPythonDictionaryObserver(PythonDictionary<PropertiesType_List> &dict,
                                  const std::vector<std::string> &subject,
                                  std::string observer,
                                  Loki::Functor<ReturnType, Args> &func,
                                  bool change_functor = false)
       : Observer(), _dict(&dict), _subject(subject) {
      _observer.push_back(observer);
      _func_no_args = new Loki::Functor<>(
          this,
          &CustomPythonDictionaryObserver<PropertiesType_List,
                                          ReturnType,
                                          Args>::update_result);
      _func = new FunctorWrapper<ReturnType, Args, PropertiesType_List>(
          func, subject, dict);
      if (change_functor)
         _dict->_var_holder[observer] = &_func_no_args;
      PythonDictionary<PropertiesType_List>::_recursive[observer] = false;
   }

   CustomPythonDictionaryObserver(PythonDictionary<PropertiesType_List> &dict,
                                  const std::vector<std::string> &subject,
                                  const std::vector<std::string> &observer,
                                  Loki::Functor<ReturnType, Args> &func,
                                  bool change_functor = false)
       : Observer(), _dict(&dict), _subject(subject), _observer(observer) {
      _func_no_args = new Loki::Functor<>(
          this,
          &CustomPythonDictionaryObserver<PropertiesType_List,
                                          ReturnType,
                                          Args>::update_result);
      _func = new FunctorWrapper<ReturnType, Args, PropertiesType_List>(
          func, subject, dict);
      if (change_functor)
         for (std::vector<std::string>::iterator it = _observer.begin();
              it != _observer.end();
              ++it) {
            _dict->_var_holder[*it] = &_func_no_args;
            PythonDictionary<PropertiesType_List>::_recursive[*it] = false;
         }
   }

   void update();
};

template <bool flag>
struct SetValuesFromReturnType {
   template <class A, class B, class PropertiesType_List>
   static void doF(A &a,
                   B &b,
                   int N,
                   PythonDictionary<PropertiesType_List> &dict,
                   bool element_wise) {
      if (element_wise)
         dict[a] = b[N];
   }
};

template <>
struct SetValuesFromReturnType<false> {
   template <class A, class B, class PropertiesType_List>
   static void doF(
       A &a, B &b, int, PythonDictionary<PropertiesType_List> &dict, bool) {
      dict[a] = b;
   }
};

template <class PropertiesType_List, class A>
class TimeObserver : public Observer {
   PythonDictionary<PropertiesType_List> *_dict;
   std::string _var_name;
   bool _upto_date;

  public:
   TimeObserver(PythonDictionary<PropertiesType_List> &dict,
                std::string var_name)
       : Observer(), _var_name(var_name), _upto_date(false) {
      _dict = &dict;
   }
   void update();
};

template <class PropertiesType_List>
struct PythonDictionary {

   typedef typename tmb::CreateListOfClassWithTypedefOrMemFunc<
       PropertiesType_List,
       has_result_type>::List PropertiesType_Which_Are_Functors;
   typedef typename tmb::Subtract<PropertiesType_List,
                                  PropertiesType_Which_Are_Functors>::Result
       PropertiesType_Without_Functors;
   typedef typename tmb::PythonDictionaryListForFunctors<
       PropertiesType_Which_Are_Functors,
       FunctorWrapper,
       PropertiesType_List>::List FunctorWrapperList;
   typedef typename Loki::TL::Append<PropertiesType_Without_Functors,
                                     FunctorWrapperList>::Result PropertiesType;

   template <class List>
   struct IterateForMapPtr;

   template <class Head, class Tail>
   struct IterateForMapPtr<Loki::Typelist<Head, Tail> > {
      static void doF(Loki::Tuple<typename tmb::CreateMapOfVectorsFromList<
                          PropertiesType,
                          std::string>::List> &_holder,
                      std::vector<void *> &vec_ptr) {
         Loki::Field<Loki::TL::IndexOf<PropertiesType, Head>::value>(_holder) =
             NULL;
         vec_ptr[Loki::TL::IndexOf<PropertiesType, Head>::value] =
             &(Loki::Field<Loki::TL::IndexOf<PropertiesType, Head>::value>(
                 _holder));
         IterateForMapPtr<Tail>::doF(_holder, vec_ptr);
      }
   };

   template <class Tail>
   struct IterateForMapPtr<Loki::Typelist<Tail, Loki::NullType> > {
      static void doF(Loki::Tuple<typename tmb::CreateMapOfVectorsFromList<
                          PropertiesType,
                          std::string>::List> &_holder,
                      std::vector<void *> &vec_ptr) {
         Loki::Field<Loki::TL::IndexOf<PropertiesType, Tail>::value>(_holder) =
             NULL;
         vec_ptr[Loki::TL::IndexOf<PropertiesType, Tail>::value] =
             &(Loki::Field<Loki::TL::IndexOf<PropertiesType, Tail>::value>(
                 _holder));
      }
   };

   template <class List>
   struct IterateForTempPtr;

   template <class Head, class Tail>
   struct IterateForTempPtr<Loki::Typelist<Head, Tail> > {
      typedef typename tmb::Subtract<
          PropertiesType,
          typename tmb::CreateListOfClassWithTypedefOrMemFunc<
              PropertiesType,
              has_result_type>::List>::Result ListOfClassesNotFunctor;
      static void doF(Loki::Tuple<ListOfClassesNotFunctor> &_holder,
                      std::vector<void *> &vec_ptr) {
         vec_ptr[Loki::TL::IndexOf<ListOfClassesNotFunctor, Head>::value] =
             &(Loki::Field<
                 Loki::TL::IndexOf<ListOfClassesNotFunctor, Head>::value>(
                 _holder));
         IterateForTempPtr<Tail>::doF(_holder, vec_ptr);
      }
   };

   typedef typename tmb::Subtract<
       PropertiesType,
       typename tmb::CreateListOfClassWithTypedefOrMemFunc<
           PropertiesType,
           has_result_type>::List>::Result ListOfClassesNotFunctor;

   template <class Tail>
   struct IterateForTempPtr<Loki::Typelist<Tail, Loki::NullType> > {
      static void doF(Loki::Tuple<ListOfClassesNotFunctor> &_holder,
                      std::vector<void *> &vec_ptr) {
         vec_ptr[Loki::TL::IndexOf<ListOfClassesNotFunctor, Tail>::value] =
             &(Loki::Field<
                 Loki::TL::IndexOf<ListOfClassesNotFunctor, Tail>::value>(
                 _holder));
      }
   };

   PythonDictionary() {
      _map_ptr_holder.resize(Loki::TL::Length<PropertiesType>::value, NULL);
      _temp_ptr_holder.resize(Loki::TL::Length<PropertiesType>::value);
      IterateForMapPtr<PropertiesType>::doF(_holder, _map_ptr_holder);
      IterateForTempPtr<ListOfClassesNotFunctor>::doF(_temp_holder,
                                                      _temp_ptr_holder);
      _compute_var_functor =
          Loki::Functor<>(this, &PythonDictionary::compute_var);
      _solved_var_functor =
          Loki::Functor<>(this, &PythonDictionary::solved_var);
      _setting_old = false;

      _evaluate_functor =
          Loki::Functor<>(this, &PythonDictionary::evaluate_function);
      _error_key_functor = Loki::Functor<>(this, &PythonDictionary::error_key);
      _evaluate_ptr = &_evaluate_functor;
   }

   PythonDictionary(const PythonDictionary<PropertiesType_List> &) {
      _map_ptr_holder.resize(Loki::TL::Length<PropertiesType>::value, NULL);
      _temp_ptr_holder.resize(Loki::TL::Length<PropertiesType>::value);
      IterateForMapPtr<PropertiesType>::doF(_holder, _map_ptr_holder);
      IterateForTempPtr<ListOfClassesNotFunctor>::doF(_temp_holder,
                                                      _temp_ptr_holder);
      _compute_var_functor =
          Loki::Functor<>(this, &PythonDictionary::compute_var);
      _solved_var_functor =
          Loki::Functor<>(this, &PythonDictionary::solved_var);
      _setting_old = false;

      _evaluate_functor =
          Loki::Functor<>(this, &PythonDictionary::evaluate_function);
      _error_key_functor = Loki::Functor<>(this, &PythonDictionary::error_key);
      _evaluate_ptr = &_evaluate_functor;
   }

   template <class A>
   void register_variable(const char *name) {
      A temp;
      (*this)[name] = temp;
   }

   template <class A>
   void register_variable(const std::vector<const char *> &vec_vars,
                          A default_val) {
      for (std::vector<const char *>::const_iterator it = vec_vars.begin();
           it != vec_vars.end();
           ++it)
         if (!key_found(*it))
            (*this)[*it] = default_val;
   }

   template <class A>
   void register_variable(A default_val, int count, ...) {
      std::vector<const char *> vec_args;
      va_list args;
      va_start(args, count);
      for (int i = 0; i < count; ++i)
         vec_args.push_back(va_arg(args, const char *));
      va_end(args);
      register_variable<A>(vec_args, default_val);
   }

   bool _setting_old;
   typedef std::vector<std::string>::iterator key_iterator;

   Loki::Functor<> _compute_var_functor, _solved_var_functor, _evaluate_functor,
       _error_key_functor;
   Loki::Functor<> *_evaluate_ptr;
   std::map<std::string, unsigned> _var_type_holder;
   std::vector<std::string> _key_list;
   std::vector<Observer *> _time_observer_list;
   std::map<std::string, Loki::Functor<> **> _var_holder;
   std::map<std::string, Loki::Functor<> *> _compute_var_holder,
       _solved_var_holder;
   std::vector<void *> _map_ptr_holder, _temp_ptr_holder;
   std::map<std::string, void *> _val_holder;
   std::map<std::string, Subject *> _subject_holder;
   std::map<std::string, Observer *> _observer_holder;
   std::string _last_string;
   void *_last_var;
   unsigned _var_type;
   Loki::Tuple<ListOfClassesNotFunctor> _temp_holder;
   Loki::Tuple<typename tmb::CreateMapOfVectorsFromList<PropertiesType,
                                                        std::string>::List>
       _holder;
   static std::map<std::string, bool> _recursive;

   key_iterator begin() { return _key_list.begin(); }
   key_iterator end() { return _key_list.end(); }
   size_t size() { return _key_list.size(); }

   PythonDictionary &operator[](std::string var_string) {
      std::transform(
          var_string.begin(), var_string.end(), var_string.begin(), ::tolower);
      if (key_found(var_string)) {
         _evaluate_ptr = &_evaluate_functor;
      } else
         _evaluate_ptr = &_error_key_functor;
      _last_string = var_string;
      return *this;
   }

   void evaluate_function() {
      (**_var_holder[_last_string])();
      _last_var = &(_val_holder[_last_string]);
   }

   void error_key() {
      std::cout << boost::format(
                       "Please note the key %1% is not present in this "
                       "dictionary\n") %
                       _last_string;
      assert(false);
   }

   PythonDictionary &operator[](const char *var_string) {
      std::string name(var_string);
      return (*this)[name];
   }

   void compute_var() {
      if (tmb::IterateTypesIf_String<
              typename tmb::Subtract<
                  PropertiesType,
                  typename tmb::CreateListOfClassWithTypedefOrMemFunc<
                      PropertiesType,
                      has_result_type>::List>::Result,
              FindKeyAndStoreInTempPtr>::doF(_last_string, &_last_var, this)) {
      } else {
         if (tmb::IterateTypesIf_String<
                 typename tmb::CreateListOfClassWithTypedefOrMemFunc<
                     PropertiesType,
                     has_result_type>::List,
                 FindKeyAndStoreInTempPtrFunctor>::doF(_last_string,
                                                       &_last_var,
                                                       this)) {
         }
      }
      _subject_holder[_last_string]->notify();
   }

   void solved_var() {
      _var_type = _var_type_holder[_last_string];
      _last_var = &(_val_holder[_last_string]);
   }

   template <class A>
   PythonDictionary<PropertiesType_List> &operator=(
       PythonDictionary<A> &copy_from) {
      copy_from.copy_all_keys(*this);
      return *this;
   }

   template <class A>
   A &operator=(A val) {
      typedef char PLEASE_ADD_THIS_TYE_TO_PROPERTIES_TYPE_LIST
          [Loki::TL::IndexOf<PropertiesType, A>::value];
      if (strlen(_last_string.c_str())) {
         if (std::find(_key_list.begin(), _key_list.end(), _last_string) ==
             _key_list.end()) {
            _key_list.push_back(_last_string);
            _subject_holder[_last_string] = new Subject();
            //_object_holder[_last_string] = new PythonDictionaryObserver(
            //    *this, _subject_holder[_last_string], _last_string);
            _compute_var_holder[_last_string] = &_compute_var_functor;
            _solved_var_holder[_last_string] = &_solved_var_functor;
            _var_holder[_last_string] = &(_compute_var_holder[_last_string]);
            _var_type_holder[_last_string] =
                Loki::TL::IndexOf<PropertiesType, A>::value;
            /*if (!_setting_old &&
                (_last_string.size() > 4
                     ? (_last_string.substr(_last_string.size() - 4) != "_old")
                     : true)) {
               std::string name_old(_last_string);
               std::string temp = _last_string;
               name_old.append("_old");
               _setting_old = true;
               (*this)[name_old.c_str()] = val;
               _time_observer_list.push_back(
                   new TimeObserver<PropertiesType_List, A>(*this, name_old));
               _setting_old = false;
               _last_string = temp;
            }*/
         } else {
            // tmb::IterateTypesIf_ConstChar<
            //    typename tmb::Subtract<
            //        PropertiesType,
            //        Loki::Typelist<A, Loki::NullType> >::Result,
            //    DeleteKeyFromMap>::doF(_last_string, this);
         }
         std::map<std::string, A> *ptr =
             *static_cast<std::map<std::string, A> **>(
                 _map_ptr_holder[Loki::TL::IndexOf<PropertiesType, A>::value]);
         if (!ptr) {
            Loki::Field<Loki::TL::IndexOf<PropertiesType, A>::value>(_holder) =
                new std::map<std::string, A>();
            ptr = *static_cast<std::map<std::string, A> **>(
                      _map_ptr_holder[Loki::TL::IndexOf<PropertiesType,
                                                        A>::value]);
         }
         _subject_holder[_last_string]->notify();
         (*ptr)[_last_string] = val;
         _last_var = _val_holder[_last_string] =
             &(ptr->find(_last_string)->second);
         _var_holder[_last_string] = &(_solved_var_holder[_last_string]);
         _var_type = Loki::TL::IndexOf<PropertiesType, A>::value;
         // solved_var();
         //_subject_holder[std::find(
         //                    _key_list.begin(), _key_list.end(),
         //                    _last_string)
         //                    -
         //                _key_list.begin()]->notify();
         //_last_string = "";
         //_var_type = std::numeric_limits<unsigned>::max();
         return ptr->find(_last_string)->second;
      } else {
         return (*const_cast<A *>(static_cast<const A *>(_last_var))) = val;
      }
      _evaluate_ptr = NULL;
   }

   template <class A>
   PythonDictionary<PropertiesType_List> &assign(std::string key, A val) {
      (*this)[key] = val;
      return *this;
   }

   PythonDictionary<PropertiesType_List> &clear() {
      _key_list.clear();
      return *this;
   }

   void make_time_backup() {
      for (std::map<std::string, bool>::iterator it =
               PythonDictionary<PropertiesType_List>::_recursive.begin();
           it != PythonDictionary<PropertiesType_List>::_recursive.end();
           ++it) {
         it->second = true;
      }
      for (std::vector<Observer *>::iterator it = _time_observer_list.begin();
           it != _time_observer_list.end();
           ++it)
         (*it)->update();
      for (std::vector<Observer *>::iterator it = _time_observer_list.begin();
           it != _time_observer_list.end();
           ++it)
         (*it)->update();
      for (std::map<std::string, bool>::iterator it =
               PythonDictionary<PropertiesType_List>::_recursive.begin();
           it != PythonDictionary<PropertiesType_List>::_recursive.end();
           ++it) {
         it->second = false;
      }
   }

   bool key_found(std::string key) const {
      return (std::find(_key_list.begin(), _key_list.end(), key) !=
              _key_list.end());
   }

   template <class A>
   operator A *() {
      std::string temp(_last_string);
      if (_evaluate_ptr)
         (*_evaluate_ptr)();
      _last_string = "";
      std::map<std::string, A> *ptr =
          *static_cast<std::map<std::string, A> **>(
              _map_ptr_holder[Loki::TL::IndexOf<PropertiesType, A>::value]);
      return &((*ptr).find(temp)->second);
   }

   template <class A>
   struct OutputDict {
      template <class Ostream, class Dict>
      static bool doF(std::string name, Ostream *os, Dict *dict) {
         std::map<std::string, A> *ptr =
             *static_cast<std::map<std::string, A> **>(
                 dict->_map_ptr_holder[Loki::TL::IndexOf<PropertiesType,
                                                         A>::value]);
         if (ptr && ptr->find(name) != ptr->end()) {
            A temp = (*dict)[name];
            (*os) << temp;
            return true;
         } else
            return false;
      }
   };

   template <class A>
   struct OutputDictAll {
      template <class Ostream, class Dict>
      static void doF(Ostream &os, Dict &dict) {
         std::map<std::string, A> *ptr =
             *static_cast<std::map<std::string, A> **>(
                 dict._map_ptr_holder[Loki::TL::IndexOf<PropertiesType,
                                                        A>::value]);
         if (ptr)
            for (typename std::map<std::string, A>::iterator it = ptr->begin();
                 it != ptr->end();
                 ++it) {
               A temp = dict[it->first];
               os << it->first << ": " << temp << std::endl;
            }
      }
   };

   template <class A>
   struct CreateSetOfKeyTypeValue {
      template <class VectorOfPairOfStringsAndPairOfStrings, class Dict>
      static void doF(VectorOfPairOfStringsAndPairOfStrings &return_value,
                      Dict &dict) {
         std::pair<std::string, std::string> type_value;
         std::map<std::string, A> *ptr =
             *static_cast<std::map<std::string, A> **>(
                 dict._map_ptr_holder[Loki::TL::IndexOf<PropertiesType,
                                                        A>::value]);
         if (ptr)
            for (typename std::map<std::string, A>::iterator it = ptr->begin();
                 it != ptr->end();
                 ++it) {
               A temp = dict[it->first];
               std::stringstream ss;
               ss << temp;
               std::string name(it->first);
               std::transform(
                   name.begin(), name.end(), name.begin(), ::toupper);
               std::string val(ss.str());
               std::string type_name(typeName<A>());
               type_value = std::make_pair(name, val);
               return_value.push_back(
                   std::make_pair(typeName<A>(), type_value));
            }
      }
   };

   template <class A>
   struct DeleteKeyFromMap {
      template <class Dict>
      static bool doF(std::string name, Dict *dict) {
         std::map<std::string, A> *ptr =
             *static_cast<std::map<std::string, A> **>(
                 dict->_map_ptr_holder[Loki::TL::IndexOf<PropertiesType,
                                                         A>::value]);
         typename std::map<std::string, A>::iterator it = ptr->find(name);
         if (it != ptr->end()) {
            ptr->erase(it);
            return true;
         } else
            return false;
      }
   };

   template <class A>
   struct FindKeyAndStoreInTemp {
      template <class TempType, class Dict>
      static bool doF(std::string name, TempType *temp, Dict *dict) {
         std::map<std::string, A> *ptr =
             *static_cast<std::map<std::string, A> **>(
                 dict->_map_ptr_holder[Loki::TL::IndexOf<PropertiesType,
                                                         A>::value]);
         typename std::map<std::string, A>::iterator it = ptr->find(name);
         if (it != ptr->end()) {
            dict->_val_holder[name] = &(it->second);
            dict->_var_holder[name] = &(dict->_solved_var_holder[name]);
            (*temp) = &(dict->_val_holder[name]);
            return true;
         } else
            return false;
      }
   };

   template <class A>
   struct FindNonOldVersionOfKey {
      template <class Dict>
      static bool doF(std::string name_old, std::string *name, Dict *dict) {
         std::map<std::string, A> *ptr =
             *static_cast<std::map<std::string, A> **>(
                 dict->_map_ptr_holder[Loki::TL::IndexOf<PropertiesType,
                                                         A>::value]);
         if (ptr) {
            typename std::map<std::string, A>::iterator it = ptr->find(*name);
            if (it != ptr->end()) {
               A temp = (*dict)[*name];
               (*dict)[name_old] = temp;
               dict->_time_observer_list.push_back(
                   new TimeObserver<PropertiesType_List, A>(*dict, name_old));
               return true;
            } else
               return false;
         } else
            return false;
      }
   };

   template <class A>
   void copy_all_keys(PythonDictionary<A> &dict) {
      for (key_iterator it_args = begin(); it_args != end(); ++it_args)
         if (tmb::IterateTypesIf_String<
                 typename tmb::Subtract<
                     PropertiesType,
                     typename tmb::CreateListOfClassWithTypedefOrMemFunc<
                         PropertiesType,
                         has_result_type>::List>::Result,
                 FindKeyAndStoreInTempPtr>::doF(*it_args, &dict, this)) {
         }
   }

   template <class A>
   struct FindKeyAndStoreInTempPtr {
      template <class TempType, class Dict>
      static bool doF(std::string name, TempType *temp, Dict *dict) {
         std::map<std::string, A> *ptr =
             *static_cast<std::map<std::string, A> **>(
                 dict->_map_ptr_holder[Loki::TL::IndexOf<PropertiesType,
                                                         A>::value]);
         if (ptr) {
            typename std::map<std::string, A>::iterator it = ptr->find(name);
            if (it != ptr->end()) {
               dict->_val_holder[name] = &(it->second);
               dict->_var_holder[name] = &(dict->_solved_var_holder[name]);
               (*temp) = &(dict->_val_holder[name]);
               dict->_var_type = Loki::TL::IndexOf<PropertiesType, A>::value;
               // dict->_subject_holder[name]->notify();
               return true;
            } else
               return false;
         } else
            return false;
      }
      template <class TempType, class Dict>
      static bool doF(std::string name,
                      PythonDictionary<TempType> *temp,
                      Dict *dict) {
         std::map<std::string, A> *ptr =
             *static_cast<std::map<std::string, A> **>(
                 dict->_map_ptr_holder[Loki::TL::IndexOf<PropertiesType,
                                                         A>::value]);
         if (ptr) {
            typename std::map<std::string, A>::iterator it = ptr->find(name);
            if (it != ptr->end()) {
               dict->_val_holder[name] = &(it->second);
               dict->_var_holder[name] = &(dict->_solved_var_holder[name]);
               (*temp)[name] = it->second;
               return true;
            } else
               return false;
         } else
            return false;
      }
   };

   template <class A, bool flag>
   struct FindKeyAndStoreInTempPtrFunctorIfNotVoid {
      template <class TempType, class Dict>
      static bool doF(std::string name, TempType *temp, Dict *dict) {
         std::map<std::string, A> *ptr =
             *static_cast<std::map<std::string, A> **>(
                 dict->_map_ptr_holder[Loki::TL::IndexOf<PropertiesType,
                                                         A>::value]);
         typename A::ResultType *ptr_var =
             static_cast<typename A::ResultType *>(
                 dict->_temp_ptr_holder[Loki::TL::IndexOf<
                     ListOfClassesNotFunctor,
                     typename A::ResultType>::value]);
         if (ptr) {
            typename std::map<std::string, A>::iterator it = ptr->find(name);
            if (it != ptr->end()) {
               (*ptr_var) = static_cast<typename A::ResultType>(it->second);
               dict->_val_holder[name] = ptr_var;
               dict->_var_holder[name] = &(dict->_solved_var_holder[name]);
               (*temp) = &(dict->_val_holder[name]);
               dict->_var_type =
                   Loki::TL::IndexOf<PropertiesType,
                                     typename A::ResultType>::value;
               return true;
            } else
               return false;
         } else
            return false;
      }
   };

   template <class A>
   struct FindKeyAndStoreInTempPtrFunctorIfNotVoid<A, true> {
      template <class TempType, class Dict>
      static bool doF(std::string name, TempType *temp, Dict *dict) {
         return false;
      }
   };
   template <class A>
   struct FindKeyAndStoreInTempPtrFunctor {
      template <class TempType, class Dict>
      static bool doF(std::string name, TempType *temp, Dict *dict) {
         return FindKeyAndStoreInTempPtrFunctorIfNotVoid<
             A,
             tmb::isVoid<typename A::ResultType>::value>::doF(name, temp, dict);
      }
   };

   template <class A, int N>
   struct RegisterVariable {
      static bool doF(PythonDictionary<PropertiesType_List> &ptr_name,
                      int *number_checked) {
         if (*number_checked == N) {
            ptr_name.register_variable<A>(ptr_name._last_string);
            return true;
         } else
            return false;
      }
   };

   void check_registered_variable_old(std::string name) {
      std::string name_new(name.substr(0, name.size() - 4));
      if (key_found(name_new)) {
         // name is basically a registered variable with _old appended
         // so we will register the variable and add a time observer
         // so that it is updated every time make_time_backup is called
         if (tmb::IterateTypesIf_String<PropertiesType,
                                        FindNonOldVersionOfKey>::doF(name,
                                                                     &name_new,
                                                                     this)) {
            return;
         } else {
            std::cerr << boost::format(
                             "The key %1% is not present in the dictionary") %
                             name << std::endl;
            assert(false);
         }
      }
   }

   template <class ResultType, class TList>
   void registerFunc(std::string name,
                     Loki::Functor<ResultType, TList> &functor,
                     std::vector<std::string> &args) {
      bool change_functor = false;
      if (std::find(_key_list.begin(), _key_list.end(), name) ==
          _key_list.end()) {
         registerFunctorVariable(name);
         change_functor = true;
      }
      std::vector<std::string> *arglist = new std::vector<std::string>;
      assert(Loki::TL::Length<TList>::value == args.size());
      _compute_var_holder[name] = &_compute_var_functor;
      _solved_var_holder[name] = &_solved_var_functor;
      _var_holder[name] = &(_compute_var_holder[name]);
      for (int i = 0; i < args.size(); ++i) {
         if (!key_found(args[i]))
            check_registered_variable_old(args[i]);
         arglist->push_back(args[i]);
      }
      _observer_holder[name] =
          new CustomPythonDictionaryObserver<PropertiesType_List,
                                             ResultType,
                                             TList>(
              *this, *arglist, name, functor, change_functor);
      _var_type_holder[name] =
          Loki::TL::IndexOf<PropertiesType, ResultType>::value;
      for (unsigned i = 0; i < args.size(); ++i)
         _subject_holder[args[i]]->attach(_observer_holder[name]);
      _observer_holder[name]->update();
   }

   void registerFunctorVariable(const std::string &name) {
      _subject_holder[name] = new Subject();
      _compute_var_holder[name] = &_compute_var_functor;
      _solved_var_holder[name] = &_solved_var_functor;
      _var_holder[name] = &(_compute_var_holder[name]);
      _key_list.push_back(name);
   }

   template <class ResultType, class TList, class VarType>
   void registerFunc(std::string name,
                     Loki::Functor<ResultType, TList> &functor,
                     std::vector<std::string> &args,
                     VarType *val,
                     Subject *subject) {
      bool change_functor = false;
      std::vector<std::string> *arglist = new std::vector<std::string>;
      if (std::find(_key_list.begin(), _key_list.end(), name) ==
          _key_list.end()) {
         registerFunctorVariable(name);
         change_functor = true;
      }
      Loki::Functor<
          ResultType,
          typename tmb::Subtract<
              TList,
              Loki::Typelist<typename TList::Head, Loki::NullType> >::Result>
          final_func(tmb::BindFirstRef(functor, *val));
      for (unsigned i = 0; i < args.size(); ++i) {
         if (!key_found(args[i]))
            check_registered_variable_old(args[i]);
         arglist->push_back(args[i]);
      }
      _observer_holder[name] = new CustomPythonDictionaryObserver<
          PropertiesType_List,
          ResultType,
          typename tmb::Subtract<
              TList,
              Loki::Typelist<typename TList::Head, Loki::NullType> >::Result>(
          *this, *arglist, name, final_func, change_functor);
      _var_type_holder[name] =
          Loki::TL::IndexOf<PropertiesType, ResultType>::value;
      subject->attach(_observer_holder[name]);
      for (unsigned i = 0; i < args.size(); ++i)
         _subject_holder[args[i]]->attach(_observer_holder[name]);
      _observer_holder[name]->update();
   }

   template <class ResultType, class TList, class VarType>
   void registerFunc(std::string name,
                     Loki::Functor<ResultType, TList> &functor,
                     VarType *val,
                     Subject *subject,
                     int count,
                     ...) {
      std::vector<std::string> arglist;
      va_list args;
      va_start(args, count);
      for (int i = 0; i < count; ++i) {
         const char *arg_char = va_arg(args, const char *);
         if (!key_found(arg_char))
            check_registered_variable_old(arg_char);
         arglist.push_back(arg_char);
      }
      va_end(args);
      registerFunc(name, functor, arglist, val, subject);
   }

   template <class ResultType, class TList, class VarType>
   void registerFunc(const std::vector<std::string> &vec_name,
                     Loki::Functor<ResultType, TList> &functor,
                     std::vector<std::string> &args,
                     VarType *val,
                     Subject *subject) {
      bool change_functor = false;
      std::vector<std::string> *arglist = new std::vector<std::string>;
      Loki::Functor<
          ResultType,
          typename tmb::Subtract<
              TList,
              Loki::Typelist<typename TList::Head, Loki::NullType> >::Result>
          final_func(tmb::BindFirstRef(functor, *val));
      for (unsigned i = 0; i < args.size(); ++i) {
         if (!key_found(args[i]))
            check_registered_variable_old(args[i]);
         arglist->push_back(args[i]);
      }
      for (std::vector<std::string>::const_iterator it = vec_name.begin();
           it != vec_name.end();
           ++it) {
         _observer_holder[*it] = new CustomPythonDictionaryObserver<
             PropertiesType_List,
             ResultType,
             typename tmb::Subtract<TList,
                                    Loki::Typelist<typename TList::Head,
                                                   Loki::NullType> >::Result>(
             *this, *arglist, *it, final_func, change_functor);
         _var_type_holder[*it] =
             Loki::TL::IndexOf<PropertiesType, ResultType>::value;
         subject->attach(_observer_holder[*it]);
         for (unsigned i = 0; i < args.size(); ++i)
            _subject_holder[args[i]]->attach(_observer_holder[*it]);
         _observer_holder[*it]->update();
      }
   }

   template <class ResultType, class TList>
   void registerFunc(const std::vector<std::string> &vec_name,
                     Loki::Functor<ResultType, TList> &functor,
                     const std::vector<std::string> &args) {
      bool change_functor = false;
      std::vector<std::string> *arglist = new std::vector<std::string>;
      for (unsigned i = 0; i < args.size(); ++i) {
         if (!key_found(args[i]))
            check_registered_variable_old(args[i]);
         arglist->push_back(args[i]);
      }
      Observer *new_obs =
          new CustomPythonDictionaryObserver<PropertiesType_List,
                                             ResultType,
                                             TList>(
              *this, *arglist, vec_name, functor, change_functor);
      //_var_type_holder[*it] =
      //    Loki::TL::IndexOf<PropertiesType, ResultType>::value;
      for (unsigned i = 0; i < args.size(); ++i)
         _subject_holder[args[i]]->attach(new_obs);
   }

   // template <class ResultType, class TList, class ObserverList>
   // void registerFunc(
   //    std::string name,
   //    Loki::Functor<ResultType, TList> &functor,
   //    std::vector<std::string > &args,
   //    Loki::Tuple<typename tmb::CreatePointerList<ObserverList>::List> &
   //        tuple_vars,
   //    std::vector<Observer *> &vec_observers) {
   //   std::vector<std::string > *arglist = new std::vector<std::string >;
   //   assert(Loki::TL::Length<TList>::value == args.size());
   //   _compute_var_holder[name] = &_compute_var_functor;
   //   _solved_var_holder[name] = &_solved_var_functor;
   //   _var_holder[name] = &(_compute_var_holder[name]);
   //   _observer_holder[name] =
   //       new PythonDictionaryObserver<PropertiesType_List>(*this, name);
   //   for (unsigned i = 0; i < Loki::TL::Length<TList>::value; ++i) {
   //      arglist->push_back(args[i]);
   //      _subject_holder[args[i]]->attach(_observer_holder[name]);
   //   }
   //   FunctorWrapper<ResultType, TList, PropertiesType_List> obj(
   //       functor, *arglist, *this);
   //   (*this)[name] = obj;
   //}

   void attachObserver(Observer *observer) {
      if (std::find(_key_list.begin(), _key_list.end(), _last_string) ==
          _key_list.end())
         registerFunctorVariable(_last_string);
      _subject_holder[_last_string]->attach(observer);
      _last_string = "";
   }

   void attachObserver(std::string name) {
      if (std::find(_key_list.begin(), _key_list.end(), _last_string) ==
          _key_list.end())
         registerFunctorVariable(_last_string);
      _subject_holder[_last_string]->attach(_observer_holder[name]);
      _last_string = "";
   }

   template <class A, class B>
   void attachObserver(std::string name, Loki::Functor<A, B> &func) {
      std::vector<std::string> *vec_args = new std::vector<std::string>;
      vec_args->push_back(_last_string);
      if (std::find(_key_list.begin(), _key_list.end(), _last_string) ==
          _key_list.end())
         registerFunctorVariable(_last_string);
      _subject_holder[_last_string]->attach(
          new CustomPythonDictionaryObserver<PropertiesType_List, A, B>(
              *this, *vec_args, name, func));
      _last_string = "";
   }

   template <class A, class B>
   void attachObserver(const std::vector<std::string> &name,
                       Loki::Functor<A, B> &func) {
      std::vector<std::string> vec_args;
      vec_args.push_back(_last_string);
      if (std::find(_key_list.begin(), _key_list.end(), _last_string) ==
          _key_list.end())
         registerFunctorVariable(_last_string);
      _subject_holder[_last_string]->attach(
          new CustomPythonDictionaryObserver<PropertiesType_List, A, B>(
              *this, vec_args, name, func));
      _last_string = "";
   }

   template <class A, class B>
   void attachObserver(Loki::Functor<A, B> &func, int count, ...) {
      std::vector<std::string> vec_args;
      va_list args;
      va_start(args, count);
      for (int i = 0; i < count; ++i)
         vec_args.push_back(va_arg(args, const char *));
      va_end(args);
      attachObserver(vec_args, func);
   }

   Observer *getObserver(std::string name) { return _observer_holder[name]; }

   template <class A>
   operator A &() const;

   friend std::ostream &operator<<<>(std::ostream &,
                                     PythonDictionary<PropertiesType> &);

   template <class A>
   struct IfSameType {
      template <class Type, class Dict>
      static bool doF(unsigned var_type, Dict *dict) {
         if (var_type == Loki::TL::IndexOf<PropertiesType_List, A>::value) {
            Loki::Field<Loki::TL::IndexOf<ListOfClassesNotFunctor, A>::value>(
                dict->_temp_holder) = **static_cast<A **>(dict->_last_var);
            Loki::Field<
                Loki::TL::IndexOf<ListOfClassesNotFunctor, Type>::value>(
                dict->_temp_holder) =
                Loki::Field<
                    Loki::TL::IndexOf<ListOfClassesNotFunctor, A>::value>(
                    dict->_temp_holder);
            dict->_last_var = &(Loki::Field<
                Loki::TL::IndexOf<ListOfClassesNotFunctor, Type>::value>(
                dict->_temp_holder));
            return true;
         } else
            return false;
      }
   };

   void fill_type_key_value(std::vector<
       std::pair<std::string, std::pair<std::string, std::string> > > &
                                return_value) {
      typedef typename Loki::TL::NoDuplicates<typename Loki::TL::Append<
          typename tmb::CreateListOfClassWithTypedefOrMemFunc<
              PropertiesType,
              tmb::has_insertion_operator>::List,
          typename tmb::CreateListOfClassWithTypedefOrMemFunc<
              PropertiesType,
              tmb::has_non_const_insertion_operator>::List>::Result>::Result
          ListOfClassesWithOstream;
      tmb::IterateTypes<ListOfClassesWithOstream, CreateSetOfKeyTypeValue>::doF(
          return_value, *this);
   }
};

template <class PropertiesType_List>
std::map<std::string, bool> PythonDictionary<PropertiesType_List>::_recursive;

template <class PropertiesType_List>
template <class A>
PythonDictionary<PropertiesType_List>::operator A &() const {
   (*_evaluate_ptr)();
   if (Loki::TL::IndexOf<PythonDictionary<PropertiesType_List>::PropertiesType,
                         A>::value == _var_type) {
      return **static_cast<A **>(_last_var);
   } else {
      typedef typename tmb::Subtract<
          PropertiesType,
          typename tmb::CreateListOfClassWithTypedefOrMemFunc<
              PropertiesType,
              has_result_type>::List>::Result ListOfClassesNotFunctor;
      tmb::IterateTypesIf_SameType<
          typename Loki::Select<
              (Loki::TL::IndexOf<typename tmb::CreateListOfClassWhichArePODs<
                                     PropertiesType>::List,
                                 A>::value > -1),
              typename tmb::CreateListOfClassWhichArePODs<PropertiesType>::List,
              Loki::NullType>::Result,
          IfSameType>::template doF<A>(_var_type,
                                       const_cast<PythonDictionary<
                                           PropertiesType_List> *>(this));
      return *static_cast<A *>(_last_var);
   }
}

/*template <class PropertiesType_List>
template <class A>
PythonDictionary<PropertiesType_List>::operator A &() {
   if (key_found(_last_string)) {
      std::map<std::string , A> *ptr =
          *static_cast<std::map<std::string , A> **>(
              _map_ptr_holder[Loki::TL::IndexOf<PropertiesType, A>::value]);
      typename std::map<std::string , A>::iterator it =
ptr->find(_last_string);
      if (it != ptr->end()) {
         std::string temp = _last_string;
         _last_string = "";
         return (*ptr)[temp];
      } else {
         if (tmb::IterateTypesIf_ConstChar<
                 typename tmb::CreateListOfClassWithTypedefOrMemFunc<
                     typename Loki::Select<
                         Loki::TypeTraits<A>::isStdFundamental,
                         PropertiesType,
                         Loki::NullType>::Result,
                     has_result_type>::List,
                 FindKeyAndStoreInTemp>::
                 doF(_last_string,
                     static_cast<A *>(_temp_ptr_holder[Loki::TL::IndexOf<
                         typename tmb::CreateListOfClassWhichArePODs<
                             PropertiesType>::List,
                         A>::value]),
                     this)) {
            _last_string = "";
         } else {
            if (tmb::IterateTypesIf_ConstChar<
                    typename tmb::Subtract<
                        typename tmb::ListOfAllowedImplicitConversion<
                            typename tmb::CreateListOfClassWhichArePODs<
                                PropertiesType>::List,
                            A>::List,
                        Loki::Typelist<A, Loki::NullType> >::Result,
                    FindKeyAndStoreInTemp>::
                    doF(_last_string,
                        static_cast<A *>(_temp_ptr_holder[Loki::TL::IndexOf<
                            typename tmb::CreateListOfClassWhichArePODs<
                                PropertiesType>::List,
                            A>::value]),
                        this)) {
               _last_string = "";
            }
         }
      }
      std::stringstream err;
      err << "*****************************************************"
             "*" << std::endl;
      err << "Key: " << _last_string
          << " was not found in any dictionary associated with any "
             "map "
             "from which implicit conversion is possible for the "
             "type "
             "specified" << std::endl;
      err << "*****************************************************"
             "*" << std::endl;
      std::cerr << err.str();
      assert(false);
   }
   return *(static_cast<A *>(_temp_ptr_holder[Loki::TL::IndexOf<
       typename tmb::CreateListOfClassWhichArePODs<PropertiesType>::List,
       A>::value]));
}*/

template <class ResultType, class Tlist, class PropertiesType>
FunctorWrapper<ResultType, Tlist, PropertiesType>::operator ResultType() {
   // TODO: Test
   // Ensure every variable has been computed
   for (std::vector<std::string>::const_iterator it = _list_of_strings.begin();
        it != _list_of_strings.end();
        ++it) {
      (*_dict)[*it];
      (*(_dict->_evaluate_ptr))();
      // PythonDictionary<PropertiesType>::_recursive[*it] = true;
   }

   ResultType temp = _func_no_args();
   return temp;
}

template <class ResultType, class Tlist, class PropertiesType>
std::ostream &operator<<(
    std::ostream &os, FunctorWrapper<ResultType, Tlist, PropertiesType> &func) {
   ResultType temp = func;
   os << temp;
   return os;
}

template <class PropertiesType_List>
std::ostream &operator<<(std::ostream &os,
                         PythonDictionary<PropertiesType_List> &dict) {
   typedef typename Loki::TL::Append<
       typename tmb::CreateListOfClassWithTypedefOrMemFunc<
           typename PythonDictionary<PropertiesType_List>::PropertiesType,
           tmb::has_insertion_operator>::List,
       typename tmb::CreateListOfClassWithTypedefOrMemFunc<
           typename PythonDictionary<PropertiesType_List>::PropertiesType,
           tmb::has_non_const_insertion_operator>::List>::Result
       ListOfClassesWithOstream;
   if (dict._last_string.size() && dict.key_found(dict._last_string)) {
      tmb::IterateTypesIf_String<
          ListOfClassesWithOstream,
          PythonDictionary<PropertiesType_List>::template OutputDict>::
          doF(dict._last_string, &os, &dict);
   } else {
      tmb::IterateTypes<
          ListOfClassesWithOstream,
          PythonDictionary<
              PropertiesType_List>::template OutputDictAll>::doF(os, dict);
   }
   return os;
}

template <class A, class PropertiesType_List>
void operator+=(A &a, const PythonDictionary<PropertiesType_List> &dict) {
   static A temp;
   temp = dict;
   a += temp;
}

template <class A, class PropertiesType_List>
void operator+=(PythonDictionary<PropertiesType_List> &dict, const A &a) {
   A *ptr = dict;
   (*ptr) += a;
}

template <class A, class PropertiesType_List>
void operator-=(A &a, const PythonDictionary<PropertiesType_List> &dict) {
   static A temp;
   temp = dict;
   a -= temp;
}

template <class A, class PropertiesType_List>
void operator-=(const PythonDictionary<PropertiesType_List> &dict, const A &a) {
   A *ptr = dict;
   (*ptr) -= a;
}

template <class A, class PropertiesType_List>
void operator*=(A &a, const PythonDictionary<PropertiesType_List> &dict) {
   a *= static_cast<const A &>(dict);
}

template <class A, class PropertiesType_List>
void operator*=(const PythonDictionary<PropertiesType_List> &dict, const A &a) {
   A *ptr = dict;
   (*ptr) *= a;
}

template <class A, class PropertiesType_List>
void operator/=(A &a, const PythonDictionary<PropertiesType_List> &dict) {
   a *= static_cast<const A &>(dict);
}

template <class A, class PropertiesType_List>
void operator/=(const PythonDictionary<PropertiesType_List> &dict, const A &a) {
   A *ptr = dict;
   (*ptr) /= a;
}

template <class PropertiesType_List, class A>
void TimeObserver<PropertiesType_List, A>::update() {

   //_dict->_last_string = _var_name;
   //(*(_dict->_compute_var_holder[_var_name]))();

   std::map<std::string, Loki::Functor<> **> var_holder_bk(_dict->_var_holder);
   A temp = (*_dict)[_var_name.substr(0, _var_name.size() - 4)];
   if (_upto_date) {
      (*_dict)[_var_name] = temp;
      _dict->_var_holder = var_holder_bk;
      _upto_date = false;
   } else
      _upto_date = true;
}

template <class PropertiesType_List>
void PythonDictionaryObserver<PropertiesType_List>::update() {

   //_dict->_last_string = _var_name;
   //(*(_dict->_compute_var_holder[_var_name]))();
   _dict->_var_holder[_var_name] = &(_dict->_compute_var_holder[_var_name]);
   _dict->_subject_holder[_var_name]->notify();
}

template <class PropertiesType_List, class ReturnType, class ArgType>
void CustomPythonDictionaryObserver<PropertiesType_List,
                                    ReturnType,
                                    ArgType>::update() {
   for (std::vector<std::string>::iterator it = _observer.begin();
        it != _observer.end();
        ++it) {
      if (!PythonDictionary<PropertiesType_List>::_recursive[*it]) {
         PythonDictionary<PropertiesType_List>::_recursive[*it] = true;
         _dict->_subject_holder[*it]->notify();
         PythonDictionary<PropertiesType_List>::_recursive[*it] = false;
         _dict->_var_holder[*it] = &_func_no_args;
      }
   }
}

template <class PropertiesType_List, class ReturnType, class ArgType>
void CustomPythonDictionaryObserver<PropertiesType_List,
                                    ReturnType,
                                    ArgType>::update_result() {
   std::string temp_string = _dict->_last_string;
   for (std::vector<std::string>::iterator it = _observer.begin();
        it != _observer.end();
        ++it) {
      _dict->_var_holder[*it] = &_dict->_solved_var_holder[*it];
   }

   ReturnType temp = *_func;

   for (std::vector<std::string>::iterator it = _observer.begin();
        it != _observer.end();
        ++it) {
      SetValuesFromReturnType<tmb::isVector<ReturnType>::Result &&(
          Loki::TL::IndexOf<
              typename PythonDictionary<PropertiesType_List>::PropertiesType,
              ReturnType>::value == -1)>::doF(*it,
                                              temp,
                                              std::distance(_observer.begin(),
                                                            it),
                                              *_dict,
                                              _observer.size() > 1);
   }
   for (std::vector<std::string>::iterator it = _observer.begin();
        it != _observer.end();
        ++it)
      _dict->_var_holder[*it] = &_dict->_solved_var_holder[*it];
   (*_dict)[temp_string];
}

//
// template <class A, class PropertiesType>
// A &operator<<(A &obj, PythonDictionary<PropertiesType> &dict) {
//   obj = (*(Loki::Field<Loki::TL::IndexOf<PropertiesType, A>::value>(
//       dict._holder)))[dict._last_string];
//   dict._last_string = "";
//   return obj;
//}
#endif
