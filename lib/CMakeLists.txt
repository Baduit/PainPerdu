set(SRCS ./PainPerdu/PainPerdu.cpp)

add_subdirectory(external/crepuscule/lib)

add_library(PainPerdu ${SRCS})
target_compile_features(PainPerdu PUBLIC cxx_std_20)
target_compile_options(PainPerdu
                       PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
                       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -Wshadow -Wpointer-arith -O3>)
target_include_directories(PainPerdu PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(PainPerdu PRIVATE crepuscule)
