cmake_minimum_required(VERSION 3.15)
project(libmem VERSION 0.1.0
				DESCRIPTION "Memory Management Utilities Library"
				HOMEPAGE_URL "https://github.com/welljsjs/memory-management-c"
				LANGUAGES C)

# include(CTest)
# enable_testing()

############################################################
# Create a library
############################################################

# Generate static library from library sources
add_library (mem STATIC
	src/assert.c
	src/except.c
	src/mem.c
	src/memchk.c
)

target_include_directories (mem 
	PUBLIC 
	${PROJECT_SOURCE_DIR}/include
)

set_target_properties (mem PROPERTIES 
	C_STANDARD 11
	C_EXTENSIONS OFF
	C_STANDARD_REQUIRED ON
)

target_compile_options (mem PUBLIC
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic-errors -Wmissing-prototypes -Wformat-security -Wswitch-enum>
)

include (CheckIncludeFiles)
CHECK_INCLUDE_FILES (stdlib.h HAVE_STDLIB_H)
CHECK_INCLUDE_FILES (stdio.h HAVE_STDIO_H)
CHECK_INCLUDE_FILES (stddef.h HAVE_STDDEF_H)
CHECK_INCLUDE_FILES (string.h HAVE_STRING_H)
CHECK_INCLUDE_FILES (setjmp.h HAVE_SETJMP_H)
CHECK_INCLUDE_FILES (malloc.h HAVE_MALLOC_H)

include(CheckSymbolExists)
check_symbol_exists (setjmp setjmp.h HAVE_SETJMP)
check_symbol_exists (longjmp setjmp.h HAVE_LONGJMP)
check_symbol_exists(malloc stdlib.h HAVE_MALLOC)
check_symbol_exists(calloc stdlib.h HAVE_CALLOC)
check_symbol_exists(free stdlib.h HAVE_FREE)
check_symbol_exists(fprintf stdio.h HAVE_FPRINTF)
check_symbol_exists(memcpy string.h HAVE_MEMCPY)
check_symbol_exists(memset string.h HAVE_MEMSET)
check_symbol_exists(memmove string.h HAVE_MEMMOVE)

configure_file (${CMAKE_CURRENT_SOURCE_DIR}/include/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/config.h ESCAPE_QUOTES)

set (CPACK_PROJECT_NAME ${PROJECT_NAME})
set (CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include (CPack)
