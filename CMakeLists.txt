cmake_minimum_required(VERSION 3.16)
project(libbbfpm LANGUAGES C VERSION "1.0.0")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# =============== MODULES ===============

# --------------- HELPERS ---------------
add_library(libbbfpm__helper__logging OBJECT
	${CMAKE_CURRENT_SOURCE_DIR}/src/helper/logging/logging.c
)
target_include_directories(libbbfpm__helper__logging PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include/helper
)

add_library(libbbfpm__helper__io__output INTERFACE)
target_include_directories(libbbfpm__helper__io__output INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/include/helper/io
)

add_library(libbbfpm__helper__io__input OBJECT
	${CMAKE_CURRENT_SOURCE_DIR}/src/helper/io/input/file.c
)
target_include_directories(libbbfpm__helper__io__input PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include/helper/io
)
target_link_libraries(libbbfpm__helper__io__input
	PRIVATE
		libbbfpm__helper__logging
)

# --------------- LEXER ---------------
add_library(libbbfpm__lexer OBJECT
	${CMAKE_CURRENT_SOURCE_DIR}/src/lexer/lexer.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/lexer/primitives.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/lexer/types.c
)
target_include_directories(libbbfpm__lexer PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(libbbfpm__lexer
	PRIVATE
		libbbfpm__helper__logging
	PUBLIC
		libbbfpm__helper__io__output
		libbbfpm__helper__io__input
)

# --------------- PARSER ---------------
add_library(libbbfpm__parser OBJECT
	${CMAKE_CURRENT_SOURCE_DIR}/src/parser/parser.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/parser/primitives.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/parser/types.c
)
target_include_directories(libbbfpm__parser PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(libbbfpm__parser
	PRIVATE
		libbbfpm__helper__logging
	PUBLIC
		libbbfpm__helper__io__output
		libbbfpm__lexer
)

# --------------- SCHEMA ---------------
add_library(libbbfpm__schema OBJECT
	${CMAKE_CURRENT_SOURCE_DIR}/src/schema/schema.c
)
target_include_directories(libbbfpm__schema PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(libbbfpm__schema
	PRIVATE
		libbbfpm__helper__logging
		libbbfpm__data_mapper
	PUBLIC
		libbbfpm__parser
)

# --------------- DATA_MAPPER ---------------
add_library(libbbfpm__data_mapper OBJECT
	${CMAKE_CURRENT_SOURCE_DIR}/src/data_mapper/data_mapper.c
)
target_include_directories(libbbfpm__data_mapper PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(libbbfpm__data_mapper
	PUBLIC
		libbbfpm__parser
		libbbfpm__helper__io__output
)

# --------------- API ---------------
add_library(libbbfpm__api OBJECT
	${CMAKE_CURRENT_SOURCE_DIR}/src/bbfpm/bbfpm.c
)
target_include_directories(libbbfpm__api PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(libbbfpm__api
	PRIVATE
		libbbfpm__data_mapper
		libbbfpm__parser
		libbbfpm__schema
)

# =============== LIBRARY ===============

add_library(libbbfpm SHARED)
target_link_libraries(libbbfpm PRIVATE
	libbbfpm__helper__logging
	libbbfpm__helper__io__input
	libbbfpm__lexer
	libbbfpm__parser
	libbbfpm__schema
	libbbfpm__data_mapper
	libbbfpm__api
)
target_include_directories(libbbfpm PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(libbbfpm PROPERTIES
	VERSION 1.0.0
	SOVERSION 1
	PUBLIC_HEADER "include/bbfpm/bbfpm.h"
	PREFIX "lib"
	OUTPUT_NAME "bbfpm"
	C_VISIBILITY_PRESET hidden
)

# =============== TESTING ===============

enable_testing()
add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/test
)
