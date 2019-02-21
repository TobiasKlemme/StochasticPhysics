OPTION(General_Benchmarks "Enable building of benchmarks" ON)
OPTION(General_Tests "Enable building of tests" ON)
OPTION(General_AVX512 "Enable Compiling with AVX512" OFF)
OPTION(General_FAST_MATH "Enable FAST_MATH" OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (CMAKE_COMPILER_IS_GNUCC)
  option(General_Coverage "Enable coverage reporting for gcc/clang" FALSE)
  
    if (General_Coverage)
    add_compile_options(--coverage -O0)
  endif()
endif()
