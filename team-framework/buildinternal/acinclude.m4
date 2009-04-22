dnl @synopsis AC_CXX_BOOL
dnl
dnl If the compiler recognizes bool as a separate built-in type,
dnl define HAVE_BOOL. Note that a typedef is not a separate
dnl type since you cannot overload a function such that it accepts either
dnl the basic type or the typedef.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_BOOL],
[AC_CACHE_CHECK(whether the compiler recognizes bool as a built-in type,
ac_cv_cxx_bool,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([
int f(int  x){return 1;}
int f(char x){return 1;}
int f(bool x){return 1;}
],[bool b = true; return f(b);],
 ac_cv_cxx_bool=yes, ac_cv_cxx_bool=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_bool" = yes; then
  AC_DEFINE(HAVE_BOOL,,[define if bool is a built-in type])
fi
])
dnl @synopsis AC_CXX_COMPLEX_MATH_IN_NAMESPACE_STD
dnl
dnl If the C math functions are in the cmath header file and std:: namespace,
dnl define HAVE_MATH_FN_IN_NAMESPACE_STD.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_COMPLEX_MATH_IN_NAMESPACE_STD],
[AC_CACHE_CHECK(whether complex math functions are in std::,
ac_cv_cxx_complex_math_in_namespace_std,
[AC_REQUIRE([AC_CXX_NAMESPACES])
 AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <complex>
namespace S { using namespace std;
              complex<float> pow(complex<float> x, complex<float> y)
              { return std::pow(x,y); }
            };
],[using namespace S; complex<float> x = 1.0, y = 1.0; S::pow(x,y); return 0;],
 ac_cv_cxx_complex_math_in_namespace_std=yes, ac_cv_cxx_complex_math_in_namespace_std=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_complex_math_in_namespace_std" = yes; then
  AC_DEFINE(HAVE_COMPLEX_MATH_IN_NAMESPACE_STD,,
            [define if complex math functions are in std::])
fi
])
dnl @synopsis AC_CXX_CONST_CAST
dnl
dnl If the compiler supports const_cast<>, define HAVE_CONST_CAST.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_CONST_CAST],
[AC_CACHE_CHECK(whether the compiler supports const_cast<>,
ac_cv_cxx_const_cast,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE(,[int x = 0;const int& y = x;int& z = const_cast<int&>(y);return z;],
 ac_cv_cxx_const_cast=yes, ac_cv_cxx_const_cast=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_const_cast" = yes; then
  AC_DEFINE(HAVE_CONST_CAST,,[define if the compiler supports const_cast<>])
fi
])
dnl @synopsis AC_CXX_DEFAULT_TEMPLATE_PARAMETERS
dnl
dnl If the compiler supports default template parameters,
dnl define HAVE_DEFAULT_TEMPLATE_PARAMETERS.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_DEFAULT_TEMPLATE_PARAMETERS],
[AC_CACHE_CHECK(whether the compiler supports default template parameters,
ac_cv_cxx_default_template_parameters,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([
template<class T = double, int N = 10> class A {public: int f() {return 0;}};
],[A<float> a; return a.f();],
 ac_cv_cxx_default_template_parameters=yes, ac_cv_cxx_default_template_parameters=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_default_template_parameters" = yes; then
  AC_DEFINE(HAVE_DEFAULT_TEMPLATE_PARAMETERS,,
            [define if the compiler supports default template parameters])
fi
])
dnl @synopsis AC_CXX_DTOR_AFTER_ATEXIT
dnl
dnl If the C++ compiler calls global destructors after atexit functions,
dnl define HAVE_DTOR_AFTER_ATEXIT.
dnl WARNING: If cross-compiling, the test cannot be performed, the
dnl default action is to define HAVE_DTOR_AFTER_ATEXIT.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_DTOR_AFTER_ATEXIT],
[AC_CACHE_CHECK(whether the compiler calls global destructors after functions registered through atexit,
ac_cv_cxx_dtor_after_atexit,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_RUN([
#include <unistd.h>
#include <stdlib.h>

static int dtor_called = 0;
class A { public : ~A () { dtor_called = 1; } };
static A a;

void f() { _exit(dtor_called); }

int main (int , char **)
{
  atexit (f);
  return 0;
}
],
 ac_cv_cxx_dtor_after_atexit=yes, ac_cv_cxx_dtor_after_atexit=yes=no,
 ac_cv_cxx_dtor_after_atexit=yes)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_dtor_after_atexit" = yes; then
  AC_DEFINE(HAVE_DTOR_AFTER_ATEXIT,,
            [define if the compiler calls global destructors after functions registered through atexit])
fi
])

