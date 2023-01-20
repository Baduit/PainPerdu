# ppb = PainPerdu Baker
add_executable(ppb main.cpp)
target_compile_options(ppb
					   PRIVATE
					   $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
					   $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -Wshadow -Wpointer-arith -O3>)
target_link_libraries(ppb PRIVATE PainPerdu)   