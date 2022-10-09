cmake_minimum_required(VERSION 3.12)

project(PainPerduProject VERSION 1.3.0 DESCRIPTION "An example")

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(EMSCRIPTEN "EMSCRIPTEN" OFF)

add_subdirectory(lib)

if (EMSCRIPTEN)
    set(EM_PP_OUTPUT_DIR "${CMAKE_CURRENT_LIST_DIR}/public/generated")
else()
    add_subdirectory(bin)                    
                        
    include(CTest)
    include(addTest)
    enable_testing(MAKE_DIRECTORY test)
    add_subdirectory(test)
endif()