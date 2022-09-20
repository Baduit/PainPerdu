
find_package(Threads REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_path(CPP_HTTPLIB_INCLUDE_DIRS "httplib.h")

add_executable(web_pain_perdu main.cpp)
target_compile_features(web_pain_perdu PRIVATE cxx_std_20)
target_compile_options(web_pain_perdu
                       PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
                       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -Wshadow -Wpointer-arith -O3>)
target_include_directories(web_pain_perdu PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CPP_HTTPLIB_INCLUDE_DIRS})
target_link_libraries(web_pain_perdu PRIVATE PainPerdu Threads::Threads nlohmann_json::nlohmann_json)   