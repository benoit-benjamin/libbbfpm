cmake_minimum_required(VERSION 3.16)
project(libbbfpm LANGUAGES C VERSION "1.0.0")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(libbbfpm SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/src/helper/logging/logging.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/helper/io/input/file.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/bbfpm/bbfpm.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/lexer/lexer.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/parser/parser.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/schema/schema.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/data_mapper/data_mapper.c
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
	POSITION_INDEPENDENT_CODE ON
	C_VISIBILITY_PRESET hidden
)
