set(SRCS
    ./PainPerdu/PainPerdu.cpp
    ./PainPerdu/parser/Parser.cpp
    ./PainPerdu/vm/VirtualMachine.cpp
    ./PainPerdu/vm/Memory.cpp)

add_subdirectory(external/crepuscule/lib)

find_package(pegtl CONFIG REQUIRED)
find_package(nameof CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
find_path(RANG_INCLUDE_DIRS "rang.hpp")

add_library(PainPerdu ${SRCS})
target_compile_features(PainPerdu PUBLIC cxx_std_20)
target_compile_options(PainPerdu
                       PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
                       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -Wshadow -Wpointer-arith -O3>)
target_include_directories(PainPerdu PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${RANG_INCLUDE_DIRS})
target_link_libraries(PainPerdu PUBLIC crepuscule nameof::nameof magic_enum::magic_enum taocpp::pegtl)
