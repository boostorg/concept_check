//
// (C) Copyright Jeremy Siek 2000. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
//
#ifndef BOOST_CONCEPT_ARCHETYPES_HPP
#define BOOST_CONCEPT_ARCHETYPES_HPP

#include <iterator>
#include <functional>

namespace boost {

  //===========================================================================
  // Basic Archetype Classes

  namespace detail {
    class dummy_constructor { };
  }
  static detail::dummy_constructor dummy_cons;

  // A type that models no concept. The template parameter 
  // is only there so that null_archetype types can be created
  // that have different type.
  template <class T = int>
  class null_archetype {
  private:
    null_archetype() { }
    null_archetype(const null_archetype&) { }
    null_archetype& operator=(const null_archetype&) { return *this; }
  public:
    null_archetype(detail::dummy_constructor) { }
    template <class TT>
    friend void dummy_friend(); // just to avoid warnings
  };

  // This is a helper class that provides a way to get a reference to
  // an object.
  template <class T>
  class static_object {
  public:
    static T& get() {
      detail::dummy_constructor d;
      static T x(d);
      return x;
    }
  };
  template <>
  class static_object<bool> {
  public:
    static bool& get() {
      static bool b;
      return b;
    }
  };
  template <>
  class static_object<int> {
  public:
    static int& get() {
      static int b;
      return b;
    }
  };

  template <class Base = null_archetype<> >
  class default_constructible_archetype : public Base {
  public:
    default_constructible_archetype() : Base(dummy_cons) { }
    default_constructible_archetype(detail::dummy_constructor x) : Base(x) { }
  };

  template <class Base = null_archetype<> >
  class assignable_archetype : public Base {
    assignable_archetype() { }
    assignable_archetype(const assignable_archetype&) { }
  public:
    assignable_archetype& operator=(const assignable_archetype&) {
      return *this;
    }
    assignable_archetype(detail::dummy_constructor x) : Base(x) { }
  };

  template <class Base = null_archetype<> >
  class copy_constructible_archetype : public Base {
  public:
    copy_constructible_archetype() : Base(dummy_cons) { }
    copy_constructible_archetype(const copy_constructible_archetype&)
      : Base(dummy_cons) { }
    copy_constructible_archetype(detail::dummy_constructor x) : Base(x) { }
  };

  template <class Base = null_archetype<> >
  class sgi_assignable_archetype : public Base {
  public:
    sgi_assignable_archetype(const sgi_assignable_archetype&)
      : Base(dummy_cons) { }
    sgi_assignable_archetype& operator=(const sgi_assignable_archetype&) {
      return *this;
    }
    sgi_assignable_archetype(const detail::dummy_constructor& x) : Base(x) { }
  };

  struct default_archetype_base {
    default_archetype_base(detail::dummy_constructor x) { }
  };

  // Careful, don't use same type for T and Base. That results in the
  // conversion operator being invalid.  Since T is often
  // null_archetype, can't use null_archetype for Base.
  template <class T, class Base = default_archetype_base>
  class convertible_to_archetype : public Base {
  private:
    convertible_to_archetype() { }
    convertible_to_archetype(const convertible_to_archetype& ) { }
    convertible_to_archetype& operator=(const convertible_to_archetype&)
      { return *this; }
  public:
    convertible_to_archetype(detail::dummy_constructor x) : Base(x) { }
    operator const T&() const { return static_object<T>::get(); }
  };

  template <class T, class Base = default_archetype_base>
  class convertible_from_archetype : public Base {
  private:
    convertible_from_archetype() { }
    convertible_from_archetype(const convertible_from_archetype& ) { }
    convertible_from_archetype& operator=(const convertible_from_archetype&)
      { return *this; }
  public:
    convertible_from_archetype(detail::dummy_constructor x) : Base(x) { }
    convertible_from_archetype(const T&) { }
    convertible_from_archetype& operator=(const T&)
      { return *this; }
  };

  class boolean_archetype {
  public:
    boolean_archetype(const boolean_archetype&) { }
    operator bool() const { return true; }
    boolean_archetype(detail::dummy_constructor x) { }
  private:
    boolean_archetype() { }
    boolean_archetype& operator=(const boolean_archetype&) { return *this; }
  };
  
  template <class Base = null_archetype<> >
  class equality_comparable_archetype : public Base {
  public:
    equality_comparable_archetype(detail::dummy_constructor x) : Base(x) { }
  };
  template <class Base>
  boolean_archetype
  operator==(const equality_comparable_archetype<Base>&,
	     const equality_comparable_archetype<Base>&) 
    { return boolean_archetype(dummy_cons); }
  template <class Base>
  boolean_archetype
  operator!=(const equality_comparable_archetype<Base>&,
	     const equality_comparable_archetype<Base>&)
    { return boolean_archetype(dummy_cons); }


