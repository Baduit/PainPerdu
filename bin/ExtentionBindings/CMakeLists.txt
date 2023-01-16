set(FORCE_EXCEPTION_FLAG "-fwasm-exceptions")

add_executable(ExtentionBindings ExtentionBindings.cpp)
target_compile_options(ExtentionBindings PRIVATE -Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -Wshadow -Wpointer-arith -O3 ${FORCE_EXCEPTION_FLAG})
target_link_options(ExtentionBindings PRIVATE --bind ${FORCE_EXCEPTION_FLAG})
target_include_directories(ExtentionBindings PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(ExtentionBindings PRIVATE PainPerdu)
# Later in  the vscode extension directory
#set_target_properties(ExtentionBindings PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../../website/generated")