
find_package(Threads REQUIRED)

add_executable(web_pain_perdu
               main.cpp
               external/httplib.cc)
target_compile_features(web_pain_perdu PRIVATE cxx_std_20)
target_compile_options(web_pain_perdu
                       PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
                       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -Wshadow -Wpointer-arith -O3>)
target_include_directories(web_pain_perdu PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(web_pain_perdu PRIVATE PainPerdu Threads::Threads)   