dnl @synopsis AC_CXX_DYNAMIC_CAST
dnl
dnl If the compiler supports dynamic_cast<>, define HAVE_DYNAMIC_CAST.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_DYNAMIC_CAST],
[AC_CACHE_CHECK(whether the compiler supports dynamic_cast<>,
ac_cv_cxx_dynamic_cast,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <typeinfo>
class Base { public : Base () {} virtual void f () = 0;};
class Derived : public Base { public : Derived () {} virtual void f () {} };],[
Derived d; Base& b=d; return dynamic_cast<Derived*>(&b) ? 0 : 1;],
 ac_cv_cxx_dynamic_cast=yes, ac_cv_cxx_dynamic_cast=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_dynamic_cast" = yes; then
  AC_DEFINE(HAVE_DYNAMIC_CAST,,[define if the compiler supports dynamic_cast<>])
fi
])
dnl @synopsis AC_CXX_ENUM_COMPUTATIONS
dnl
dnl If the compiler handle computations inside an enum, define
dnl HAVE_ENUM_COMPUTATIONS.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_ENUM_COMPUTATIONS],
[AC_CACHE_CHECK(whether the compiler handle computations inside an enum,
ac_cv_cxx_enum_computations,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([
struct A { enum { a = 5, b = 7, c = 2 }; };
struct B { enum { a = 1, b = 6, c = 9 }; };
template<class T1, class T2> struct Z
{ enum { a = (T1::a > T2::a) ? T1::a : T2::b,
         b = T1::b + T2::b,
         c = (T1::c * T2::c + T2::a + T1::a)
       };
};],[
return (((int)Z<A,B>::a == 5)
     && ((int)Z<A,B>::b == 13)
     && ((int)Z<A,B>::c == 24)) ? 0 : 1;],
 ac_cv_cxx_enum_computations=yes, ac_cv_cxx_enum_computations=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_enum_computations" = yes; then
  AC_DEFINE(HAVE_ENUM_COMPUTATIONS,,
            [define if the compiler handle computations inside an enum])
fi
])
dnl @synopsis AC_CXX_ENUM_COMPUTATIONS_WITH_CAST
dnl
dnl If the compiler handle (int) casts in enum computations, define
dnl HAVE_ENUM_COMPUTATIONS_WITH_CAST.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_ENUM_COMPUTATIONS_WITH_CAST],
[AC_CACHE_CHECK(whether the compiler handles (int) casts in enum computations,
ac_cv_cxx_enum_computations_with_cast,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([
struct A { enum { a = 5, b = 7, c = 2 }; };
struct B { enum { a = 1, b = 6, c = 9 }; };
template<class T1, class T2> struct Z
{ enum { a = ((int)T1::a > (int)T2::a) ? (int)T1::a : (int)T2::b,
         b = (int)T1::b + (int)T2::b,
         c = ((int)T1::c * (int)T2::c + (int)T2::a + (int)T1::a)
       };
};],[
return (((int)Z<A,B>::a == 5)
     && ((int)Z<A,B>::b == 13)
     && ((int)Z<A,B>::c == 24)) ? 0 : 1;],
 ac_cv_cxx_enum_computations_with_cast=yes, ac_cv_cxx_enum_computations_with_cast=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_enum_computations_with_cast" = yes; then
  AC_DEFINE(HAVE_ENUM_COMPUTATIONS_WITH_CAST,,
            [define if the compiler handles (int) casts in enum computations])
fi
])
dnl @synopsis AC_CXX_EXCEPTIONS
dnl
dnl If the C++ compiler supports exceptions handling (try,
dnl throw and catch), define HAVE_EXCEPTIONS.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_EXCEPTIONS],
[AC_CACHE_CHECK(whether the compiler supports exceptions,
ac_cv_cxx_exceptions,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE(,[try { throw  1; } catch (int i) { return i; }],
 ac_cv_cxx_exceptions=yes, ac_cv_cxx_exceptions=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_exceptions" = yes; then
  AC_DEFINE(HAVE_EXCEPTIONS,,[define if the compiler supports exceptions])
fi
])
dnl @synopsis AC_CXX_EXPLICIT_INSTANTIATIONS
dnl
dnl If the C++ compiler supports explicit instanciations syntax,
dnl define HAVE_INSTANTIATIONS.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_EXPLICIT_INSTANTIATIONS],
[AC_CACHE_CHECK(whether the compiler supports explicit instantiations,
ac_cv_cxx_explinst,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([template <class T> class A { T t; }; template class A<int>;],
 [], ac_cv_cxx_explinst=yes, ac_cv_cxx_explinst=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_explinst" = yes; then
  AC_DEFINE(HAVE_INSTANTIATIONS,,
            [define if the compiler supports explicit instantiations])
fi
])
dnl @synopsis AC_CXX_EXPLICIT
dnl
dnl If the compiler can be asked to prevent using implicitly one argument
dnl constructors as converting constructors with the explicit
dnl keyword, define HAVE_EXPLICIT.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_EXPLICIT],
[AC_CACHE_CHECK(whether the compiler supports the explicit keyword,
ac_cv_cxx_explicit,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([class A{public:explicit A(double){}};],
[double c = 5.0;A x(c);return 0;],
 ac_cv_cxx_explicit=yes, ac_cv_cxx_explicit=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_explicit" = yes; then
  AC_DEFINE(HAVE_EXPLICIT,,[define if the compiler supports the explicit keyword])
fi
])
dnl @synopsis AC_CXX_EXPLICIT_TEMPLATE_FUNCTION_QUALIFICATION
dnl
dnl If the compiler supports explicit template function qualification,
dnl define HAVE_EXPLICIT_TEMPLATE_FUNCTION_QUALIFICATION.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_EXPLICIT_TEMPLATE_FUNCTION_QUALIFICATION],
[AC_CACHE_CHECK(whether the compiler supports explicit template function qualification,
ac_cv_cxx_explicit_template_function_qualification,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([
template<class Z> class A { public : A() {} };
template<class X, class Y> A<X> to (const A<Y>&) { return A<X>(); }
],[A<float> x; A<double> y = to<double>(x); return 0;],
 ac_cv_cxx_explicit_template_function_qualification=yes, ac_cv_cxx_explicit_template_function_qualification=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_explicit_template_function_qualification" = yes; then
  AC_DEFINE(HAVE_EXPLICIT_TEMPLATE_FUNCTION_QUALIFICATION,,
            [define if the compiler supports explicit template function qualification])
fi
])
dnl @synopsis AC_CXX_FULL_SPECIALIZATION_SYNTAX
dnl
dnl If the compiler recognizes the full specialization syntax, define
dnl HAVE_FULL_SPECIALIZATION_SYNTAX.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_FULL_SPECIALIZATION_SYNTAX],
[AC_CACHE_CHECK(whether the compiler recognizes the full specialization syntax,
ac_cv_cxx_full_specialization_syntax,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([
template<class T> class A        { public : int f () const { return 1; } };
template<>        class A<float> { public:  int f () const { return 0; } };],[
A<float> a; return a.f();],
 ac_cv_cxx_full_specialization_syntax=yes, ac_cv_cxx_full_specialization_syntax=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_full_specialization_syntax" = yes; then
  AC_DEFINE(HAVE_FULL_SPECIALIZATION_SYNTAX,,
            [define if the compiler recognizes the full specialization syntax])
fi
])
dnl @synopsis AC_CXX_FUNCTION_NONTYPE_PARAMETERS
dnl
dnl If the compiler supports function templates with non-type parameters,
dnl define HAVE_FUNCTION_NONTYPE_PARAMETERS.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_FUNCTION_NONTYPE_PARAMETERS],
[AC_CACHE_CHECK(whether the compiler supports function templates with non-type parameters,
ac_cv_cxx_function_nontype_parameters,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([
template<class T, int N> class A {};
template<class T, int N> int f(const A<T,N>& x) { return 0; }
],[A<double, 17> z; return f(z);],
 ac_cv_cxx_function_nontype_parameters=yes, ac_cv_cxx_function_nontype_parameters=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_function_nontype_parameters" = yes; then
  AC_DEFINE(HAVE_FUNCTION_NONTYPE_PARAMETERS,,
            [define if the compiler supports function templates with non-type parameters])
fi
])
dnl @synopsis AC_CXX_HAVE_COMPLEX
dnl
dnl If the compiler has complex<T>, define HAVE_COMPLEX.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_HAVE_COMPLEX],
[AC_CACHE_CHECK(whether the compiler has complex<T>,
ac_cv_cxx_have_complex,
[AC_REQUIRE([AC_CXX_NAMESPACES])
 AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <complex>
#ifdef HAVE_NAMESPACES
using namespace std;
#endif],[complex<float> a; complex<double> b; return 0;],
 ac_cv_cxx_have_complex=yes, ac_cv_cxx_have_complex=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_complex" = yes; then
  AC_DEFINE(HAVE_COMPLEX,,[define if the compiler has complex<T>])
fi
])
dnl @synopsis AC_CXX_HAVE_COMPLEX_MATH1
dnl
dnl If the compiler has the complex math functions cos, cosh, exp, log,
dnl pow, sin, sinh, sqrt, tan and tanh, define HAVE_COMPLEX_MATH1.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_HAVE_COMPLEX_MATH1],
[AC_CACHE_CHECK(whether the compiler has complex math functions,
ac_cv_cxx_have_complex_math1,
[AC_REQUIRE([AC_CXX_NAMESPACES])
 AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 ac_save_LIBS="$LIBS"
 LIBS="$LIBS -lm"
 AC_TRY_LINK([#include <complex>
#ifdef HAVE_NAMESPACES
using namespace std;
#endif],[complex<double> x(1.0, 1.0), y(1.0, 1.0);
cos(x); cosh(x); exp(x); log(x); pow(x,1); pow(x,double(2.0));
pow(x, y); pow(double(2.0), x); sin(x); sinh(x); sqrt(x); tan(x); tanh(x);
return 0;],
 ac_cv_cxx_have_complex_math1=yes, ac_cv_cxx_have_complex_math1=no)
 LIBS="$ac_save_LIBS"
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_complex_math1" = yes; then
  AC_DEFINE(HAVE_COMPLEX_MATH1,,[define if the compiler has complex math functions])
fi
])
dnl @synopsis AC_CXX_HAVE_COMPLEX_MATH2
dnl
dnl If the compiler has the complex math functions acos, asin,
dnl atan, atan2 and log10, define HAVE_COMPLEX_MATH2.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_HAVE_COMPLEX_MATH2],
[AC_CACHE_CHECK(whether the compiler has more complex math functions,
ac_cv_cxx_have_complex_math2,
[AC_REQUIRE([AC_CXX_NAMESPACES])
 AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 ac_save_LIBS="$LIBS"
 LIBS="$LIBS -lm"
 AC_TRY_LINK([#include <complex>
#ifdef HAVE_NAMESPACES
using namespace std;
#endif],[complex<double> x(1.0, 1.0), y(1.0, 1.0);
acos(x); asin(x); atan(x); atan2(x,y); atan2(x, double(3.0));
atan2(double(3.0), x); log10(x); return 0;],
 ac_cv_cxx_have_complex_math2=yes, ac_cv_cxx_have_complex_math2=no)
 LIBS="$ac_save_LIBS"
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_complex_math2" = yes; then
  AC_DEFINE(HAVE_COMPLEX_MATH2,,[define if the compiler has more complex math functions])
fi
])
dnl
dnl @synopsis AC_CXX_HAVE_EMPTY_IOSTREAM
dnl
dnl Check if the compiler allow the empty iostream constructor. Ok
dnl before gcc3, not after.
dnl
dnl @author Alain BARBET <alian@cpan.org>
dnl @version $1.0 09/04/2002$
dnl
AC_DEFUN([AC_CXX_HAVE_EMPTY_IOSTREAM],
[AC_CACHE_CHECK(whether the compiler allow empty iostream,
ac_cv_cxx_have_empty_iostream,
[AC_REQUIRE([AC_CXX_NAMESPACES])
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_TRY_COMPILE([#include <iostream>
#ifdef HAVE_NAMESPACES
using namespace std;
#endif],[iostream iostr; return 0;],
  ac_cv_cxx_have_empty_iostream=yes, ac_cv_cxx_have_empty_iostream=no)
  AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_empty_iostream" = yes; then
   AC_DEFINE(HAVE_EMPTY_IOSTREAM,,[define if the compiler allow empty
iostream])
fi
])
dnl @synopsis AC_CXX_HAVE_EXT_HASH_MAP
dnl
dnl Check if the compiler has ext/hash_map
dnl Eg:
dnl
dnl   #if defined(HAVE_EXT_HASH_MAP)
dnl   #include <ext/hash_map>
dnl   #else
dnl   #if defined(HAVE_STL)
dnl   #include <hash_map>
dnl   #else
dnl   # Can't find hash_map header !
dnl   #endif
dnl   #endif
dnl
dnl This file is Alain BARBET's AC_CXX_HAVE_EXT_HASH_SET 1.1
dnl with s/set/map/g :)
dnl
dnl @author Perceval ANICHINI <perceval.anichini@epita.fr>
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl
AC_DEFUN([AC_CXX_HAVE_EXT_HASH_MAP],
[AC_CACHE_CHECK(whether the compiler has ext/hash_map,
ac_cv_cxx_have_ext_hash_map,
[AC_REQUIRE([AC_CXX_NAMESPACES])
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_TRY_COMPILE([#include <ext/hash_map>
#ifdef HAVE_NAMESPACES
using namespace std;
#endif],[hash_map<int, int> t; return 0;],
  ac_cv_cxx_have_ext_hash_map=yes, ac_cv_cxx_have_ext_hash_map=no)
  AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_ext_hash_map" = yes; then
   AC_DEFINE(HAVE_EXT_HASH_MAP,,[define if the compiler has ext/hash_map])
fi
])
dnl
dnl @synopsis AC_CXX_HAVE_EXT_HASH_SET
dnl
dnl Check if the compiler has ext/hash_set
dnl Eg:
dnl
dnl   #if defined(HAVE_EXT_HASH_SET)
dnl   #include <ext/hash_set>
dnl   #else
dnl   #if defined(HAVE_STL)
dnl   #include <hash_set>
dnl   #else
dnl   # Can't find hash_set header !
dnl   #endif
dnl   #endif
dnl
dnl @author Alain BARBET <alian@cpan.org>
dnl @version $1.1 09/04/2002$
dnl
AC_DEFUN([AC_CXX_HAVE_EXT_HASH_SET],
[AC_CACHE_CHECK(whether the compiler has ext/hash_set,
ac_cv_cxx_have_ext_hash_set,
[AC_REQUIRE([AC_CXX_NAMESPACES])
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_TRY_COMPILE([#include <ext/hash_set>
#ifdef HAVE_NAMESPACES
using namespace std;
#endif],[hash_set<int> t; return 0;],
  ac_cv_cxx_have_ext_hash_set=yes, ac_cv_cxx_have_ext_hash_set=no)
  AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_ext_hash_set" = yes; then
   AC_DEFINE(HAVE_EXT_HASH_SET,,[define if the compiler has ext/hash_set])
fi
])
dnl
dnl @synopsis AC_CXX_HAVE_EXT_SLIST
dnl
dnl Check if the compiler has ext/slist.
dnl Eg:
dnl
dnl   #if defined(HAVE_EXT_SLIST)
dnl   #include <ext/slist>
dnl   #else
dnl   #if defined(HAVE_STL)
dnl   #include <slist>
dnl   #else
dnl   # Can't find slist header !
dnl   #endif
dnl   #endif
dnl
dnl @author Alain BARBET <alian@cpan.org>
dnl @version $1.1 09/04/2002$
dnl
AC_DEFUN([AC_CXX_HAVE_EXT_SLIST],
[AC_CACHE_CHECK(whether the compiler has ext/slist,
ac_cv_cxx_have_ext_slist,
[AC_REQUIRE([AC_CXX_NAMESPACES])
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_TRY_COMPILE([#include <ext/slist>
#ifdef HAVE_NAMESPACES
using namespace std;
#endif],[slist<int> s; return 0;],
  ac_cv_cxx_have_ext_slist=yes, ac_cv_cxx_have_ext_slist=no)
  AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_ext_slist" = yes; then
   AC_DEFINE(HAVE_EXT_SLIST,,[define if the compiler has ext/slist])
fi
])
dnl
dnl @synopsis AC_CXX_HAVE_FREEZE_SSTREAM
dnl
dnl Check if the compiler has (need) freeze method call in stringstream/
dnl strstream. Seems that Win32 and STLPort have it, libstdc++ not ...
dnl
dnl Eg:
dnl
dnl   #include <sstream>
dnl   #ifdef HAVE_NAMESPACES
dnl   using namespace std;
dnl   #endif
dnl   #ifdef HAVE_SSTREAM
dnl   stringstream message;
dnl   #else
dnl   strstream message;
dnl   #endif
dnl   message << "Hello";
dnl   #ifdef HAVE_FREEZE_SSTREAM
dnl   message.freeze(0);
dnl   #endif
dnl
dnl
dnl @author Alain BARBET <alian@cpan.org>
dnl @version $2.0 09/04/2002$
dnl
AC_DEFUN([AC_CXX_HAVE_FREEZE_SSTREAM],
[AC_CACHE_CHECK(whether the compiler has freeze in stringstream,
ac_cv_cxx_have_freeze_sstream,
[AC_REQUIRE([AC_CXX_NAMESPACES])
  AC_REQUIRE([AC_CXX_HAVE_SSTREAM])
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_TRY_COMPILE([#include <sstream>
#ifdef HAVE_NAMESPACES
using namespace std;
#endif],
[#ifdef HAVE_SSTREAM
stringstream message;
#else
strstream message;
#endif
message << "Hello"; message.freeze(0); return 0;],
  ac_cv_cxx_have_freeze_sstream=yes, ac_cv_cxx_have_freeze_sstream=no)
  AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_freeze_sstream" = yes; then
   AC_DEFINE(HAVE_FREEZE_SSTREAM,,[define if the compiler has freeze in
stringstream])
fi
])
dnl @synopsis AC_CXX_HAVE_IEEE_MATH
dnl
dnl If the compiler has the double math functions acosh,
dnl asinh, atanh, expm1, erf, erfc, isnan, j0, j1, lgamma, logb,
dnl log1p, rint, y0 and y1, define HAVE_IEEE_MATH.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_HAVE_IEEE_MATH],
[AC_CACHE_CHECK(whether the compiler supports IEEE math library,
ac_cv_cxx_have_ieee_math,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 ac_save_LIBS="$LIBS"
 LIBS="$LIBS -lm"
 AC_TRY_LINK([
#ifndef _ALL_SOURCE
 #define _ALL_SOURCE
#endif
#ifndef _XOPEN_SOURCE
 #define _XOPEN_SOURCE
#endif
#ifndef _XOPEN_SOURCE_EXTENDED
 #define _XOPEN_SOURCE_EXTENDED 1
#endif
#include <math.h>],[double x = 1.0; double y = 1.0;
acosh(x); asinh(x); atanh(x); expm1(x); erf(x); erfc(x); isnan(x);
j0(x); j1(x); lgamma(x); logb(x); log1p(x); rint(x); y0(x); y1(x);
return 0;],
 ac_cv_cxx_have_ieee_math=yes, ac_cv_cxx_have_ieee_math=no)
 LIBS="$ac_save_LIBS"
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_ieee_math" = yes; then
  AC_DEFINE(HAVE_IEEE_MATH,,[define if the compiler supports IEEE math library])
fi
])
dnl
dnl @synopsis AC_CXX_HAVE_LONG_LONG_FOR_IOSTREAM
dnl
dnl Check if the compiler allow long long for [i|o]stream
dnl Seems that OpenBSD / gcc-3 don't have it.
dnl
dnl Eg:
dnl
dnl   #include <iostream>
dnl   #ifdef HAVE_SSTREAM
dnl   #include <strstream>
dnl   #else
dnl   #include <sstream>
dnl   #endif
dnl   #ifdef HAVE_NAMESPACES
dnl   using namespace std;
dnl   #endif
dnl   ostream str((streambuf *)0);
dnl   long long lo=1;
dnl   str <<
dnl   #ifdef HAVE_LONG_LONG_FOR_IOSTREAM
dnl     (long int)
dnl   #endif
dnl           lo;
dnl
dnl @author Alain BARBET <alian@cpan.org>
dnl @version $2.0 09/04/2002$
dnl
AC_DEFUN([AC_CXX_HAVE_LONG_LONG_FOR_IOSTREAM],
[AC_CACHE_CHECK(whether the compiler allow long long for [i|o]stream,
ac_cv_cxx_have_ll_for_iostream,
[AC_REQUIRE([AC_CXX_NAMESPACES])
  AC_REQUIRE([AC_CXX_HAVE_SSTREAM])
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_TRY_COMPILE([#include <iostream>
#ifdef HAVE_SSTREAM
#include <strstream>
#else
#include <sstream>
#endif
#ifdef HAVE_NAMESPACES
using namespace std;
#endif],[ ostream str((streambuf *)0); long long l=1; str << l; return 0;],
  ac_cv_cxx_have_ll_for_iostream=yes, ac_cv_cxx_have_ll_for_iostream=no)
  AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_ll_for_iostream" = yes; then
   AC_DEFINE(HAVE_LONG_LONG_FOR_IOSTREAM,,[define if the compiler allow long long for [i|o]stream])
fi
])
dnl @synopsis AC_CXX_HAVE_NUMERIC_LIMITS
dnl
dnl If the compiler has numeric_limits<T>, define HAVE_NUMERIC_LIMITS.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_HAVE_NUMERIC_LIMITS],
[AC_CACHE_CHECK(whether the compiler has numeric_limits<T>,
ac_cv_cxx_have_numeric_limits,
[AC_REQUIRE([AC_CXX_NAMESPACES])
 AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <limits>
#ifdef HAVE_NAMESPACES
using namespace std;
#endif],[double e = numeric_limits<double>::epsilon(); return 0;],
 ac_cv_cxx_have_numeric_limits=yes, ac_cv_cxx_have_numeric_limits=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_numeric_limits" = yes; then
  AC_DEFINE(HAVE_NUMERIC_LIMITS,,[define if the compiler has numeric_limits<T>])
fi
])
dnl @synopsis AC_CXX_HAVE_SSTREAM
dnl
dnl If the C++ library has a working stringstream, define HAVE_SSTREAM.
dnl
dnl @author Ben Stanley
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl
AC_DEFUN([AC_CXX_HAVE_SSTREAM],
[AC_CACHE_CHECK(whether the compiler has stringstream,
ac_cv_cxx_have_sstream,
[AC_REQUIRE([AC_CXX_NAMESPACES])
 AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <sstream>
#ifdef HAVE_NAMESPACES
using namespace std;
#endif],[stringstream message; message << "Hello"; return 0;],
 ac_cv_cxx_have_sstream=yes, ac_cv_cxx_have_sstream=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_sstream" = yes; then
  AC_DEFINE(HAVE_SSTREAM,,[define if the compiler has stringstream])
fi
])
dnl @synopsis AC_CXX_HAVE_STD
dnl
dnl If the compiler supports ISO C++ standard library (i.e., can include the
dnl files iostream, map, iomanip and cmath}), define HAVE_STD.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_HAVE_STD],
[AC_CACHE_CHECK(whether the compiler supports ISO C++ standard library,
ac_cv_cxx_have_std,
[AC_REQUIRE([AC_CXX_NAMESPACES])
 AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <iostream>
#include <map>
#include <iomanip>
#include <cmath>
#ifdef HAVE_NAMESPACES
using namespace std;
#endif],[return 0;],
 ac_cv_cxx_have_std=yes, ac_cv_cxx_have_std=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_std" = yes; then
  AC_DEFINE(HAVE_STD,,[define if the compiler supports ISO C++ standard library])
fi
])
dnl @synopsis AC_CXX_HAVE_STL
dnl
dnl If the compiler supports the Standard Template Library, define HAVE_STL.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_HAVE_STL],
[AC_CACHE_CHECK(whether the compiler supports Standard Template Library,
ac_cv_cxx_have_stl,
[AC_REQUIRE([AC_CXX_NAMESPACES])
 AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <list>
#include <deque>
#ifdef HAVE_NAMESPACES
using namespace std;
#endif],[list<int> x; x.push_back(5);
list<int>::iterator iter = x.begin(); if (iter != x.end()) ++iter; return 0;],
 ac_cv_cxx_have_stl=yes, ac_cv_cxx_have_stl=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_stl" = yes; then
  AC_DEFINE(HAVE_STL,,[define if the compiler supports Standard Template Library])
fi
])
dnl @synopsis AC_CXX_HAVE_STRING_PUSH_BACK
dnl
dnl If the implementation of the C++ library provides the method
dnl std::string::push_back (char), define HAVE_STRING_PUSH_BACK.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Jan Langer <jan@langernetz.de>
dnl
AC_DEFUN([AC_CXX_HAVE_STRING_PUSH_BACK],
[AC_CACHE_CHECK(whether the compiler has std::string::push_back (char),
ac_cv_cxx_have_string_push_back,
[AC_REQUIRE([AC_CXX_NAMESPACES])
 AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <string>
#ifdef HAVE_NAMESPACES
using namespace std;
#endif],[string message; message.push_back ('a'); return 0;],
 ac_cv_cxx_have_string_push_back=yes, ac_cv_cxx_have_string_push_back=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_string_push_back" = yes; then
 AC_DEFINE(HAVE_STRING_PUSH_BACK,,[define if the compiler has the method
std::string::push_back (char)])
fi
])dnl
dnl @synopsis AC_CXX_HAVE_SYSTEM_V_MATH
dnl
dnl If the compiler has the double math functions _class,
dnl ilogb, itrunc, nearest, rsqrt, uitrunc, copysign, drem, fmod, hypot,
dnl nextafter, remainder, scalb and unordered, define HAVE_SYSTEM_V_MATH.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_HAVE_SYSTEM_V_MATH],
[AC_CACHE_CHECK(whether the compiler supports System V math library,
ac_cv_cxx_have_system_v_math,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 ac_save_LIBS="$LIBS"
 LIBS="$LIBS -lm"
 AC_TRY_LINK([
#ifndef _ALL_SOURCE
 #define _ALL_SOURCE
#endif
#ifndef _XOPEN_SOURCE
 #define _XOPEN_SOURCE
#endif
#ifndef _XOPEN_SOURCE_EXTENDED
 #define _XOPEN_SOURCE_EXTENDED 1
#endif
#include <math.h>],[double x = 1.0; double y = 1.0;
_class(x); ilogb(x); itrunc(x); nearest(x); rsqrt(x); uitrunc(x);
copysign(x,y); drem(x,y); fmod(x,y); hypot(x,y); nextafter(x,y);
remainder(x,y); scalb(x,y); unordered(x,y);
return 0;],
 ac_cv_cxx_have_system_v_math=yes, ac_cv_cxx_have_system_v_math=no)
 LIBS="$ac_save_LIBS"
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_system_v_math" = yes; then
  AC_DEFINE(HAVE_SYSTEM_V_MATH,,[define if the compiler supports System V math library])
fi
])
dnl @synopsis AC_CXX_HAVE_VALARRAY
dnl
dnl If the compiler has valarray<T>, define HAVE_VALARRAY.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_HAVE_VALARRAY],
[AC_CACHE_CHECK(whether the compiler has valarray<T>,
ac_cv_cxx_have_valarray,
[AC_REQUIRE([AC_CXX_NAMESPACES])
 AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <valarray>
#ifdef HAVE_NAMESPACES
using namespace std;
#endif],[valarray<float> x(100); return 0;],
 ac_cv_cxx_have_valarray=yes, ac_cv_cxx_have_valarray=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_valarray" = yes; then
  AC_DEFINE(HAVE_VALARRAY,,[define if the compiler has valarray<T>])
fi
])
dnl @synopsis AC_CXX_HAVE_VECTOR_AT
dnl
dnl If the implementation of the C++ library provides the method
dnl std::vector::at(std::size_t), define HAVE_VECTOR_AT.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Jan Langer <jan@langernetz.de>
dnl
AC_DEFUN([AC_CXX_HAVE_VECTOR_AT],
[AC_CACHE_CHECK(whether the compiler has std::vector::at (std::size_t),
ac_cv_cxx_have_vector_at,
[AC_REQUIRE([AC_CXX_NAMESPACES])
 AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <vector>
#ifdef HAVE_NAMESPACES
using namespace std;
#endif],[vector v (1); message.at (0); return 0;],
 ac_cv_cxx_have_vector_at=yes, ac_cv_cxx_have_vector_at=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_have_vector_at" = yes; then
 AC_DEFINE(HAVE_VECTOR_AT,,[define if the compiler has the method
std::vector::at (std::size_t)])
fi
])dnl
dnl @synopsis AC_CXX_MEMBER_CONSTANTS
dnl
dnl If the compiler supports member constants, define HAVE_MEMBER_CONSTANTS.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_MEMBER_CONSTANTS],
[AC_CACHE_CHECK(whether the compiler supports member constants,
ac_cv_cxx_member_constants,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([class C {public: static const int i = 0;}; const int C::i;],
[return C::i;],
 ac_cv_cxx_member_constants=yes, ac_cv_cxx_member_constants=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_member_constants" = yes; then
  AC_DEFINE(HAVE_MEMBER_CONSTANTS,,[define if the compiler supports member constants])
fi
])
dnl @synopsis AC_CXX_MEMBER_TEMPLATES
dnl
dnl If the compiler supports member templates, define HAVE_MEMBER_TEMPLATES.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_MEMBER_TEMPLATES],
[AC_CACHE_CHECK(whether the compiler supports member templates,
ac_cv_cxx_member_templates,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([
template<class T, int N> class A
{ public:
  template<int N2> A<T,N> operator=(const A<T,N2>& z) { return A<T,N>(); }
};],[A<double,4> x; A<double,7> y; x = y; return 0;],
 ac_cv_cxx_member_templates=yes, ac_cv_cxx_member_templates=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_member_templates" = yes; then
  AC_DEFINE(HAVE_MEMBER_TEMPLATES,,[define if the compiler supports member templates])
fi
])
dnl @synopsis AC_CXX_MEMBER_TEMPLATES_OUTSIDE_CLASS
dnl
dnl If the compiler supports member templates outside the class declaration,
dnl define HAVE_MEMBER_TEMPLATES_OUTSIDE_CLASS.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_MEMBER_TEMPLATES_OUTSIDE_CLASS],
[AC_CACHE_CHECK(whether the compiler supports member templates outside the class declaration,
ac_cv_cxx_member_templates_outside_class,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([
template<class T, int N> class A
{ public :
  template<int N2> A<T,N> operator=(const A<T,N2>& z);
};
template<class T, int N> template<int N2>
A<T,N> A<T,N>::operator=(const A<T,N2>& z){ return A<T,N>(); }],[
A<double,4> x; A<double,7> y; x = y; return 0;],
 ac_cv_cxx_member_templates_outside_class=yes, ac_cv_cxx_member_templates_outside_class=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_member_templates_outside_class" = yes; then
  AC_DEFINE(HAVE_MEMBER_TEMPLATES_OUTSIDE_CLASS,,
            [define if the compiler supports member templates outside the class declaration])
fi
])
dnl @synopsis AC_CXX_MUTABLE
dnl
dnl If the compiler allows modifying class data members flagged with
dnl the mutable keyword even in const objects (for example in the
dnl body of a const member function), define HAVE_MUTABLE.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_MUTABLE],
[AC_CACHE_CHECK(whether the compiler supports the mutable keyword,
ac_cv_cxx_mutable,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([
class A { mutable int i;
          public:
          int f (int n) const { i = n; return i; }
        };
],[A a; return a.f (1);],
 ac_cv_cxx_mutable=yes, ac_cv_cxx_mutable=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_mutable" = yes; then
  AC_DEFINE(HAVE_MUTABLE,,[define if the compiler supports the mutable keyword])
fi
])
dnl @synopsis AC_CXX_NAMESPACES
dnl
dnl If the compiler can prevent names clashes using namespaces, define
dnl HAVE_NAMESPACES.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_NAMESPACES],
[AC_CACHE_CHECK(whether the compiler implements namespaces,
ac_cv_cxx_namespaces,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([namespace Outer { namespace Inner { int i = 0; }}],
                [using namespace Outer::Inner; return i;],
 ac_cv_cxx_namespaces=yes, ac_cv_cxx_namespaces=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_namespaces" = yes; then
  AC_DEFINE(HAVE_NAMESPACES,,[define if the compiler implements namespaces])
fi
])
dnl @synopsis AC_CXX_NCEG_RESTRICT
dnl
dnl If the compiler supports the Numerical C Extensions Group
dnl restrict keyword, define HAVE_NCEG_RESTRICT.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_NCEG_RESTRICT],
[AC_CACHE_CHECK(whether the compiler supports the Numerical C Extensions Group restrict keyword,
ac_cv_cxx_nceg_restrict,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([
void add(int length, double * restrict a,
         const double * restrict b, const double * restrict c)
{ for (int i=0; i < length; ++i) a[i] = b[i] + c[i];}
],[double a[10], b[10], c[10];
for (int i=0; i < 10; ++i) { a[i] = 0.0;  b[i] = 0.0; c[i] = 0.0;}
add(10,a,b,c);
return 0;],
 ac_cv_cxx_nceg_restrict=yes, ac_cv_cxx_nceg_restrict=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_nceg_restrict" = yes; then
  AC_DEFINE(HAVE_NCEG_RESTRICT,,
            [define if  the compiler supports the Numerical C Extensions Group restrict keyword])
fi
])
dnl @synopsis AC_CXX_NEW_FOR_SCOPING
dnl
dnl If the compiler accepts the new for scoping rules (the scope of a
dnl variable declared inside the parentheses is restricted to the
dnl for-body), define HAVE_NEW_FOR_SCOPING.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_NEW_FOR_SCOPING],
[AC_CACHE_CHECK(whether the compiler accepts the new for scoping rules,
ac_cv_cxx_new_for_scoping,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE(,[
  int z = 0;
  for (int i = 0; i < 10; ++i)
    z = z + i;
  for (int i = 0; i < 10; ++i)
    z = z - i;
  return z;],
 ac_cv_cxx_new_for_scoping=yes, ac_cv_cxx_new_for_scoping=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_new_for_scoping" = yes; then
  AC_DEFINE(HAVE_NEW_FOR_SCOPING,,[define if the compiler accepts the new for scoping rules])
fi
])
dnl @synopsis AC_CXX_OLD_FOR_SCOPING
dnl
dnl If the compiler accepts the old for scoping rules (the scope of a
dnl variable declared inside the parentheses extends outside the
dnl for-body), define HAVE_OLD_FOR_SCOPING. Note that some
dnl compilers (notably g++ and egcs) support both new and old
dnl rules since they accept the old rules and only generate a warning.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_OLD_FOR_SCOPING],
[AC_CACHE_CHECK(whether the compiler accepts the old for scoping rules,
ac_cv_cxx_old_for_scoping,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE(,[int z;for (int i=0; i < 10; ++i)z=z+i;z=i;return z;],
 ac_cv_cxx_old_for_scoping=yes, ac_cv_cxx_old_for_scoping=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_old_for_scoping" = yes; then
  AC_DEFINE(HAVE_OLD_FOR_SCOPING,,[define if the compiler accepts the old for scoping rules])
fi
])
dnl @synopsis AC_CXX_PARTIAL_ORDERING
dnl
dnl If the compiler supports partial ordering, define HAVE_PARTIAL_ORDERING.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_PARTIAL_ORDERING],
[AC_CACHE_CHECK(whether the compiler supports partial ordering,
ac_cv_cxx_partial_ordering,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([
template<int N> struct I {};
template<class T> struct A
{  int r;
   template<class T1, class T2> int operator() (T1, T2)       { r = 0; return r; }
   template<int N1, int N2>     int operator() (I<N1>, I<N2>) { r = 1; return r; }
};],[A<float> x, y; I<0> a; I<1> b; return x (a,b) + y (float(), double());],
 ac_cv_cxx_partial_ordering=yes, ac_cv_cxx_partial_ordering=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_partial_ordering" = yes; then
  AC_DEFINE(HAVE_PARTIAL_ORDERING,,
            [define if the compiler supports partial ordering])
fi
])
dnl @synopsis AC_CXX_PARTIAL_SPECIALIZATION
dnl
dnl If the compiler supports partial specialization,
dnl define HAVE_PARTIAL_SPECIALIZATION.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_PARTIAL_SPECIALIZATION],
[AC_CACHE_CHECK(whether the compiler supports partial specialization,
ac_cv_cxx_partial_specialization,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([
template<class T, int N> class A            { public : enum e { z = 0 }; };
template<int N>          class A<double, N> { public : enum e { z = 1 }; };
template<class T>        class A<T, 2>      { public : enum e { z = 2 }; };
],[return (A<int,3>::z == 0) && (A<double,3>::z == 1) && (A<float,2>::z == 2);],
 ac_cv_cxx_partial_specialization=yes, ac_cv_cxx_partial_specialization=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_partial_specialization" = yes; then
  AC_DEFINE(HAVE_PARTIAL_SPECIALIZATION,,
            [define if the compiler supports partial specialization])
fi
])
dnl @synopsis AC_CXX_REINTERPRET_CAST
dnl
dnl If the compiler supports reinterpret_cast<>, define HAVE_REINTERPRET_CAST.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_REINTERPRET_CAST],
[AC_CACHE_CHECK(whether the compiler supports reinterpret_cast<>,
ac_cv_cxx_reinterpret_cast,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <typeinfo>
class Base { public : Base () {} virtual void f () = 0;};
class Derived : public Base { public : Derived () {} virtual void f () {} };
class Unrelated { public : Unrelated () {} };
int g (Unrelated&) { return 0; }],[
Derived d;Base& b=d;Unrelated& e=reinterpret_cast<Unrelated&>(b);return g(e);],
 ac_cv_cxx_reinterpret_cast=yes, ac_cv_cxx_reinterpret_cast=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_reinterpret_cast" = yes; then
  AC_DEFINE(HAVE_REINTERPRET_CAST,,
            [define if the compiler supports reinterpret_cast<>])
fi
])
dnl @synopsis AC_CXX_RTTI
dnl
dnl If the compiler supports Run-Time Type Identification (typeinfo
dnl header and typeid keyword), define HAVE_RTTI.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_RTTI],
[AC_CACHE_CHECK(whether the compiler supports Run-Time Type Identification,
ac_cv_cxx_rtti,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <typeinfo>
class Base { public :
             Base () {}
             virtual int f () { return 0; }
           };
class Derived : public Base { public :
                              Derived () {}
                              virtual int f () { return 1; }
                            };
],[Derived d;
Base *ptr = &d;
return typeid (*ptr) == typeid (Derived);
],
 ac_cv_cxx_rtti=yes, ac_cv_cxx_rtti=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_rtti" = yes; then
  AC_DEFINE(HAVE_RTTI,,
            [define if the compiler supports Run-Time Type Identification])
fi
])
dnl @synopsis AC_CXX_STATIC_CAST
dnl
dnl If the compiler supports static_cast<>, define HAVE_STATIC_CAST.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_STATIC_CAST],
[AC_CACHE_CHECK(whether the compiler supports static_cast<>,
ac_cv_cxx_static_cast,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <typeinfo>
class Base { public : Base () {} virtual void f () = 0; };
class Derived : public Base { public : Derived () {} virtual void f () {} };
int g (Derived&) { return 0; }],[
Derived d; Base& b = d; Derived& s = static_cast<Derived&> (b); return g (s);],
 ac_cv_cxx_static_cast=yes, ac_cv_cxx_static_cast=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_static_cast" = yes; then
  AC_DEFINE(HAVE_STATIC_CAST,,
            [define if the compiler supports static_cast<>])
fi
])
dnl @synopsis AC_CXX_TEMPLATE_KEYWORD_QUALIFIER
dnl
dnl If the compiler supports use of the template keyword as a qualifier,
dnl define HAVE_TEMPLATE_KEYWORD_QUALIFIER.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen, Bernardo Innocenti, and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_TEMPLATE_KEYWORD_QUALIFIER],
[AC_CACHE_CHECK(whether the compiler supports use of the template keyword as a qualifier,
ac_cv_cxx_template_keyword_qualifier,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([
  class X
  {
    public:
    template<int> void member() {}
    template<int> static void static_member() {}
  };
  template<class T> void f(T* p)
  {
    p->template member<200>(); // OK: < starts template argument
    T::template static_member<100>(); // OK: < starts explicit qualification
  }
],[X x; f(&x); return 0;],
 ac_cv_cxx_template_keyword_qualifier=yes, ac_cv_cxx_template_keyword_qualifier=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_template_keyword_qualifier" = yes; then
  AC_DEFINE(HAVE_TEMPLATE_KEYWORD_QUALIFIER,,
            [define if the compiler supports use of the template keyword as a qualifier])
fi
])
dnl @synopsis AC_CXX_TEMPLATE_OBJS
dnl
dnl This macro tries to find the place where the objects files resulting
dnl from templates instantiations are stored and the associated compiler
dnl flags. This is particularly useful to include these files in
dnl libraries. Currently only g++/egcs and SUN CC are supported (there is
dnl nothing to be done for the formers while the latter uses directory
dnl ./Templates.DB if you use the -ptr. flag). This macro sets the
dnl CXXFLAGS if needed, it also sets the output variable
dnl TEMPLATES_OBJ. Note that if you use libtool, this macro does work
dnl correctly with the SUN compilers ONLY while building static
dnl libraries. Since there are sometimes problems with exception handling
dnl with multiple levels of shared libraries even with g++ on this
dnl platform, you may wish to enforce the usage of static libraires
dnl there. You can do this by putting the following statements in your
dnl configure.in file:
dnl
dnl    AC_CANONICAL_HOST
dnl    case x$host_os in
dnl      xsolaris*) AC_DISABLE_SHARED ;;
dnl    esac
dnl    AM_PROG_LIBTOOL
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Luc Maisonobe
dnl
AC_DEFUN([AC_CXX_TEMPLATE_OBJS],
[AC_CACHE_CHECK(where template objects are stored, ac_cv_cxx_templobjs,
 [ ac_cv_cxx_templobjs='unknown'
   if test "$GXX" = yes; then
     ac_cv_cxx_templobjs='nowhere'
   else
     case $CXX in
       CC|*/CC)
        cat > conftest.cc <<EOF
template<class T> class A { public : A () {} };
template<class T> void f (const A<T>&) {}
main()
{ A<double> d;
  A<int> i;
  f (d);
  f (i);
  return 0;
}
EOF
        if test "$ac_cv_cxx_templobjs" = 'unknown' ; then
          if test -d Templates.DB ; then
            rm -fr Templates.DB
          fi
          if $CXX $CXXFLAGS -ptr. -c conftest.cc 1> /dev/null 2>&1; then
            if test -d Templates.DB ; then
#             this should be Sun CC <= 4.2
              CXXFLAGS="$CXXFLAGS -ptr."
              if test x"$LIBTOOL" = x ; then
                ac_cv_cxx_templobjs='Templates.DB/*.o'
              else
                ac_cv_cxx_templobjs='Templates.DB/*.lo'
              fi
              rm -fr Templates.DB
            fi
          fi
        fi
        if test "$ac_cv_cxx_templobjs" = 'unknown' ; then
          if test -d SunWS_cache ; then
            rm -fr SunWS_cache
          fi
          if $CXX $CXXFLAGS -c conftest.cc 1> /dev/null 2>&1; then
            if test -d SunWS_cache ; then
#             this should be Sun WorkShop C++ compiler 5.x
#             or Sun Forte C++ compiler >= 6.x
              if test x"$LIBTOOL" = x ; then
                ac_cv_cxx_templobjs='SunWS_cache/*/*.o'
              else
                ac_cv_cxx_templobjs='SunWS_cache/*/*.lo'
              fi
              rm -fr SunWS_cache
            fi
          fi
        fi
        rm -f conftest* ;;
     esac
   fi
   case "x$ac_cv_cxx_templobjs" in
     xunknown|xnowhere)
     TEMPLATE_OBJS="" ;;
     *)
     TEMPLATE_OBJS="$ac_cv_cxx_templobjs" ;;
   esac
   AC_SUBST(TEMPLATE_OBJS)])])
dnl @synopsis AC_CXX_TEMPLATE_QUALIFIED_BASE_CLASS
dnl
dnl If the compiler supports template-qualified base class specifiers,
dnl define HAVE_TEMPLATE_QUALIFIED_BASE_CLASS.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_TEMPLATE_QUALIFIED_BASE_CLASS],
[AC_CACHE_CHECK(whether the compiler supports template-qualified base class specifiers,
ac_cv_cxx_template_qualified_base_class,
[AC_REQUIRE([AC_CXX_TYPENAME])
 AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([
#ifndef HAVE_TYPENAME
 #define typename
#endif
class Base1 { public : int f () const { return 1; } };
class Base2 { public : int f () const { return 0; } };
template<class X> struct base_trait        { typedef Base1 base; };
                  struct base_trait<float> { typedef Base2 base; };
template<class T> class Weird : public base_trait<T>::base
{ public :
  typedef typename base_trait<T>::base base;
  int g () const { return f (); }
};],[ Weird<float> z; return z.g ();],
 ac_cv_cxx_template_qualified_base_class=yes, ac_cv_cxx_template_qualified_base_class=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_template_qualified_base_class" = yes; then
  AC_DEFINE(HAVE_TEMPLATE_QUALIFIED_BASE_CLASS,,
            [define if the compiler supports template-qualified base class specifiers])
fi
])
dnl @synopsis AC_CXX_TEMPLATE_QUALIFIED_RETURN_TYPE
dnl
dnl If the compiler supports template-qualified return types, define
dnl HAVE_TEMPLATE_QUALIFIED_RETURN_TYPE.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_TEMPLATE_QUALIFIED_RETURN_TYPE],
[AC_CACHE_CHECK(whether the compiler supports template-qualified return types,
ac_cv_cxx_template_qualified_return_type,
[AC_REQUIRE([AC_CXX_TYPENAME])
 AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([
#ifndef HAVE_TYPENAME
 #define typename
#endif
template<class X, class Y> struct promote_trait             { typedef X T; };
template<>                 struct promote_trait<int, float> { typedef float T; };
template<class T> class A { public : A () {} };
template<class X, class Y>
A<typename promote_trait<X,Y>::T> operator+ (const A<X>&, const A<Y>&)
{ return A<typename promote_trait<X,Y>::T>(); }
],[A<int> x; A<float> y; A<float> z = x + y; return 0;],
 ac_cv_cxx_template_qualified_return_type=yes, ac_cv_cxx_template_qualified_return_type=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_template_qualified_return_type" = yes; then
  AC_DEFINE(HAVE_TEMPLATE_QUALIFIED_RETURN_TYPE,,
            [define if the compiler supports template-qualified return types])
fi
])
dnl @synopsis AC_CXX_TEMPLATES_AS_TEMPLATE_ARGUMENTS
dnl
dnl If the compiler supports templates as template arguments, define
dnl HAVE_TEMPLATES_AS_TEMPLATE_ARGUMENTS.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_TEMPLATES_AS_TEMPLATE_ARGUMENTS],
[AC_CACHE_CHECK(whether the compiler supports templates as template arguments,
ac_cv_cxx_templates_as_template_arguments,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([
template<class T> class allocator { public : allocator() {}; };
template<class X, template<class Y> class T_alloc>
class A { public : A() {} private : T_alloc<X> alloc_; };
],[A<double, allocator> x; return 0;],
 ac_cv_cxx_templates_as_template_arguments=yes, ac_cv_cxx_templates_as_template_arguments=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_templates_as_template_arguments" = yes; then
  AC_DEFINE(HAVE_TEMPLATES_AS_TEMPLATE_ARGUMENTS,,
            [define if the compiler supports templates as template arguments])
fi
])
dnl @synopsis AC_CXX_TEMPLATE_SCOPED_ARGUMENT_MATCHING
dnl
dnl If the compiler supports function matching with argument types which are
dnl template scope-qualified, define HAVE_TEMPLATE_SCOPED_ARGUMENT_MATCHING.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_TEMPLATE_SCOPED_ARGUMENT_MATCHING],
[AC_CACHE_CHECK(whether the compiler supports function matching with argument types which are template scope-qualified,
ac_cv_cxx_template_scoped_argument_matching,
[AC_REQUIRE([AC_CXX_TYPENAME])
 AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([
#ifndef HAVE_TYPENAME
 #define typename
#endif
template<class X> class A { public : typedef X W; };
template<class Y> class B {};
template<class Y> void operator+(B<Y> d1, typename Y::W d2) {}
],[B<A<float> > z; z + 0.5f; return 0;],
 ac_cv_cxx_template_scoped_argument_matching=yes, ac_cv_cxx_template_scoped_argument_matching=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_template_scoped_argument_matching" = yes; then
  AC_DEFINE(HAVE_TEMPLATE_SCOPED_ARGUMENT_MATCHING,,
            [define if the compiler supports function matching with argument types which are template scope-qualified])
fi
])
dnl @synopsis AC_CXX_TEMPLATES
dnl
dnl If the compiler supports basic templates, define HAVE_TEMPLATES.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_TEMPLATES],
[AC_CACHE_CHECK(whether the compiler supports basic templates,
ac_cv_cxx_templates,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([template<class T> class A {public:A(){}};
template<class T> void f(const A<T>& ){}],[
A<double> d; A<int> i; f(d); f(i); return 0;],
 ac_cv_cxx_templates=yes, ac_cv_cxx_templates=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_templates" = yes; then
  AC_DEFINE(HAVE_TEMPLATES,,[define if the compiler supports basic templates])
fi
])
dnl @synopsis AC_CXX_TYPENAME
dnl
dnl If the compiler recognizes the typename keyword, define HAVE_TYPENAME.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_TYPENAME],
[AC_CACHE_CHECK(whether the compiler recognizes typename,
ac_cv_cxx_typename,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([template<typename T>class X {public:X(){}};],
[X<float> z; return 0;],
 ac_cv_cxx_typename=yes, ac_cv_cxx_typename=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_typename" = yes; then
  AC_DEFINE(HAVE_TYPENAME,,[define if the compiler recognizes typename])
fi
])
dnl @synopsis AC_CXX_USE_NUMTRAIT
dnl
dnl If the compiler supports numeric traits promotions, define
dnl HAVE_USE_NUMTRAIT.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>
dnl
AC_DEFUN([AC_CXX_USE_NUMTRAIT],
[AC_CACHE_CHECK(whether the compiler supports numeric traits promotions,
ac_cv_cxx_use_numtrait,
[AC_REQUIRE([AC_CXX_TYPENAME])
 AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([
#ifndef HAVE_TYPENAME
 #define typename
#endif
template<class T_numtype> class SumType       { public : typedef T_numtype T_sumtype;   };
template<>                class SumType<char> { public : typedef int T_sumtype; };
template<class T> class A {};
template<class T> A<typename SumType<T>::T_sumtype> sum(A<T>)
{ return A<typename SumType<T>::T_sumtype>(); }
],[A<float> x; sum(x); return 0;],
 ac_cv_cxx_use_numtrait=yes, ac_cv_cxx_use_numtrait=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_use_numtrait" = yes; then
  AC_DEFINE(HAVE_USE_NUMTRAIT,,[define if the compiler supports numeric traits promotions])
fi
])
dnl @synopsis AC_CXX_VERBOSE_TERMINATE_HANDLER
dnl
dnl If the compiler does have the verbose terminate handler, define
dnl HAVE_VERBOSE_TERMINATE_HANDLER.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Lapo Luchini <lapo@lapo.it>
dnl
AC_DEFUN([AC_CXX_VERBOSE_TERMINATE_HANDLER],
[AC_CACHE_CHECK(whether the compiler has __gnu_cxx::__verbose_terminate_handler,
ac_cv_verbose_terminate_handler,
[
  AC_REQUIRE([AC_CXX_EXCEPTIONS])
  AC_REQUIRE([AC_CXX_NAMESPACES])
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_TRY_COMPILE(
    [#include <exception>], [std::set_terminate(__gnu_cxx::__verbose_terminate_handler);],
    ac_cv_verbose_terminate_handler=yes, ac_cv_verbose_terminate_handler=no
  )
  AC_LANG_RESTORE
])
if test "$ac_cv_verbose_terminate_handler" = yes; then
  AC_DEFINE(HAVE_VERBOSE_TERMINATE_HANDLER, , [define if the compiler has __gnu_cxx::__verbose_terminate_handler])
fi
])
dnl @synopsis AX_CXX_GCC_ABI_DEMANGLE
dnl
dnl If the compiler supports GCC C++ ABI name demangling (has header cxxabi.h 
dnl and abi::__cxa_demangle() function), define HAVE_GCC_ABI_DEMANGLE
dnl
dnl Adapted from AC_CXX_RTTI by Luc Maisonobe
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Neil Ferguson <nferguso@eso.org>
dnl
AC_DEFUN([AX_CXX_GCC_ABI_DEMANGLE],
[AC_CACHE_CHECK(whether the compiler supports GCC C++ ABI name demangling, 
ac_cv_cxx_gcc_abi_demangle,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 AC_TRY_COMPILE([#include <typeinfo>
#include <cxxabi.h>
#include <string>

template<typename TYPE>
class A {};
],[A<int> instance;
int status = 0;
char* c_name = 0;

c_name = abi::__cxa_demangle(typeid(instance).name(), 0, 0, &status);
        
std::string name(c_name);
free(c_name);

return name == "A<int>";
],
 ac_cv_cxx_gcc_abi_demangle=yes, ac_cv_cxx_gcc_abi_demangle=no)
 AC_LANG_RESTORE
])
if test "$ac_cv_cxx_gcc_abi_demangle" = yes; then
  AC_DEFINE(HAVE_GCC_ABI_DEMANGLE,1,
            [define if the compiler supports GCC C++ ABI name demangling]) 
fi
])
dnl @synopsis MDL_CXX_FUNCTION_TRY_BLOCKS
dnl
dnl If the C++ compiler supports function try blocks, define
dnl `HAVE_FUNCTION_TRY_BLOCKS'.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Matthew D. Langston <langston@SLAC.Stanford.EDU>
dnl
AC_DEFUN([MDL_CXX_FUNCTION_TRY_BLOCKS],
[
AC_REQUIRE([AC_PROG_CXX])
changequote(,)dnl
AC_MSG_CHECKING(whether ${CXX} supports function try blocks)
changequote([,])dnl
AC_CACHE_VAL(mdl_cv_have_function_try_blocks,
[
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_TRY_COMPILE([void foo() try{} catch( ... ){}],
[foo();],
mdl_cv_have_function_try_blocks=yes,
mdl_cv_have_function_try_blocks=no)
AC_LANG_RESTORE
])
AC_MSG_RESULT($mdl_cv_have_function_try_blocks)
if test "$mdl_cv_have_function_try_blocks" = yes; then
AC_DEFINE(HAVE_FUNCTION_TRY_BLOCKS)
fi])
dnl @synopsis mni_CXX_HAVE_KOENIG_LOOKUP
dnl
dnl Define CXX_HAVE_KOENIG_LOOKUP if the C++ compiler has
dnl argument-dependent name lookup (a.k.a. Koenig lookup).
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Steve Robbins
dnl
AC_DEFUN([mni_CXX_HAVE_KOENIG_LOOKUP],
    [AC_CACHE_CHECK(whether the compiler implements Koenig lookup,
                    ac_cv_cxx_have_koenig_lookup,
                    [AC_LANG_PUSH(C++)
                     AC_TRY_COMPILE([
    namespace N1 {
        class C {};
        void f1(const C& c) {}
    }

    namespace N2 {
        void f2() {
            N1::C x;
            f1(x);     // resolves to N1::f1() if we have Koenig lookup,
                       // otherwise this will fail to compile.
        }
    }
    ],[],
                     ac_cv_cxx_have_koenig_lookup=yes,
                     ac_cv_cxx_have_koenig_lookup=no)
                     AC_LANG_POP])
    if test "$ac_cv_cxx_have_koenig_lookup" = yes; then
        AC_DEFINE(CXX_HAVE_KOENIG_LOOKUP,1,
                  [define to 1 if the compiler implements Koenig lookup])
    fi
])
dnl RTB_EXPAND_DIR(VARNAME, DIR)
dnl expands occurrences of ${prefix} and ${exec_prefix} in the given DIR,
dnl and assigns the resulting string to VARNAME
dnl example: RTB_EXPAND_DIR(ROBOTDIR, "${RTB_DIR}/Robots")
dnl eg, then: AC_DEFINE_UNQUOTED(ROBOTDIR, "$ROBOTDIR")
dnl by Alexandre Oliva 
dnl from http://www.cygnus.com/ml/automake/1998-Aug/0040.html
AC_DEFUN(RTB_EXPAND_DIR, [
        $1=$2
        $1=`(
            test "x$prefix" = xNONE && prefix="$ac_default_prefix"
            test "x$exec_prefix" = xNONE && exec_prefix="${prefix}"
            eval echo \""[$]$1"\"
        )`
])
dnl @synopsis ETR_SOCKET_NSL
dnl
dnl This macro figures out what libraries are required on this platform
dnl to link sockets programs.  It's usually -lsocket and/or -lnsl or
dnl neither.  We test for all three combinations.
dnl
dnl @version $Id: acinclude.m4,v 1.1 2005/04/13 15:11:11 jonico Exp $
dnl @author Warren Young <warren@etr-usa.com>
dnl
AC_DEFUN([ETR_SOCKET_NSL],
[
AC_CACHE_CHECK(for libraries containing socket functions,
ac_cv_socket_libs, [
        oCFLAGS=$CFLAGS

        AC_TRY_LINK([
                        #include <sys/types.h>
                        #include <sys/socket.h>
                        #include <netinet/in.h>
                        #include <arpa/inet.h>
                ],
                [
                        struct in_addr add;
                        int sd = socket(AF_INET, SOCK_STREAM, 0);
                        inet_ntoa(add);
                ],
                ac_cv_socket_libs=-lc, ac_cv_socket_libs=no)

        if test x"$ac_cv_socket_libs" = "xno"
        then
                CFLAGS="$oCFLAGS -lsocket"
                AC_TRY_LINK([
                                #include <sys/types.h>
                                #include <sys/socket.h>
                                #include <netinet/in.h>
                                #include <arpa/inet.h>
                        ],
                        [
                                struct in_addr add;
                                int sd = socket(AF_INET, SOCK_STREAM, 0);
                                inet_ntoa(add);
                        ],
                        ac_cv_socket_libs=-lsocket, ac_cv_socket_libs=no)
        fi

        if test x"$ac_cv_socket_libs" = "xno"
        then
                CFLAGS="$oCFLAGS -lsocket -lnsl"
                AC_TRY_LINK([
                                #include <sys/types.h>
                                #include <sys/socket.h>
                                #include <netinet/in.h>
                                #include <arpa/inet.h>
                        ],
                        [
                                struct in_addr add;
                                int sd = socket(AF_INET, SOCK_STREAM, 0);
                                inet_ntoa(add);
                        ],
                        ac_cv_socket_libs="-lsocket -lnsl", ac_cv_socket_libs=no)
        fi

        CFLAGS=$oCFLAGS
])

        if test x"$ac_cv_socket_libs" = "xno"
        then
                AC_MSG_ERROR([Cannot find socket libraries])
        elif test x"$ac_cv_socket_libs" = "x-lc"
        then
                ETR_SOCKET_LIBS=""
        else
                ETR_SOCKET_LIBS="$ac_cv_socket_libs"
        fi

        AC_SUBST(ETR_SOCKET_LIBS)
]) dnl ETR_SOCKET_NSL