  template <class Base = null_archetype<> >
  class equality_comparable2_first_archetype : public Base {
  public:
    equality_comparable2_first_archetype(detail::dummy_constructor x) 
      : Base(x) { }
  };
  template <class Base = null_archetype<> >
  class equality_comparable2_second_archetype : public Base {
  public:
    equality_comparable2_second_archetype(detail::dummy_constructor x) 
      : Base(x) { }
  };
  template <class Base1, class Base2>
  boolean_archetype
  operator==(const equality_comparable2_first_archetype<Base1>&,
	     const equality_comparable2_second_archetype<Base2>&) 
    { return boolean_archetype(dummy_cons); }
  template <class Base1, class Base2>
  boolean_archetype
  operator!=(const equality_comparable2_first_archetype<Base1>&,
	     const equality_comparable2_second_archetype<Base2>&)
    { return boolean_archetype(dummy_cons); }


  template <class Base = null_archetype<> >
  class less_than_comparable_archetype : public Base {
  public:
    less_than_comparable_archetype(detail::dummy_constructor x) : Base(x) { }
  };
  template <class Base>
  boolean_archetype
  operator<(const less_than_comparable_archetype<Base>&,
	    const less_than_comparable_archetype<Base>&)
    { return boolean_archetype(dummy_cons); }



  template <class Base = null_archetype<> >
  class comparable_archetype : public Base {
  public:
    comparable_archetype(detail::dummy_constructor x) : Base(x) { }
  };
  template <class Base>
  boolean_archetype
  operator<(const comparable_archetype<Base>&,
	    const comparable_archetype<Base>&)
    { return boolean_archetype(dummy_cons); }
  template <class Base>
  boolean_archetype
  operator<=(const comparable_archetype<Base>&,
	     const comparable_archetype<Base>&)
    { return boolean_archetype(dummy_cons); }
  template <class Base>
  boolean_archetype
  operator>(const comparable_archetype<Base>&,
	    const comparable_archetype<Base>&)
    { return boolean_archetype(dummy_cons); }
  template <class Base>
  boolean_archetype
  operator>=(const comparable_archetype<Base>&,
	     const comparable_archetype<Base>&)
    { return boolean_archetype(dummy_cons); }


