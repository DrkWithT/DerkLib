add_executable(test_typelist)
target_include_directories(test_typelist PUBLIC ${DERKLIB_INCLUDES})
target_sources(test_typelist PRIVATE test_typelist.cpp)
add_test(NAME test_typelist COMMAND "$<TARGET_FILE:test_typelist>")

add_executable(test_graphs)
target_include_directories(test_graphs PUBLIC ${DERKLIB_INCLUDES})
target_sources(test_graphs PRIVATE test_graphs.cpp)
add_test(NAME test_graphs COMMAND "$<TARGET_FILE:test_graphs>")
