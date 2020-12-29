set(SRCS ./my_lib/my_lib.cpp)


add_library(my_lib ${SRCS})

target_compile_features(my_lib PUBLIC cxx_std_20)

target_compile_options(my_lib
                       PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
                       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -Wshadow -Wpointer-arith -O3>)

target_include_directories(my_lib PUBLIC ${CMAKE_CURRENT_LIST_DIR})