  // The purpose of the optags is so that one can specify
  // exactly which types the operator< is defined between.
  // This is useful for allowing the operations:
  //
  // A a; B b;
  // a < b
  // b < a
  //
  // without also allowing the combinations:
  //
  // a < a
  // b < b
  //
  struct optag1 { };
  struct optag2 { };
  struct optag3 { };

#define BOOST_DEFINE_BINARY_PREDICATE_ARCHETYPE(OP, NAME) \
  template <class Base = null_archetype<>, class Tag = optag1 > \
  class NAME##_first_archetype : public Base { \
  public: \
    NAME##_first_archetype(detail::dummy_constructor x) : Base(x) { } \
  }; \
  \
  template <class Base = null_archetype<>, class Tag = optag1 > \
  class NAME##_second_archetype : public Base { \
  public: \
    NAME##_second_archetype(detail::dummy_constructor x) : Base(x) { } \
  }; \
  \
  template <class BaseFirst, class BaseSecond, class Tag> \
  boolean_archetype \
  operator OP (const NAME##_first_archetype<BaseFirst, Tag>&, \
               const NAME##_second_archetype<BaseSecond, Tag>&) \
  { \
    return boolean_archetype(dummy_cons); \
  }

  BOOST_DEFINE_BINARY_PREDICATE_ARCHETYPE(==, equal_op)
  BOOST_DEFINE_BINARY_PREDICATE_ARCHETYPE(!=, not_equal_op)
  BOOST_DEFINE_BINARY_PREDICATE_ARCHETYPE(<, less_than_op)
  BOOST_DEFINE_BINARY_PREDICATE_ARCHETYPE(<=, less_equal_op)
  BOOST_DEFINE_BINARY_PREDICATE_ARCHETYPE(>, greater_than_op)
  BOOST_DEFINE_BINARY_PREDICATE_ARCHETYPE(>=, greater_equal_op)

#define BOOST_DEFINE_OPERATOR_ARCHETYPE(OP, NAME) \
  template <class Base = null_archetype<> > \
  class NAME##_archetype : public Base { \
  public: \
    NAME##_archetype(detail::dummy_constructor x) : Base(x) { } \
    NAME##_archetype(const NAME##_archetype&)  \
      : Base(dummy_cons) { } \
    NAME##_archetype& operator=(const NAME##_archetype&) { return *this; } \
  }; \
  template <class Base> \
  NAME##_archetype<Base> \
  operator OP (const NAME##_archetype<Base>&,\
	       const NAME##_archetype<Base>&)  \
    { return NAME##_archetype<Base>(dummy_cons); }

  BOOST_DEFINE_OPERATOR_ARCHETYPE(+, addable)
  BOOST_DEFINE_OPERATOR_ARCHETYPE(-, subtractable)
  BOOST_DEFINE_OPERATOR_ARCHETYPE(*, multipliable)
  BOOST_DEFINE_OPERATOR_ARCHETYPE(/, dividable)
  BOOST_DEFINE_OPERATOR_ARCHETYPE(%, modable)

  // As is, these are useless because of the return type.
  // Need to invent a better way...
#define BOOST_DEFINE_BINARY_OPERATOR_ARCHETYPE(OP, NAME) \
  template <class Return, class Base = null_archetype<> > \
  class NAME##_first_archetype : public Base { \
  public: \
    NAME##_first_archetype(detail::dummy_constructor x) : Base(x) { } \
  }; \
  \
  template <class Return, class Base = null_archetype<> > \
  class NAME##_second_archetype : public Base { \
  public: \
    NAME##_second_archetype(detail::dummy_constructor x) : Base(x) { } \
  }; \
  \
  template <class Return, class BaseFirst, class BaseSecond> \
  Return \
  operator OP (const NAME##_first_archetype<Return, BaseFirst>&, \
               const NAME##_second_archetype<Return, BaseSecond>&) \
  { \
    return Return(dummy_cons); \
  }

  BOOST_DEFINE_BINARY_OPERATOR_ARCHETYPE(+, plus_op)
  BOOST_DEFINE_BINARY_OPERATOR_ARCHETYPE(*, time_op)
  BOOST_DEFINE_BINARY_OPERATOR_ARCHETYPE(/, divide_op)
  BOOST_DEFINE_BINARY_OPERATOR_ARCHETYPE(-, subtract_op)
  BOOST_DEFINE_BINARY_OPERATOR_ARCHETYPE(%, mod_op)

  //===========================================================================
  // Function Object Archetype Classes

  template <class Return>
  class generator_archetype {
  public:
    const Return& operator()() {
      return static_object<Return>::get(); 
    }
  };

  class void_generator_archetype {
  public:
    void operator()() { }
  };

  template <class Arg, class Return>
  class unary_function_archetype {
  private:
    unary_function_archetype() { }
  public:
    unary_function_archetype(detail::dummy_constructor) { }
    const Return& operator()(const Arg&) {
      return static_object<Return>::get(); 
    }
  };

  template <class Arg1, class Arg2, class Return>
  class binary_function_archetype {
  private:
    binary_function_archetype() { }
  public:
    binary_function_archetype(detail::dummy_constructor) { }
    const Return& operator()(const Arg1&, const Arg2&) {
      return static_object<Return>::get(); 
    }
  };

  template <class Arg>
  class unary_predicate_archetype {
    typedef boolean_archetype Return;
    unary_predicate_archetype() { }
  public:
    unary_predicate_archetype(detail::dummy_constructor) { }
    const Return& operator()(const Arg&) {
      return static_object<Return>::get(); 
    }
  };

  template <class Arg1, class Arg2, class Base = null_archetype<> >
  class binary_predicate_archetype {
    typedef boolean_archetype Return;
    binary_predicate_archetype() { }
  public:
    binary_predicate_archetype(detail::dummy_constructor) { }
    const Return& operator()(const Arg1&, const Arg2&) {
      return static_object<Return>::get(); 
    }
  };

  template <class Arg1, class Arg2>
  class const_binary_predicate_archetype {
    typedef boolean_archetype Return;
    const_binary_predicate_archetype() { }
  public:
    const_binary_predicate_archetype(detail::dummy_constructor) { }
    const Return& operator()(const Arg1&, const Arg2&) const {
      return static_object<Return>::get(); 
    }
  };


  //===========================================================================
  // Iterator Archetype Classes

  template <class T>
  struct input_proxy {
    operator const T&() { return static_object<T>::get(); }
  };
  template <class T>
  class trivial_iterator_archetype
  {
    typedef trivial_iterator_archetype self;
  public:
#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    typedef T value_type;
    typedef void reference;
    typedef void pointer;
    typedef void difference_type;
    typedef void iterator_category;
#endif
    trivial_iterator_archetype() { }
    self& operator=(const self&) { return *this;  }
    bool operator==(const self&) const { return true; }
    bool operator!=(const self&) const { return true; }
    input_proxy<T> operator*() const { return input_proxy<T>(); }
  };
} // namespace boost

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
namespace std {
  template <class T>
  struct iterator_traits< boost::trivial_iterator_archetype<T> >
  {
    typedef T value_type;
  };
}
#endif

