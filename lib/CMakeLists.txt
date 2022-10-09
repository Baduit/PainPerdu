set(SRCS
    ${CMAKE_CURRENT_LIST_DIR}/PainPerdu/PainPerdu.cpp
    ${CMAKE_CURRENT_LIST_DIR}/PainPerdu/parser/Parser.cpp
    ${CMAKE_CURRENT_LIST_DIR}/PainPerdu/vm/VirtualMachine.cpp
    ${CMAKE_CURRENT_LIST_DIR}/PainPerdu/vm/Memory.cpp)

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
target_link_libraries(PainPerdu PRIVATE nameof::nameof magic_enum::magic_enum taocpp::pegtl)

if (EMSCRIPTEN)
set(FORCE_EXCEPTION_FLAG "-fwasm-exceptions")

    add_executable(EmPainPerdu ${CMAKE_CURRENT_LIST_DIR}/EmPainPerdu/EmPainPerdu.cpp)
    target_compile_features(EmPainPerdu PUBLIC cxx_std_20)
    target_compile_options(EmPainPerdu PRIVATE -Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -Wshadow -Wpointer-arith -O3 ${FORCE_EXCEPTION_FLAG})
    target_link_options(EmPainPerdu PRIVATE --bind ${FORCE_EXCEPTION_FLAG})
    target_include_directories(EmPainPerdu PUBLIC ${CMAKE_CURRENT_LIST_DIR})
    target_link_libraries(EmPainPerdu PRIVATE PainPerdu)
    set_target_properties(EmPainPerdu PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../docs/generated")
endif()
