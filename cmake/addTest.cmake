if (EMSCRIPTEN)
	set(FORCE_EXCEPTION_FLAG "-fwasm-exceptions")
else()
	set(FORCE_EXCEPTION_FLAG "")
endif()

function(addTest test_name test_files)
	set(test_target ${test_name}_test)

	add_executable(${test_target} ${test_files})
	target_compile_options(${test_target}
						PRIVATE
						$<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
						$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -g ${FORCE_EXCEPTION_FLAG}>)
	target_link_libraries(${test_target} PRIVATE PainPerdu doctest::doctest)                       
	target_link_options(${test_target} PRIVATE ${FORCE_EXCEPTION_FLAG})
	target_include_directories(${test_target} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
	add_test(${test_name} ${test_target})
endfunction()