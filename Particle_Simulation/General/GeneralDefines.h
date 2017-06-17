#pragma once

#define PREC double	// is eigentlich egal ob man float, double oder long double verwendet auf nem x64 system.....(Kein performance unterschied feststellbar)

#define _USE_MATH_DEFINES

#ifndef USE_BOOST
#define USE_BOOST //We use Boost!
#endif

#ifndef USE_PCG_RANDOM 
#define USE_PCG_RANDOM
#endif

#ifdef _MSC_VER
#define EIGEN_HAS_CXX11_NOEXCEPT 1
#define EIGEN_HAS_CXX11_CONTAINERS 1
#define EIGEN_HAS_CXX11_MATH 1
#define EIGEN_HAS_CONSTEXPR 1
#define EIGEN_HAS_RVALUE_REFERENCES 1

#define EIGEN_VECTORIZE_SSE3
#define EIGEN_VECTORIZE_SSSE3
#define EIGEN_VECTORIZE_SSE4_1
#define EIGEN_VECTORIZE_SSE4_2
#define EIGEN_VECTORIZE_AVX 1
#define EIGEN_VECTORIZE_AVX2 1
//#define EIGEN_VECTORIZE_AVX512 1 (*Visual Studio does not have intrinsics for this*)
#define EIGEN_VECTORIZE_FMA 1
#endif
//#define EIGEN_USE_MKL_ALL //Does not make a difference in this project
//#define EIGEN_USE_MKL_VML
//#define EIGEN_USE_BLAS

//#define EIGEN_DONT_VECTORIZE 1
//#define EIGEN_DONT_ALIGN 1
#define EIGEN_FAST_MATH 0

//#define EIGEN_UNROLLING_LIMIT 10000
//#define EIGEN_NO_MALLOC