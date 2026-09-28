include(FetchContent)
FetchContent_Declare(
	unity
	GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
	GIT_TAG v2.7.0
)
FetchContent_MakeAvailable(unity)

add_executable(test__api
	${CMAKE_CURRENT_SOURCE_DIR}/bbfpm/test_bbfpm.c
)
target_link_libraries(test__api libbbfpm unity)

add_test(NAME libbbfpm__test__api COMMAND test__api)

set_tests_properties(libbbfpm__test__api PROPERTIES
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
