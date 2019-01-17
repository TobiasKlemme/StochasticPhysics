enable_language(CXX)
 
 message(STATUS "Reported configuration types:  ${CMAKE_CONFIGURATION_TYPES}")
 message(STATUS "Reported compiler:  ${CMAKE_CXX_COMPILER_ID}")

 if (MSVC AND NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # using Visual Studio C++
  LIST(APPEND MSVC_WARNINGS "/W4;/w44640;/w14242;/w14254;/w14263;/w14265;/w14287;/we4289;/w14296;/w14311;/w14545;/w14546;/w14547;/w14549;/w14555;")
  LIST(APPEND MSVC_WARNINGS "/w14619;/w14640;/w14826;/w14905;/w14906;/w14928")
  set(COMMON_OPTIONS "${MSVC_WARNINGS};/MP;/bigobj;/EHsc;/permissive-;/arch:AVX")
  set(COMMON_RELEASE_OPTIONS "${COMMON_OPTIONS};/O2;/Oi;/Ot;/Oy;/fp:fast;/fp:except-;/Qvec;/JMC;")
  #Note: O2 is better then Ox (Ox is old and deprecated)
  add_compile_options("$<$<CONFIG:RELEASE>:${COMMON_RELEASE_OPTIONS}>")
  add_compile_options("$<$<CONFIG:RELWITHDEBINFO>:${COMMON_RELEASE_OPTIONS}>")
  add_compile_options("$<$<CONFIG:DEBUG>:${COMMON_OPTIONS}>")
  #add_definitions(-DEIGEN_ENABLE_AVX512)
  #add_definitions(-D__FAST_MATH__)
  if ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # using Clang-Cl
	LIST(APPEND MSVC_CLANG_WARNINGS "-Wall;-Wextra;-Wshadow;-Wnon-virtual-dtor;-Wold-style-cast;-Wcast-align;-Wunused;-Woverloaded-virtual;-Wpedantic;")
	LIST(APPEND MSVC_CLANG_WARNINGS "-Wconversion;-Wsign-conversion;-Wduplicate-enum;-Wnull-dereference;-Wdouble-promotion;-Wformat=2;-Weffc++;" )
	LIST(APPEND CLANG_SILENCED "-Wno-c++98-compat;-Wno-c++98-compat-pedantic;-Wno-documentation-deprecated-sync;-Wno-undef;-Wno-documentation;-Wno-global-constructors;")
	LIST(APPEND CLANG_SILENCED "-Wno-exit-time-destructors;-Wno-documentation-unknown-command;-Wno-zero-as-null-pointer-constant;-Wno-old-style-cast;-Wno-switch-enum;")
	LIST(APPEND CLANG_SILENCED "-Wno-cast-align;-Wno-covered-switch-default;-Wno-deprecated-dynamic-exception-spec;-Wno-date-time;-Wno-language-extension-token;")
	LIST(APPEND CLANG_SILENCED "-Wno-extra-semi;-Wno-reserved-id-macro;-Wno-missing-noreturn;-Wno-unused-template")

	set(MY_RELEASE_OPTIONS "${MSVC_CLANG_WARNINGS};${CLANG_SILENCED};-fcolor-diagnostics;")
	set(MY_DEBUG_OPTIONS "${MSVC_CLANG_WARNINGS};${CLANG_SILENCED}")
	add_compile_options("$<$<CONFIG:DEBUG>:${MY_DEBUG_OPTIONS}>")
	add_compile_options("$<$<CONFIG:RELEASE>:${MY_RELEASE_OPTIONS}>")
	add_compile_options("$<$<CONFIG:RELWITHDEBINFO>:${MY_RELEASE_OPTIONS}>")
	message(STATUS "Clang-Cl compiler switches set!")
  else()	
	set(MY_DEBUG_OPTIONS "")
	set(MY_RELEASE_OPTIONS "")
	add_compile_options("$<$<CONFIG:DEBUG>:${MY_DEBUG_OPTIONS}>")
	add_compile_options("$<$<CONFIG:RELEASE>:${MY_RELEASE_OPTIONS}>")
	add_compile_options("$<$<CONFIG:RELWITHDEBINFO>:${MY_RELEASE_OPTIONS}>")
	message(STATUS "VS C++ compiler switches set!")
  endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND NOT MSVC)
    LIST(APPEND CLANG_WARNINGS "-Wall;-Wextra;-Wshadow;-Wnon-virtual-dtor;-Wold-style-cast;-Wcast-align;-Wunused;-Woverloaded-virtual;-Wpedantic;-Wconversion;")
	LIST(APPEND CLANG_WARNINGS "-Wsign-conversion;-Wmisleading-indentation;-Wduplicated-cond;-Wduplicated-branches;-Wlogical-op;-Wnull-dereference;-Wuseless-cast;")
	LIST(APPEND CLANG_WARNINGS "-Wdouble-promotion;-Wformat=2;-Weffc++" )
	if(!MINGW)
	add_compile_options("-stdlib=libc++")
	endif()
	add_compile_options("-O3")
	add_compile_options("-Ofast")
	add_compile_options("-fvectorize")
	#add_compile_options("-fsanitize=address -fsanitize=thread -fsanitize=leak -fsanitize=undefined ")
	add_compile_options("-march=skylake-avx512")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lc++ -lc++abi -lc++experimental -lc++fs")
	add_compile_options("${CLANG_WARNINGS}")
	add_compile_options("-m64")
	#add_compile_options("-fsanitize=undefined")
	#add_definitions("-v")
	#add_compile_options("-pedantic")
	#add_compile_options("-flto") # does not work
	#add_compile_options("-nostdinc")
	#add_compile_options("-isystem/usr/include/c++/4.9")
	#add_compile_options("-isystem/usr/include/clang/4.0.0/include")
	#add_compile_options("-i/usr/local/include/c++/v1")
	#add_compile_options("-isystem/usr/local/include")
	#add_compile_options("-isystem/usr/include/x86_64-linux-gnu/")
	#add_compile_options("-isystem/usr/include/x86_64-linux-gnu/c++/4.9")
	#add_compile_options("-iisystem/usr/include/eigen3")
	add_compile_options("-v")
	#add_compile_options("-lc++abi")
  # using Clang
  message(STATUS "Clang compiler switches set!")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") 
  # using GCC
  if(!MINGW)
	add_compile_options("-stdlib=libc++")
	#add_compile_options("-lc++abi")
    message(STATUS "GCC compiler switches set!")
  endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # using Intel C++
  message(STATUS "Intel compiler switches not set!")

else("${CMAKE_CXX_COMPILER_ID}" STREQUAL "")
  message(STATUS "CMAKE_CXX_COMPILER_ID not set!")
endif()