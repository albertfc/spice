// config.h - Generated from config.h.in by configure.
#pragma once

// Define to the one symbol short name of this package.
#define USTL_NAME	"ustl"
// Define to the full name and version of this package.
#define USTL_STRING	"ustl v2.8"
// Define to the version of this package.
#define USTL_VERSION	0x280
// Define to the address where bug reports for this package should be sent.
#define USTL_BUGREPORT	"Mike Sharov <msharov@users.sourceforge.net>"

/// Define to 1 if you want stream operations to throw exceptions on
/// insufficient data or insufficient space. All these errors should
/// be preventable in output code; the input code should verify the
/// data in a separate step. It slows down stream operations a lot,
/// but it is your decision. By default only debug builds throw.
///
#undef WANT_STREAM_BOUNDS_CHECKING

#if !WANT_STREAM_BOUNDS_CHECKING && !defined(NDEBUG)
    #undef WANT_STREAM_BOUNDS_CHECKING
#endif

/// Define to 1 if you want backtrace symbols demangled.
/// This adds some 15k to the library size, and requires that you link it and
/// any executables you make with the -rdynamic flag (increasing library size
/// even more). By default only the debug build does this.
#undef WANT_NAME_DEMANGLING

#if !WANT_NAME_DEMANGLING && !defined(NDEBUG)
    #define WANT_NAME_DEMANGLING 1
#endif

/// Define to 1 if you want to build without libstdc++
#define WITHOUT_LIBSTDCPP 1

/// Define GNU extensions if unavailable.
#ifndef __GNUC__
    /// GCC (and some other compilers) define '__attribute__'; ustl is using this
    /// macro to alert the compiler to flag inconsistencies in printf/scanf-like
    /// function calls.  Just in case '__attribute__' is undefined, make a dummy.
    /// 
    #ifndef __attribute__
	#define __attribute__(p)
    #endif
#endif
#define WEAKALIAS(sym)		__attribute__((weak,alias(sym)))
#if __GNUC__ >= 4
    #define DLL_EXPORT		__attribute__((visibility("default")))
    #define DLL_LOCAL		__attribute__((visibility("hidden")))
    #define INLINE		__attribute__((always_inline))
#else
    #define DLL_EXPORT
    #define DLL_LOCAL
    #define INLINE
#endif
#if __cplusplus >= 201103 && (!__GNUC__ || (__clang_major__ > 3 || (__clang_major__ == 3 && __clang_minor__ >= 2 && !__APPLE__)) || (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)))
    #define HAVE_CPP11 1
    #if __cplusplus >= 201402
	#define HAVE_CPP14 1
    #endif
#endif
#if !HAVE_CPP11
    #define constexpr
    #define override
    #define final
    #define nullptr		NULL
    #define noexcept		throw()
#endif
#if __GNUC__ >= 3 && (__i386__ || __x86_64__)
    /// GCC 3+ supports the prefetch directive, which some CPUs use to improve caching
    #define prefetch(p,rw,loc)	__builtin_prefetch(p,rw,loc)
#else
    #define prefetch(p,rw,loc)
#endif
#if __GNUC__ < 3
    /// __alignof__ returns the recommended alignment for the type
    #define __alignof__(v)	min(sizeof(v), sizeof(void*))
    /// This macro returns 1 if the value of x is known at compile time.
    #ifndef __builtin_constant_p
	#define __builtin_constant_p(x)	0
    #endif
#endif

/// gcc has decided that inline is just a suggestion
/// Define to 1 if when you say 'inline' you mean it!
#undef WANT_ALWAYS_INLINE

// Define to 1 if you have the <alloca.h> header file.
#define HAVE_ALLOCA_H 1

// Define to 1 if you have the <execinfo.h> header file.
#define HAVE_EXECINFO_H 1

/// Define to 1 if you have the <inttypes.h> header file.
#define HAVE_INTTYPES_H 1

// Define to 1 if you have the <math.h> header file.
#define HAVE_MATH_H 1

// Define to 1 if you have the <stdint.h> header file.
#define HAVE_STDINT_H 1

// Define to 1 if you have the <sys/mman.h> header file.
#define HAVE_SYS_MMAN_H 1

// Define to 1 if you have the <sys/types.h> header file.
#undef HAVE_SYS_TYPES_H

// Define to 1 if you have the <cxxabi.h> header file.
#if __GNUC__ >= 3
    #define HAVE_CXXABI_H 1
#endif

// Define to 1 if you have the __va_copy function
#define HAVE_VA_COPY 1

// Define to 1 if you have the 'strsignal' function.
#define HAVE_STRSIGNAL 1

// Define to 1 if you have the rintf function. Will use rint otherwise.
#if __linux__
    #define HAVE_RINTF 1
#endif

// Define to 1 if your compiler treats char as a separate type along with
// signed char and unsigned char. This will create overloads for char.
#define HAVE_THREE_CHAR_TYPES 1

// Define to 1 if you have 64 bit types available
#if (__GNUC__ || __x86_64__ || __APPLE__) && !BSD
    #define HAVE_INT64_T 1
#endif

// Define to 1 if you have the long long type
#undef HAVE_LONG_LONG

// Define to 1 if you want unrolled specializations for fill and copy
#define WANT_UNROLLED_COPY 1

// Define to byte sizes of types
#define SIZE_OF_CHAR		1
#define SIZE_OF_SHORT		__SIZEOF_SHORT__
#define SIZE_OF_INT		__SIZEOF_INT__
#define SIZE_OF_LONG		__SIZEOF_LONG__
/*#ifdef __SIZEOF_LONG_LONG__*/
/*#define HAVE_LONG_LONG 1	// long long type available*/
/*#define SIZE_OF_LONG_LONG	__SIZEOF_LONG_LONG__*/
/*#endif*/
#define SIZE_OF_POINTER		__SIZEOF_POINTER__
#define SIZE_OF_SIZE_T		__SIZEOF_SIZE_T__
#define SIZE_OF_BOOL		sizeof(bool)
#if __APPLE__ || (SIZE_OF_SIZE_T == SIZE_OF_LONG && SIZE_OF_SIZE_T != SIZE_OF_INT)
    #define SIZE_T_IS_LONG 1
#endif

// Byte order macros, converted in utypes.h
#define USTL_LITTLE_ENDIAN	__ORDER_LITTLE_ENDIAN__
#define USTL_BIG_ENDIAN		__ORDER_BIG_ENDIAN__
#define USTL_BYTE_ORDER		__BYTE_ORDER__

#if __i386__ || __x86_64__
    #define __x86__ 1
#endif

// GCC vector extensions
#if (__MMX__ || __SSE__) && __GNUC__ >= 3
    #undef HAVE_VECTOR_EXTENSIONS
#endif

#if __SSE__ && __GNUC__
    #define __sse_align	__attribute__((aligned(16)))
#else
    #define __sse_align
#endif

/*
#if HAVE_CPP11
    #define USTL_LIBSUPCPP_LINKARG	" -lsupc++"
    static_assert (sizeof(USTL_LIBSUPCPP_LINKARG) != sizeof(""),
	"configure was unable to find required library libsupc++.a . This "
	"may occur if your distribution violates the Linux Filesystem "
	"Standard by not storing system libraries in /us${prefix}/lib . To "
	"correct the problem, please find libsupc++.a and rerun configure "
	"with the --libdir=/us${prefix}/lib/libsupcppdir option, allowing the library "
	"to be found and for uSTL to be installed in the location expected "
	"by your distribution.");

#endif
*/
