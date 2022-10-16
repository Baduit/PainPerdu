set(FORCE_EXCEPTION_FLAG "-fwasm-exceptions")

add_executable(EmPainPerdu EmPainPerdu.cpp)
target_compile_options(EmPainPerdu PRIVATE -Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -Wshadow -Wpointer-arith -O3 ${FORCE_EXCEPTION_FLAG})
target_link_options(EmPainPerdu PRIVATE --bind ${FORCE_EXCEPTION_FLAG})
target_include_directories(EmPainPerdu PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(EmPainPerdu PRIVATE PainPerdu)
set_target_properties(EmPainPerdu PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../../website/generated")