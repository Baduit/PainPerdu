cmake_minimum_required(VERSION 3.12)

project(PainPerduProject VERSION 0.1 DESCRIPTION "An example")

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(lib)
add_subdirectory(bin)                    
                      
include(CTest)
include(addTest)
enable_testing(MAKE_DIRECTORY test)
add_subdirectory(test)