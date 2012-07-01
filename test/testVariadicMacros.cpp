///All the following example cases of macro usage were given by Yuriy


/// Helper macro for the following one to resolve macros inside its arguments

//#define _XTL_STRING_LITERAL(x)  #x

/// Macro to stringize some expression.

//#define XTL_STRING_LITERAL(x)   _XTL_STRING_LITERAL(x)

 

/// Somehow operator ## == (or any other) results in error in GCC, so we use

/// a trick to let users build operator name when they need.

//#define XTL_IDENTITY(A) A

//#define XTL_CONCATENATE(A,B) XTL_IDENTITY(A)XTL_IDENTITY(B)

 

///Function-like variadic macro:

/// Macros to use compiler's branch hinting.

/// \note These macros are only to be used in Case macro expansion, not in

///       user's code since they explicitly expect a pointer argument

/// \note We use ... (__VA_ARGS__ parameters) to allow expressions

///       containing comma as argument. Essentially this is a one arg macro

//#define   XTL_LIKELY(...) (__builtin_expect((long int)(__VA_ARGS__), 1))

//#define XTL_UNLIKELY(...) (__builtin_expect((long int)(__VA_ARGS__), 0))

 

///A macro to apply another one to variadic arguments (note variadic arguments have to 
///be in () here, e.g. (__VA_ARGS__):

 

/// MS Visual C++ 2005 (in which variadic macros became supported) till at

/// least MS Visual C++ 2010 has a bug in passing __VA_ARGS__ to subsequent

/// macros as a single token, which results in:

///     warning C4003: not enough actual parameters for macro 'XTL_ARG_N'

/// and incorrect behavior. The workaround used here is described at:

/// https://connect.microsoft.com/VisualStudio/feedback/details/380090/variadic-macro-replacement

//#define XTL_APPLY_VARIADIC_MACRO(macro,tuple) macro tuple

 

/// A macro used to count a number of variadic arguments.

/// \note Using this macro without arguments violates 6.10.3p4 of ISO C99

///       and thus it cannot be used to detect zero arguments.

/// \note This macro was invented by Laurent Deniau and can be found here:

/// \see  http://groups.google.com/group/comp.std.c/browse_thread/thread/77ee8c8f92e4a3fb/346fc464319b1ee5

//#define XTL_NARG(...)  XTL_APPLY_VARIADIC_MACRO(XTL_NARG_,(__VA_ARGS__,XTL_RSEQ_N()))

//#define XTL_NARG_(...) XTL_APPLY_VARIADIC_MACRO(XTL_ARG_N,(__VA_ARGS__))

 

/// The same as above but assumes a dummy first argument that is not counted

/// in order to deal with the fact that regular @XTL_NARG cannot cope with

/// zero arguments.

/// \note We need here 0 to be a real 0 and not something that evaluates to 0

///       as is done in some solutions to this problem of NARG, because we

///       append this number to a name to form another macro!

//#define XTL_NARG_EX(...)  XTL_APPLY_VARIADIC_MACRO(XTL_NARG_EX_,(__VA_ARGS__,XTL_RSEQ_N()))

//#define XTL_NARG_EX_(...) XTL_APPLY_VARIADIC_MACRO(XTL_ARG_N_EX,(__VA_ARGS__))

 

/// Separating a single type parameter among multiple in variadic arguments:

 

//template<typename T>              struct get_first_param;

//template<typename R, typename P1> struct get_first_param<R(P1)> { typedef P1 type; };

 

/// This is a helper macro to be able to pass template instantiations as an

/// argument of a macro. The actual problem is that template instantiations

/// may contain commas in their argument list, which are treated as macro

/// argument separator by the preprocessor. Enclosing such type argument in

/// () directly will not work because (Type) is a conversion expression in C

/// and compiler reports an error when we try to user (Type) instead of a Type.

/// \example

///       Case(TypeArg(MyMap<int,string>),x,y,z) ...

/// The solution used here is based on the following discussion:

/// \see http://stackoverflow.com/questions/4295890/trouble-with-template-parameters-used-in-macros

//#define  TypeArg_(X)           get_first_param<void X>::type

//#define  TypeArg(...)   TypeArg_((__VA_ARGS__))

/// Same as @TypeArg but to be used in the template context

//#define TTypeArg_(X)  typename get_first_param<void X>::type

//#define TTypeArg(...)  TTypeArg_((__VA_ARGS__))


template<typename A>
struct AClass {};

#define specialize_AClass(...)\
template<> struct AClass< __VA_ARGS__ > { __VA_ARGS__ a; };

specialize_AClass(int) //ok

specialize_AClass(std::vector<int,std::allocator<int> >) //error