namespace boost {
  template <class T>
  struct input_output_proxy {
    input_output_proxy<T>& operator=(const T&) { return *this; }
    operator const T&() { return static_object<T>::get(); }
  };
  template <class T>
  class mutable_trivial_iterator_archetype
  {
    typedef mutable_trivial_iterator_archetype self;
  public:
#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    typedef T value_type;
    typedef void reference;
    typedef void pointer;
    typedef void difference_type;
    typedef void iterator_category;
#endif
    mutable_trivial_iterator_archetype() { }
    self& operator=(const self&) { return *this;  }
    bool operator==(const self&) const { return true; }
    bool operator!=(const self&) const { return true; }
    input_output_proxy<T> operator*() const { return input_output_proxy<T>(); }
  };
} // namespace boost

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
namespace std {
  template <class T>
  struct iterator_traits< boost::mutable_trivial_iterator_archetype<T> >
  {
    typedef T value_type;
  };
}
#endif

namespace boost {

  template <class T>
  class input_iterator_archetype
  {
  public:
    typedef input_iterator_archetype self;
  public:
    typedef std::input_iterator_tag iterator_category;
    typedef T value_type;
    typedef const T& reference;
    typedef const T* pointer;
    typedef std::ptrdiff_t difference_type;
    input_iterator_archetype() { }
    self& operator=(const self&) { return *this;  }
    bool operator==(const self&) const { return true; }
    bool operator!=(const self&) const { return true; }
    reference operator*() const { return static_object<T>::get(); }
    self& operator++() { return *this; }
    self operator++(int) { return *this; }
  };

 template <class T>
  struct output_proxy {
    output_proxy& operator=(const T&) { return *this; }
  };

  template <class T>
  class output_iterator_archetype
  {
  public:
    typedef output_iterator_archetype self;
  public:
    typedef std::output_iterator_tag iterator_category;
    typedef output_proxy<T> value_type;
    typedef output_proxy<T> reference;
    typedef void pointer;
    typedef void difference_type;
    output_iterator_archetype() { }
    self& operator=(const self&) { return *this;  }
    bool operator==(const self&) const { return true; }
    bool operator!=(const self&) const { return true; }
    reference operator*() const { return output_proxy<T>(); }
    self& operator++() { return *this; }
    self operator++(int) { return *this; }
  };

  template <class T>
  class forward_iterator_archetype
  {
  public:
    typedef forward_iterator_archetype self;
  public:
    typedef std::forward_iterator_tag iterator_category;
    typedef T value_type;
    typedef T& reference;
    typedef T* pointer;
    typedef std::ptrdiff_t difference_type;
    forward_iterator_archetype() { }
    self& operator=(const self&) { return *this;  }
    bool operator==(const self&) const { return true; }
    bool operator!=(const self&) const { return true; }
    reference operator*() const { return static_object<T>::get(); }
    self& operator++() { return *this; }
    self operator++(int) { return *this; }
  };

  template <class T>
  class bidirectional_iterator_archetype
  {
  public:
    typedef bidirectional_iterator_archetype self;
  public:
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef T& reference;
    typedef T* pointer;
    typedef std::ptrdiff_t difference_type;
    bidirectional_iterator_archetype() { }
    self& operator=(const self&) { return *this;  }
    bool operator==(const self&) const { return true; }
    bool operator!=(const self&) const { return true; }
    reference operator*() const { return static_object<T>::get(); }
    self& operator++() { return *this; }
    self operator++(int) { return *this; }
    self& operator--() { return *this; }
    self operator--(int) { return *this; }
  };

  template <class T>
  class random_access_iterator_archetype
  {
  public:
    typedef random_access_iterator_archetype self;
  public:
    typedef std::random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef T& reference;
    typedef T* pointer;
    typedef std::ptrdiff_t difference_type;
    random_access_iterator_archetype() { }
    self& operator=(const self&) { return *this;  }
    bool operator==(const self&) const { return true; }
    bool operator!=(const self&) const { return true; }
    reference operator*() const { return static_object<T>::get(); }
    self& operator++() { return *this; }
    self operator++(int) { return *this; }
    self& operator--() { return *this; }
    self operator--(int) { return *this; }
    reference operator[](difference_type n) 
      { return static_object<T>::get(); }
    self& operator+=(difference_type) { return *this; }
    self& operator-=(difference_type) { return *this; }
    difference_type operator-(const self&) const
      { return difference_type(); }
    self operator+(difference_type) { return *this; }
    self operator-(difference_type) { return *this; }
    bool operator<(const self&) { return true; }
    bool operator<=(const self&) { return true; }
    bool operator>(const self&) { return true; }
    bool operator>=(const self&) { return true; }
  };
  template <class T>
  random_access_iterator_archetype<T> 
  operator+(typename random_access_iterator_archetype<T>::difference_type, 
            const random_access_iterator_archetype<T>& x) 
    { return x; }

} // namespace boost

#endif // BOOST_CONCEPT_ARCHETYPES_H