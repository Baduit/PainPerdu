set(SRCS
    ${CMAKE_CURRENT_LIST_DIR}/PainPerdu/PainPerdu.cpp
    ${CMAKE_CURRENT_LIST_DIR}/PainPerdu/parser/Parser.cpp
    ${CMAKE_CURRENT_LIST_DIR}/PainPerdu/vm/VirtualMachine.cpp
    ${CMAKE_CURRENT_LIST_DIR}/PainPerdu/vm/Memory.cpp)

find_package(pegtl CONFIG REQUIRED)

if (NOT CHEERP)
    add_library(PainPerdu ${SRCS})
    target_compile_options(PainPerdu
                           PRIVATE
                           $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
                           $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -Wshadow -Wpointer-arith -O3>)

    target_include_directories(PainPerdu PUBLIC ${CMAKE_CURRENT_LIST_DIR})
    target_link_libraries(PainPerdu PRIVATE taocpp::pegtl)
else()
    add_library(PainPerdu ${SRCS})
    target_compile_options(PainPerdu
                        PRIVATE
                        $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
                        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -Wshadow -Wpointer-arith -O3 -fexceptions>)
    # In my version of cheerp target_compile_features does not work and pegtl use it,
    # but it is a header only library so I can just add the files in my include path
    get_target_property(BIDOUILLE_PEGTL_INCLUDE_DIRECTORIES taocpp::pegtl INTERFACE_INCLUDE_DIRECTORIES)
    target_include_directories(PainPerdu PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${BIDOUILLE_PEGTL_INCLUDE_DIRECTORIES})
endif()