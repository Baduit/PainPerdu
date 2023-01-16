cmake_minimum_required(VERSION 3.12)

project(PainPerduProject VERSION 1.3.0 DESCRIPTION "An example")

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(EMSCRIPTEN "EMSCRIPTEN" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(lib)

if (EMSCRIPTEN)
    add_subdirectory(bin/EmPainPerdu)
    add_subdirectory(bin/ExtentionBindings)
else()
    add_subdirectory(bin/interpreter)                    
endif()                        


include(CTest)
include(addTest)
enable_testing(MAKE_DIRECTORY test)
add_subdirectory(test)

